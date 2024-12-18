#include <iostream>
#include <string>
#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include "ThreadStack0.h"
#include "ProcessManipulation.h"
#include "PointerStruct.h"


System::Windows::Forms::Form^* _mainForm;							// SET GLOBAL ACCESS TO FORM	

//// Typedefs for stack pointer types
//#if _WIN64
//typedef uint64_t StackPtrType;
//#else
//typedef uint32_t StackPtrType;
//#endif


struct CLIENT_ID {
	PVOID UniqueProcess;
	PVOID UniqueThread;
};
struct THREAD_BASIC_INFORMATION {
	NTSTATUS ExitStatus;
	PVOID TebBaseAddress;
	CLIENT_ID ClientId;
	KAFFINITY AffinityMask;
	DWORD Priority;
	DWORD BasePriority;
};
enum THREADINFOCLASS {
	ThreadBasicInformation,
};

uintptr_t lpBufferSize;

uint64_t THREADSTACK0::Get_ThreadStack_top(HANDLE process_handle, HANDLE thread_handle, LPCWSTR moduleName) {
	bool loadedManually = false;
	HMODULE module = GetModuleHandleW(moduleName);
	if (!module) {
		GetLastErrorAndPrintInfo();
		module = LoadLibraryW(moduleName);
		if (!module) {
			GetLastErrorAndPrintInfo();
			std::cerr << "Failed to load module: " << moduleName << std::endl;
			return 0;
		}
		loadedManually = true;
	}
	uint64_t result = 0;
	typedef NTSTATUS(__stdcall* NtQueryInformationThread_t)(HANDLE, THREADINFOCLASS, PVOID, ULONG, PULONG);
	NtQueryInformationThread_t NtQueryInformationThread = reinterpret_cast<NtQueryInformationThread_t>(GetProcAddress(module, "NtQueryInformationThread"));
	if (NtQueryInformationThread) {
#ifdef _WIN64
		NT_TIB64 tib = { 0 }; // Use NT_TIB64 for x64 architecture
#else
		NT_TIB tib = { 0 }; // Use NT_TIB for x86 architecture
#endif
		THREAD_BASIC_INFORMATION tbi = { 0 };

		NTSTATUS status = NtQueryInformationThread(thread_handle, ThreadBasicInformation, &tbi, sizeof(tbi), nullptr);
		if (status >= 0) {
			if (ReadProcessMemory(process_handle, tbi.TebBaseAddress, &tib, sizeof(tib), nullptr)) {
#ifdef _WIN64
				result = (uint64_t)tib.StackBase;
#else
				result = (uint32_t)tib.StackBase;
#endif
			}
			else {
				GetLastErrorAndPrintInfo();
				std::cerr << "Error reading process memory: " << std::endl;
			}
		}
		else {
			GetLastErrorAndPrintInfo();
			std::cerr << "Error querying thread information: " << status << std::endl;
		}
	}
	if (loadedManually)
		FreeLibrary(module);
	return result;
}
std::stringstream Convert_UnsignedLongLongAddress_DEC_to_HEX(unsigned long long th)
{
	std::stringstream ss;
	ss << std::uppercase << std::hex << static_cast<int>(th);
	return ss;
}
void SetThreadstack0TextToMainform(unsigned long long th) {
	std::stringstream ss = Convert_UnsignedLongLongAddress_DEC_to_HEX(th);	// ADDRESS(DEC) TO ADDRESS(HEX)	
	std::string stdstr = ss.str();											// STRINGSTREAM TO STD::STRING
	System::String^ sysstr = Convert_StdString_to_SystemString(stdstr);		// STD::STRING TO SYSTEM::STRING
	((System::Windows::Forms::TextBox^)(*_mainForm)->Controls->Find("textBox_ThreadStack", true)[0])->Text = sysstr;
}
void THREADSTACK0::Display_Threadstack_Stacktop_Buffer(HANDLE process_handle, uint64_t stacktop, MODULEINFO& moduleInfo, std::vector<uint64_t>& threadstacks)
{
	int ts_count = 0;
	StackPtrType buf[STACK_SIZE / PTR_SIZE] = { 0 };
	if (ReadProcessMemory(process_handle, (LPCVOID)(stacktop - STACK_SIZE), buf, STACK_SIZE, NULL)) {
		for (int i = STACK_SIZE / PTR_SIZE - 1; i >= 0; --i) {
			//std::cout << std::uppercase << std::dec << "T " << buf[i] <<" -> " << std::hex << buf[i] << std::endl;
			if (buf[i] >= (uint64_t)moduleInfo.lpBaseOfDll && buf[i] <= (uint64_t)moduleInfo.lpBaseOfDll + moduleInfo.SizeOfImage) {
				auto th = stacktop - STACK_SIZE + (i * PTR_SIZE);
				threadstacks.push_back(th);
				std::cout << std::uppercase << std::dec << "THREADSTACK" << ts_count << ": " << th << "\t\tADDR: " << std::hex << th << std::endl;
				std::cout << std::uppercase << std::dec << " STACKTOP:    " << stacktop << "\t\tADDR: " << std::hex << stacktop << std::endl;
				std::cout << std::uppercase << std::dec << " BUFFER:      " << buf[i] << "\tADDR: " << std::hex << buf[i] << std::endl;
				if (ts_count == 0) {
					SetThreadstack0TextToMainform(th);
				}
				ts_count++;
			}
		}
	}
}
void OnlyReadPointedValues(uintptr_t searchInt, int lastI, uintptr_t* addressPointers, HANDLE hProcess, uintptr_t lpBufferSize, bool result, bool& matchFound) {
	for (int i = 0; i < CountNumberOfNonZeroValuesInArray(addressPointers); i++) {
		int intRead = 0;
		char arrRead[128];

		bool resultArray = ReadProcessMemory(hProcess, (LPCVOID)addressPointers[lastI - 1], &arrRead, sizeof(arrRead), NULL);
		bool resultInt = ReadProcessMemory(hProcess, (LPCVOID)addressPointers[lastI - 1], &intRead, lpBufferSize, NULL);
		if (resultInt == false) {
			GetLastErrorAndPrintInfo();
		}

		if (intRead == searchInt) {		// CHECK IF READ(INT) IS SAME AS WHAT WE ARE SEARCHING FOR
			//if (matchFound == false) {
			matchFound = true;
			std::cout << "...............## MATCH FOUND ## " << std::uppercase << std::hex << addressPointers[lastI - 1] << std::endl;
			_POINTER_MAP pointerMap = {};
			ScanPointerDepthMap(pointerMap, addressPointers, result, hProcess, addressPointers[lastI - 1], lpBufferSize, lastI, matchFound);
			DisplayScanPointerDepthMap(pointerMap, addressPointers, result, hProcess, addressPointers[lastI - 1], lpBufferSize, lastI, matchFound);

			return;
		}
		lastI++;
	}
}
void Display_Read_PointedValues(_POINTER_MAP& pointerMap, uintptr_t searchInt, int lastI, uintptr_t* addressPointers, HANDLE hProcess, uintptr_t lpBufferSize, bool result, bool& matchFound) {
	int intRead = 0;
	char arrRead[128];

	bool resultArray = ReadProcessMemory(hProcess, (LPCVOID)addressPointers[lastI - 1], &arrRead, sizeof(arrRead), NULL);
	bool resultInt = ReadProcessMemory(hProcess, (LPCVOID)addressPointers[lastI - 1], &intRead, lpBufferSize, NULL);
	if (resultInt == false) {
		GetLastErrorAndPrintInfo();
	}

	if (intRead == searchInt || matchFound == true) {		// CHECK IF READ(INT) IS SAME AS WHAT WE ARE SEARCHING FOR
		matchFound = true;
		//std::cout << "...............## MATCH FOUND (Display) ## " << std::uppercase << std::hex << addressPointers[lastI - 1] << std::endl;
		std::cout << "...............## MATCH FOUND (Display) ## " << std::uppercase << std::hex << pointerMap.PointerLevel[lastI - 1].BaseAddress << std::endl;

		std::cout << "## POINTED VALUES ##" << std::endl;
		std::stringstream ss;
		//ss << std::uppercase << "--ReadProcessMemory PTR L" << lastI - 1 << ": " << std::hex << addressPointers[lastI - 1];
		ss << std::uppercase << "--ReadProcessMemory PTR L" << lastI - 1 << ": " << std::hex << pointerMap.PointerLevel[lastI - 1].BaseAddress;
		std::string line = ss.str();
		std::cout << line << std::endl;

		// Int:
		std::stringstream ssInt;
		//ssInt << std::uppercase << "--READ(INT) = " << intRead << "\tREAD(HEX) = " << std::hex << intRead << "\tADDR = " << std::hex << addressPointers[lastI - 1] << std::dec;
		ssInt << std::uppercase << "--READ(INT) = " << intRead << "\tREAD(HEX) = " << std::hex << intRead << "\tADDR = " << std::hex << pointerMap.PointerLevel[lastI - 1].BaseAddress << std::dec;
		std::string ssIntMessage = ssInt.str();
		std::cout << ssIntMessage << std::endl;
		pointerMap.PointerLevel[lastI - 1].BaseAddress_intRead = intRead;
		
		// Array:
		std::stringstream ssArr;
		//ssArr << std::uppercase << "READ(ARRAY) = " << arrRead << "\tREAD(HEX) = " << std::hex << arrRead << "\tADDR = " << std::hex << addressPointers[lastI - 1] << std::dec;
		std::cout << "--READ(ARRAY): \t" << arrRead << std::endl;
		std::cout << "--READ(HEX): \t";
		for (int i = 0; i < strlen(arrRead); ++i) {
			std::cout << std::hex << std::setw(1) << std::setfill('0') << (int)arrRead[i];
		}
		std::cout << std::endl;
		std::cout << "--ADDR: \t\t" << std::hex << addressPointers[lastI - 1] << std::dec << std::endl;
		pointerMap.PointerLevel[lastI - 1].BaseAddress_arrRead = *arrRead;
	}
}
int CountNumberOfNonZeroValuesInArray(uintptr_t* thisArray) {
	int count = 0;
	for (int i = 0; i < sizeof(thisArray); ++i) {
		if (thisArray[i] != 0) {
			count++;
		}
	}
	return count;
}
void THREADSTACK0::PointerScannerFor_IntValue(HANDLE process_handle, DWORD thread_id, LPCWSTR kernelModuleName, LPCWSTR moduleName) {
	HANDLE thread_handle = OpenThread(THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION, FALSE, thread_id);
	if (!thread_handle) {
		GetLastErrorAndPrintInfo();
		return;
	}
	uint64_t stacktop = Get_ThreadStack_top(process_handle, thread_handle, moduleName);

	CloseHandle(thread_handle);
	if (!stacktop) {
		GetLastErrorAndPrintInfo();
		return;
	}
	MODULEINFO moduleInfo;
	HMODULE kernel_module = GetModuleHandle(kernelModuleName);
	if (!kernel_module) {
		GetLastErrorAndPrintInfo();
		return;
	}
	GetModuleInformation(process_handle, kernel_module, &moduleInfo, sizeof(moduleInfo));



	//------------------------------------------------------------------------------------
	/// MOVE THIS TO NEW METHOD: 
	/// POINTER SCANNER FOR VALUE/ADDRESS			DONE ->TODO: MAKE ALSO SCAN FOR ADDRESS


	//uintptr_t searchInt_test = 10050;				//TEMP GET IT FROM mainform -> DONE

	std::string searchInt_stds = GetValueFromTextBoxInStdString(*_mainForm, "textBox_ValueToReadINT");
	unsigned long long searchInt_ulong = std::stoll(searchInt_stds);
	if (searchInt_ulong < 0) {
		MessageBox(NULL, L"Integer < 0. Results may be wrong!", L"ERROR", MB_OK);
	}
	StackPtrType searchInt = static_cast<StackPtrType>(searchInt_ulong);

	_POINTER_MAP_COLLECTION pointerMapCollection = {};
	//_POINTERS_MAP pointerMap;
	int ts_count1 = 0;
	StackPtrType buf1[STACK_SIZE / PTR_SIZE] = { 0 };
	if (ReadProcessMemory(process_handle, (LPCVOID)(stacktop - STACK_SIZE), buf1, STACK_SIZE, NULL)) {
		for (int i = STACK_SIZE / PTR_SIZE - 1; i >= 0; --i) {
			//for (int i = STACK_SIZE  - 1; i >= 0; --i) {
			uint64_t th1 = stacktop - STACK_SIZE + (i * PTR_SIZE);
			/*std::cout << std::uppercase << std::dec << "--THREADSTACK" << ts_count1 << ": " << th1 << "\t\tADDR: " << std::hex << th1 << std::endl;
			std::cout << std::uppercase << std::dec << "-- STACKTOP:    " << stacktop << "\t\tADDR: " << std::hex << stacktop << std::endl;
			std::cout << std::uppercase << std::dec << "-- BUFFER:      " << buf1[i] << "\tADDR: " << std::hex << buf1[i] << std::endl;*/
			ts_count1++;

			uintptr_t PID = GetProcessId(process_handle);
			uintptr_t  address = th1;					//TEMP
			uintptr_t address_s_uintptr_t = address;
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
			if (hProcess == NULL) {
				GetLastErrorAndPrintInfo();
			}

			uintptr_t  addressPointers[pointerScanDepth] = { };									// Set max pointers scan depth
			addressPointers[0] = address_s_uintptr_t;
			bool result = true;
			int lastI = 1;

			bool matchFound = false;
			_POINTER_MAP pointerMap = {};

			//_POINTER_MAP pointerMap = {};
			ScanPointerDepthMap(pointerMap, addressPointers, result, hProcess, address, lpBufferSize, lastI, matchFound);
			//DisplayPointerMapLayout(addressPointers, result, matchFound);
			Display_Read_PointedValues(pointerMap, searchInt, 1, addressPointers, hProcess, lpBufferSize, result, matchFound);	// lastI = 1 -> So we see the first pointed value

			DisplayScanPointerDepthMap(pointerMap, addressPointers, result, hProcess, address, lpBufferSize, lastI, matchFound);
			DisplayPointerMapLayout(pointerMap, addressPointers, result, matchFound);

			Display_Read_PointedValues(pointerMap, searchInt, pointerMap.PointerLevel.size() - 1, addressPointers, hProcess, lpBufferSize, result, matchFound);	// lastI = last baseAddress -1 -> Repeat again for last PTR
			//DisplayPointerMapLayout(addressPointers, result);

			CloseHandle(hProcess);

			if (matchFound) {
				//pointerMapCollection.PointerMap = (pointerMap);
				pointerMapCollection.PointerMap.push_back(pointerMap);
			}
		}
	}
	std::cout << "================================================================" << std::endl;
	std::cout << "                        == SUMMARY ==" << std::endl;
	std::cout << "================================================================" << std::endl;

	for (int pm = 0; pm < pointerMapCollection.PointerMap.size(); ++pm) {
		std::cout << std::uppercase << "Pointer Map PM" << pm << std::endl;
		for (int pl = 0; pl < pointerMapCollection.PointerMap[pm].PointerLevel.size(); ++pl) {
			std::cout << std::uppercase << "ReadProcessMemory L" << pl << ": " << std::hex <<
				pointerMapCollection.PointerMap[pm].PointerLevel[pl].BaseAddress ;

			std::cout << std::uppercase <<std::dec
				<< "\t>>READ(INT) = " << pointerMapCollection.PointerMap[pm].PointerLevel[pl].BaseAddress_intRead
				<< "\tREAD(HEX) = " << std::hex << pointerMapCollection.PointerMap[pm].PointerLevel[pl].BaseAddress_intRead
				//<< "\tADDR = " << std::hex << pointerMapCollection.PointerMap[pm].PointerLevel[pl].BaseAddress
				<< std::dec<<std::endl;
		}
	}
}
std::vector<uint64_t> THREADSTACK0::find_threadstack_for_thread(HANDLE process_handle, DWORD thread_id, LPCWSTR kernelModuleName, LPCWSTR moduleName) {
	std::vector<uint64_t> threadstacks = {};
	HANDLE thread_handle = OpenThread(THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION, FALSE, thread_id);
	if (!thread_handle) {
		GetLastErrorAndPrintInfo();
		return threadstacks;
	}
	uint64_t stacktop = Get_ThreadStack_top(process_handle, thread_handle, moduleName);

	CloseHandle(thread_handle);
	if (!stacktop) {
		GetLastErrorAndPrintInfo();
		return threadstacks;
	}
	MODULEINFO moduleInfo;
	HMODULE kernel_module = GetModuleHandle(kernelModuleName);
	if (!kernel_module) {
		GetLastErrorAndPrintInfo();
		return threadstacks;
	}
	GetModuleInformation(process_handle, kernel_module, &moduleInfo, sizeof(moduleInfo));

	Display_Threadstack_Stacktop_Buffer(process_handle, stacktop, moduleInfo, threadstacks);
	return threadstacks;
}
/// <summary>
/// Gets first thread from selected PID
/// </summary>
/// <param name="pid"></param>
/// <returns></returns>
DWORD THREADSTACK0::get_first_thread_id(DWORD pid) {
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (h == INVALID_HANDLE_VALUE) {
		GetLastErrorAndPrintInfo();
		return -1;
	}
	THREADENTRY32 te = {};
	te.dwSize = sizeof(te);
	if (Thread32First(h, &te)) {
		do {
			if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(te.th32OwnerProcessID)) {
				if (te.th32OwnerProcessID == pid) {
					std::cout << "First Thread ID: " << te.th32ThreadID << std::endl;
					return te.th32ThreadID;
				}
			}
			te.dwSize = sizeof(te);
		} while (Thread32Next(h, &te));
	}

	return -1;
}
void THREADSTACK0::ThreadStack_Runner_PointerScannerFor_IntValue(DWORD pid, LPCWSTR kernelModuleName, LPCWSTR moduleName) {
	try {
		HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (!process_handle || process_handle == INVALID_HANDLE_VALUE) {
			GetLastErrorAndPrintInfo();
			return;
		}
		DWORD thread_id = get_first_thread_id(pid);
		PointerScannerFor_IntValue(process_handle, thread_id, kernelModuleName, moduleName);		// THIS
		return;
	}
	catch (std::invalid_argument&) {
		GetLastErrorAndPrintInfo();
		return;
	}
}
//int main(int argc, char** argv)
void THREADSTACK0::ThreadStack_Runner_find_threadstack_for_thread(DWORD pid, LPCWSTR kernelModuleName, LPCWSTR moduleName) {
	try {
		HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (!process_handle || process_handle == INVALID_HANDLE_VALUE) {
			GetLastErrorAndPrintInfo();
			return;
		}
		DWORD thread_id = get_first_thread_id(pid);
		std::vector<uint64_t> threadstacks = find_threadstack_for_thread(process_handle, thread_id, kernelModuleName, moduleName);// THIS
		return;
	}
	catch (std::invalid_argument&) {
		GetLastErrorAndPrintInfo();
		return;
	}
}
std::string GetValueFromTextBoxInStdString(System::Windows::Forms::Form^ mainForm, System::String^ textBoxName) {
	System::String^ memoryAddress_s = ((System::Windows::Forms::TextBox^)mainForm->Controls->Find(textBoxName, true)[0])->Text;
	return Convert_SystemString_to_StdString(memoryAddress_s);
}
wchar_t* Convert_StdString_to_Wchar(std::string stdString) {
	int len = MultiByteToWideChar(CP_ACP, 0, stdString.c_str(), -1, NULL, 0);
	wchar_t* wideChar_t = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, stdString.c_str(), -1, wideChar_t, len);
	return wideChar_t;
}
/// <summary>
/// Find THREADSTACK0
/// </summary>
/// <param name="mainForm"></param>
void THREADSTACK0_Setup_find_threadstack_for_thread(System::Windows::Forms::Form^ mainForm) {
	_mainForm = &mainForm;															// SET GLOBAL ACCESS TO FORM
	uintptr_t pid = ProcessManipulation::GetPID(mainForm);
	wchar_t* kernelModuleName_w = Convert_StdString_to_Wchar(GetValueFromTextBoxInStdString(mainForm, "textBox_KernelAssemblyName"));
	wchar_t* moduleName_w = Convert_StdString_to_Wchar(GetValueFromTextBoxInStdString(mainForm, "textBox_AssemblyName"));
	ProcessManipulation::GetAppropriate_LpBufferSize_BasedOnProcessArchitecture(mainForm, lpBufferSize);
	THREADSTACK0 ts;
	ts.ThreadStack_Runner_find_threadstack_for_thread(pid, kernelModuleName_w, moduleName_w);
}
void THREADSTACK0_Setup_PointerScannerFor_IntValue(System::Windows::Forms::Form^ mainForm) {
	_mainForm = &mainForm;															// SET GLOBAL ACCESS TO FORM
	uintptr_t pid = ProcessManipulation::GetPID(mainForm);
	wchar_t* kernelModuleName_w = Convert_StdString_to_Wchar(GetValueFromTextBoxInStdString(mainForm, "textBox_KernelAssemblyName"));
	wchar_t* moduleName_w = Convert_StdString_to_Wchar(GetValueFromTextBoxInStdString(mainForm, "textBox_AssemblyName"));
	ProcessManipulation::GetAppropriate_LpBufferSize_BasedOnProcessArchitecture(mainForm, lpBufferSize);
	THREADSTACK0 ts;
	ts.ThreadStack_Runner_PointerScannerFor_IntValue(pid, kernelModuleName_w, moduleName_w);
}





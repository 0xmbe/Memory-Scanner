#include "MemoryPointerScanner.h"
#include "ProcessManipulation.h"

#include <string>
#include <iostream>
#include <Windows.h>

using namespace std;

#include <sstream>
#include "msclr\marshal_cppstd.h"
#include <iomanip>
#include <cstdint>

#include <stdio.h>
#include <tchar.h>
#include <psapi.h>

#include <codecvt>

#include <vector>
#include "ThreadStack0.h"
#include "PointerStruct.h"

//#include <C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\km\ntddk.h>		// ?
//#include <C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\km\wdm.h>
//#include <C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\km\mce.h>

// MACROS:
#define TYPE_TO_STRING(Type) (void(sizeof(Type)),#Type)

//#ifdef _WIN64
//typedef ULONGLONG ADDRESS_TYPE;
//#else
//typedef DWORD ADDRESS_TYPE;
//#endif

class testclass1 {
public:
	static void testvoid2() {

	}
};
class testclass2 {
	void test() {
		testclass1::testvoid2();
	}
};
void test() {
	testclass1::testvoid2();
}

std::string Wstring_to_StdString(std::wstring wstring) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string stdString = converter.to_bytes(wstring);
	return stdString;
}
std::wstring SystemString_to_Wstring(System::String^ systemString) {
	std::string narrowString = msclr::interop::marshal_as<std::string>(systemString);
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::wstring wString = converter.from_bytes(narrowString);
	return wString;
}
void DebugToOutput(const std::string& messageType, std::string messageText, System::Windows::Forms::Form^ mainForm)
{
	std::string fullMessage = messageType + ": " + messageText + "\n";

	std::cerr << fullMessage;

	// Convert std::string to System::String^
	System::String^ fullMessage_ss = gcnew System::String(fullMessage.c_str());

	((System::Windows::Forms::RichTextBox^)mainForm->Controls->
		Find("richTextBox_Output", true)[0])->AppendText(fullMessage_ss);
}
std::string longPtrToHex(LONG_PTR ptr) {
	std::stringstream ss;
	ss << std::hex << std::uppercase << std::setw(sizeof(LONG_PTR) * 2) << std::setfill('0') << ptr;
	return ss.str();
}
unsigned __int64 Convert_StdStringToUint64(const std::string& hexString) {
	std::stringstream ss(hexString);
	unsigned __int64 result;

	if (ss >> std::hex >> result) {
		return result;
	}
	else
	{
		std::cerr << "Invalid hexadecimal string: " << hexString << std::endl;
		return 0;
	}
}
uintptr_t  Convert_StdString_to_uintptr_t(const std::string& hexString) {
	std::stringstream ss(hexString);
	uintptr_t  result;

	if (ss >> std::hex >> result) {
		return result;
	}
	else
	{
		std::cerr << "Invalid hexadecimal string: " << hexString << std::endl;
		return 0;
	}
}
System::String^ Convert_StdString_to_SystemString(const std::string stdstr) {
	return gcnew System::String(stdstr.c_str());
}
std::string Convert_SystemString_to_StdString(System::String^ ss) {
	std::string stds = msclr::interop::marshal_as<std::string>(ss);
	return stds;
}
void WriteToSelectedMemoryAddress(System::Windows::Forms::Form^ mainForm)
{
	bool valueToWriteINT_b = ((System::Windows::Forms::TextBox^)mainForm->Controls->Find("textBox_ValueToWriteINT", true)[0])->Enabled;
	bool valueToWriteHEX_b = ((System::Windows::Forms::TextBox^)mainForm->Controls->Find("textBox_ValueToWriteHEX", true)[0])->Enabled;

	if (valueToWriteHEX_b == true) {
		WriteToMemoryAddress_HEX(mainForm);
	}
	else if (valueToWriteINT_b == true) {
		WriteToMemoryAddress_INT(mainForm);
	}
}

void WriteToMemoryAddress_INT(System::Windows::Forms::Form^ mainForm)
{
	SetLastError(-1);

	uintptr_t PID = 0;
	PID = ProcessManipulation::ProcessManipulation::GetPID(mainForm);
	if (PID == 0) {
		return;
	}

	System::String^ memoryAddress_s = ((System::Windows::Forms::TextBox^)mainForm->Controls->Find("textBox_MemoryAddress", true)[0])->Text;
	std::string memoryAddress_stds = Convert_SystemString_to_StdString(memoryAddress_s);
	unsigned __int64 address_s_int64 = Convert_StdStringToUint64(memoryAddress_stds);
	LONG_PTR address = address_s_int64;

	System::String^ IntWrite_s = ((System::Windows::Forms::TextBox^)mainForm->Controls->Find("textBox_ValueToWriteINT", true)[0])->Text;
	uintptr_t intWrite = System::Int32::Parse(IntWrite_s);

	SIZE_T bytesWritten;
	bool result = true;
	HANDLE hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, PID);
	if (hProcess == NULL) {
		DebugToOutput("ERROR", std::to_string(GetLastError()), mainForm);
	}
	result = WriteProcessMemory(hProcess, (LPVOID)address, &intWrite, sizeof(intWrite), &bytesWritten);
	if (result) {
		if (bytesWritten == sizeof(intWrite)) {
			DebugToOutput("WRITE OK", to_string(intWrite), mainForm);
		}
		else {
			std::stringstream ss;
			ss << "Bytes written (" << bytesWritten << ") does not match the size of intWrite (" << sizeof(intWrite) << ")";
			std::string outputMessage = ss.str();
			DebugToOutput("WRITE ERROR", outputMessage, mainForm);
		}
	}
	else {
		DebugToOutput("ERROR", std::to_string(GetLastError()), mainForm);
	}
}
void WriteToMemoryAddress_HEX(System::Windows::Forms::Form^ mainForm)
{
	SetLastError(-1);

	uintptr_t PID = 0;
	PID = ProcessManipulation::GetPID(mainForm);
	if (PID == 0) {
		return;
	}

	uintptr_t lpBufferSize = 0;
	ProcessManipulation::GetAppropriate_LpBufferSize_BasedOnProcessArchitecture(mainForm, lpBufferSize);

	System::String^ memoryAddress_s = ((System::Windows::Forms::TextBox^)mainForm->Controls->Find("textBox_MemoryAddress", true)[0])->Text;
	std::string memoryAddress_stds = Convert_SystemString_to_StdString(memoryAddress_s);
	unsigned __int64 address_s_int64 = Convert_StdStringToUint64(memoryAddress_stds);
	LONG_PTR address = address_s_int64;

	System::String^ valueToWriteHEX = ((System::Windows::Forms::TextBox^)mainForm->Controls->Find("textBox_ValueToWriteHEX", true)[0])->Text;
	std::string valueToWriteHEX_stds = Convert_SystemString_to_StdString(valueToWriteHEX);
	unsigned __int64 valueToWriteHEX_stds_int64 = Convert_StdStringToUint64(valueToWriteHEX_stds);


	SIZE_T bytesWritten;
	bool result = true;
	HANDLE hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, PID);
	if (hProcess == NULL) {
		DebugToOutput("ERROR", std::to_string(GetLastError()), mainForm);
	}
	result = WriteProcessMemory(hProcess, (LPVOID)address, &valueToWriteHEX_stds_int64, lpBufferSize, &bytesWritten);
	if (result) {
		if (bytesWritten == sizeof(valueToWriteHEX_stds_int64)) {
			DebugToOutput("WRITE OK", to_string(valueToWriteHEX_stds_int64), mainForm);
		}
		else {
			std::stringstream ss;
			ss << "Bytes written (" << bytesWritten << ") does not match the size of intWrite (" << lpBufferSize << ")";
			std::string outputMessage = ss.str();
			DebugToOutput("WRITE ERROR", outputMessage, mainForm);
		}
	}
	else {
		DebugToOutput("ERROR", std::to_string(GetLastError()), mainForm);
	}
}
void GetLastErrorAndPrintInfo()
{
	DWORD lastError = GetLastError();
	if (lastError != 0) {
		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,

			lastError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&messageBuffer,
			0,
			NULL);

		if (size > 0) {
			std::cerr << "Error: " << lastError << " " << messageBuffer;
			LocalFree(messageBuffer);
		}
		else {
			std::cerr << "Error: Unknown error code " << lastError;
		}
		std::cerr << std::endl;
	}
}
StackPtrType FindPointerInStackOfThatProcessThatPointsToSpecificAddress(HANDLE hProcess, StackPtrType stackBase, StackPtrType stackSize, StackPtrType targetAddress) {
	StackPtrType stackTop = stackBase;
	StackPtrType stackBottom = stackBase - stackSize;
	//for (StackPtrType currentAddress = stackBottom; currentAddress <= stackTop; currentAddress += sizeof(StackPtrType)) {
	for (StackPtrType currentAddress = stackBottom; currentAddress <= stackTop; currentAddress += 1) {
		StackPtrType value = 0;
		SIZE_T bytesRead = 0;
		//HANDLE currentProcess = GetCurrentProcess(); // Get the current process handle
		LPCVOID addressToRead = reinterpret_cast<LPCVOID>(currentAddress); // Convert the current address to a constant void pointer
		BOOL readSuccess = ReadProcessMemory(hProcess, addressToRead, &value, sizeof(StackPtrType), &bytesRead);
		if (readSuccess && bytesRead == sizeof(StackPtrType)) {
			if (value == targetAddress) {
				std::cout << std::uppercase << std::hex << currentAddress << " -> This pointer points to Target Address." << std::endl;
				return currentAddress;
			}
		}
	}
	//MessageBox(NULL, L"No pointer to memory address found!", L"ERROR", MB_OK);
	std::cout << "ERROR: NO POINTER TO MEMORY ADDRESS FROM THREADSTACK0 FOUND!" << std::endl;
	return 0; // Pointer not found in stack 
}

StackPtrType calculateOffset(StackPtrType threadstack0, StackPtrType pointerAddress) {
	return threadstack0 - pointerAddress;
}
void FindPointerToSelectedMemoryAddress(System::Windows::Forms::Form^ mainForm) {
	SetLastError(-1);
	uintptr_t PID = 0;
	PID = ProcessManipulation::GetPID(mainForm);
	if (PID == 0) {
		return;
	}
	DebugToOutput("START: FindPointerToSelectedMemoryAddress", "================================================", mainForm);

	DebugToOutput("OpenProcess", std::to_string(PID), mainForm);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (hProcess == NULL) {
		GetLastErrorAndPrintInfo();
	}
	std::cout << std::uppercase << "Handle: " << hProcess << std::endl;

	StackPtrType stackSize = 1024;					// Get that from calculation
	stackSize = STACK_SIZE;							// This gets 4096 
	StackPtrType threadstack0_test = 0x00C3FF10;		// Get that from form
	System::String^ threadstack0_ss = ((System::Windows::Forms::TextBox^)mainForm->Controls->Find("textBox_ThreadStack", true)[0])->Text;
	std::string threadstack0_stds = Convert_SystemString_to_StdString(threadstack0_ss);
	if (threadstack0_stds == "") {
		MessageBox(NULL, L"Get THREADSTACK0 First!", L"ERROR", MB_OK);
		return;
	}
	StackPtrType threadstack0 = std::stoull(threadstack0_stds, nullptr, 16);

	StackPtrType targetAddress_test = 0x00C3FE54;		// Get that from form
	System::String^ targetAddress_ss = ((System::Windows::Forms::TextBox^)mainForm->Controls->Find("textBox_MemoryAddress", true)[0])->Text;
	std::string targetAddress_stds = Convert_SystemString_to_StdString(targetAddress_ss);
	if (targetAddress_stds == "") {
		MessageBox(NULL, L"Set Memory Address First!", L"ERROR", MB_OK);
		return;
	}
	StackPtrType targetAddress = std::stoull(targetAddress_stds, nullptr, 16);

	uintptr_t lpBufferSize = 0;
	ProcessManipulation::GetAppropriate_LpBufferSize_BasedOnProcessArchitecture(mainForm, lpBufferSize);

	StackPtrType pointerAddress = FindPointerInStackOfThatProcessThatPointsToSpecificAddress(hProcess, threadstack0, stackSize, targetAddress);
	if (pointerAddress != 0) {
		StackPtrType offset = calculateOffset(threadstack0, pointerAddress);
		char negativeOffset = ' ';
		if (threadstack0 > pointerAddress) {
			negativeOffset = '-';
		}
		std::cout << std::uppercase << "Threadstack0: " << std::hex << threadstack0 << std::endl;
		std::cout << std::uppercase << "Offset from threadstack0 to pointer address: " << negativeOffset << "0x" << std::hex << offset << "\tDEC: " << negativeOffset << std::dec << offset << std::endl;
		std::cout << std::uppercase << "Pointer from Threadstack0 to target value is at: 0x" << std::hex << pointerAddress << "\tDEC: " << std::dec << pointerAddress << std::endl;

	}
}

/// <summary>
/// Scans the Memory Address, 
/// follows the pointer to the last address not pointing anywhere, 
/// gets the value of that memory address.
/// </summary>
/// <param name="mainForm"></param>
void ScanSelectedMemoryAddress(System::Windows::Forms::Form^ mainForm)
{
	SetLastError(-1);
	uintptr_t PID = 0;
	PID = ProcessManipulation::GetPID(mainForm);
	if (PID == 0) {
		return;
	}

	int searchInt = 0;				// Not used with Scan Selected Address, but has to be here.

	uintptr_t lpBufferSize = 0;
	ProcessManipulation::GetAppropriate_LpBufferSize_BasedOnProcessArchitecture(mainForm, lpBufferSize);

	System::String^ memoryAddress_s = ((System::Windows::Forms::TextBox^)mainForm->Controls->Find("textBox_MemoryAddress", true)[0])->Text;
	std::string memoryAddress_stds = Convert_SystemString_to_StdString(memoryAddress_s);
	uintptr_t address_s_uintptr_t = Convert_StdString_to_uintptr_t(memoryAddress_stds);	//-->> USE uintptr_t, MOST UNIVERSAL
	uintptr_t  address = address_s_uintptr_t;

	DebugToOutput("START: ScanSelectedMemoryAddress", "================================================", mainForm);
	DebugToOutput("OpenProcess", std::to_string(PID), mainForm);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (hProcess == NULL) {
		GetLastErrorAndPrintInfo();
	}

	uintptr_t  addressPointers[pointerScanDepth] = { 0 };									// Set max pointers scan depth
	addressPointers[0] = address_s_uintptr_t;
	bool result = true;
	int lastI = 1;

	bool matchFound = true;			// THIS IS SET TO TRUE BECAUSE WE ARE SCANNING ONLY 1 ADDRESS
	_POINTER_MAP pointerMap = {};
	ScanPointerDepthMap(pointerMap, addressPointers, result, hProcess, address, lpBufferSize, lastI, matchFound);

	Display_Read_PointedValues(pointerMap, searchInt, 1, addressPointers, hProcess, lpBufferSize, result, matchFound);	// lastI = 1 -> So we see the first pointed value

	Display_Read_PointedValues(pointerMap, searchInt, pointerMap.PointerLevel.size() - 1, addressPointers, hProcess, lpBufferSize, result, matchFound);	// lastI = last baseAddress -1 -> Repeat again for last PTR

	CloseHandle(hProcess);
}
void ScanPointerDepthMap(_POINTER_MAP& pointerMap, uintptr_t* addressPointers, bool& result, HANDLE hProcess, uintptr_t& address, uintptr_t lpBufferSize, int& lastI, bool& matchFound)
{
std:cout << "##... SCANNING POINTER DEPTH MAP ##" << std::endl;
	for (int i = 1; i < pointerScanDepth; ++i)
	{
		int offset = 0;
		StackPtrType addresOffset = 0;
		if (i > 1) {	//>2 because we start with 1, so we skip 1st.
			addresOffset = pointerMap.PointerLevel[i - 2].TargetAddress - pointerMap.PointerLevel[i - 2].BaseAddress;
		}
		_POINTER pointer = { 0,0 };
		pointerMap.PointerLevel.push_back(pointer);
		pointerMap.PointerLevel[i - 1].BaseAddress = addressPointers[i - 1];

		std::stringstream ss;
		//ss << std::uppercase << "ReadProcessMemory L" << i - 1 << ": " << std::hex << addressPointers[i - 1];
		ss << std::uppercase << "ReadProcessMemory L" << i - 1 << ": " << std::hex << pointerMap.PointerLevel[i - 1].BaseAddress << "\tOFFSET(HEX): " << addresOffset << "\tOFFSET(DEC): " << std::dec << addresOffset;
		std::string line = ss.str();
		std::cout << line << std::endl;

		result = ReadProcessMemory(hProcess, (LPCVOID)address, &addressPointers[i], lpBufferSize, NULL);	//  sizeof(uintptr_t)	//--> THIS IS FOR x86 x64
		if (!result) {
			GetLastErrorAndPrintInfo();
			break;
		}

		//pointerMap.PointerLevel[i - 1].BaseAddress_intRead = static_cast<int>(addressPointers[i]);
		pointerMap.PointerLevel[i - 1].BaseAddress_intRead = reinterpret_cast<int>(&addressPointers[i]);

		pointerMap.PointerLevel[i - 1].TargetAddress = addressPointers[i];



		//lastI = i;		// IN scanning don't set lastI because we don't want ti tto effect next results.
		//int test = 33;
		address = addressPointers[i];
	}
}
void DisplayScanPointerDepthMap(_POINTER_MAP& pointerMap, uintptr_t* addressPointers, bool& result, HANDLE hProcess, uintptr_t& address, uintptr_t lpBufferSize, int& lastI, bool& matchFound)
{
	if (matchFound) {
	std:cout << "## POINTER DEPTH MAP ##" << std::endl;
		for (int i = 1; i < pointerScanDepth; ++i)
		{
			std::stringstream ss;
			/*ss << std::uppercase << "ReadProcessMemory L" << i - 1 << ": " << std::hex << addressPointers[i - 1];*/
			ss << std::uppercase << "ReadProcessMemory L" << i - 1 << ": " << std::hex << pointerMap.PointerLevel[i - 1].BaseAddress;
			std::string line = ss.str();
			std::cout << line << std::endl;				// SOMETHING IS NOT RIGHT DOWN HERE
			result = ReadProcessMemory(hProcess, (LPCVOID)address, &addressPointers[i], lpBufferSize, NULL);	//  sizeof(uintptr_t)	//--> THIS IS FOR x86 x64
			if (!result) {
				GetLastErrorAndPrintInfo();
				break;
			}
			lastI = i;
			address = addressPointers[i];
		}
	}
}
void DisplayPointerMapLayout(_POINTER_MAP& pointerMap, uintptr_t* addressPointers, bool& result, bool& matchFound) {
	if (matchFound) {
		std::cout << "## POINTER MAP LAYOUT ##" << std::endl;

		int size = sizeof(addressPointers) / sizeof(addressPointers[0]);


		//for (int i = 1; i < size; ++i) {
		for (int i = 1; i < sizeof(addressPointers); ++i) {
			if (addressPointers[i] != 0) {
				std::stringstream ss;
				//ss << std::uppercase << "PTR L" << i - 1 << ": " << std::hex << addressPointers[i - 1] <<
				ss << std::uppercase << "PTR L" << i - 1 << ": " << std::hex << pointerMap.PointerLevel[i - 1].BaseAddress <<
					" --> " << "PTR L" << i << ": " << std::hex << pointerMap.PointerLevel[i - 1].TargetAddress;
				std::string line = ss.str();
				std::cout << line << std::endl;
			}
		}
	}
}

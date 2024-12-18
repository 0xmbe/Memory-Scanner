#include "ProcessManipulation.h"

//#include "MemoryPointerScanner.h"

//
//class ProcessManipulation {
//public:
//	static void GetPID_TEST()
//	{
//		uintptr_t PID = 0;
//		
//	}
//};

uintptr_t ProcessManipulation::GetPID(System::Windows::Forms::Form^ mainForm)
{
	uintptr_t PID = 0;
	PID = GetProcessIDbyProcessName(mainForm);
	if (PID != 0) {
		return PID;
	}
	if (PID == 0) {		// GET PID FROM USER FORM IF TARGET FILENAME IS NOT SET ON FORM
		System::String^ PID_s = ((System::Windows::Forms::TextBox^)mainForm->Controls->Find("textBox_PID", true)[0])->Text;
		int temp;
		if (System::Int32::TryParse(PID_s, temp)) {
			PID = temp;
			return PID;
		}
		else {
			System::Windows::Forms::MessageBox::Show("Enter PID or Process Name first.");
			return 0;
		}
	}
	return 0;
}
BOOL ProcessManipulation::CheckProcess_x86_x64(HANDLE hProcess) {
	BOOL isWow64Process = FALSE;
	IsWow64Process(hProcess, &isWow64Process);
	return isWow64Process;
}
void ProcessManipulation::PrintProcessNameAndID(DWORD processID)
{
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);

	// Get the process name.
	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
		{
			GetModuleBaseName(hProcess, hMod, szProcessName,
				sizeof(szProcessName) / sizeof(TCHAR));
		}
	}

	// Print the process name and identifier.
	std::wstring wsProcessName(szProcessName);
	std::wstring wsOutput;
	std::wstringstream ss;
	ss << wsProcessName << L" (PID: " << processID << L")\n";
	wsOutput = ss.str();
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string narrowString = converter.to_bytes(wsOutput);
	std::cout << narrowString;

	//_tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);

	CloseHandle(hProcess);
}

std::wstring ProcessManipulation::GetProcessName(HANDLE hProcess) {
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

	if (hProcess != NULL) {
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
			GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
		}
	}
	return std::wstring(szProcessName);
}
std::vector<DWORD> ProcessManipulation::GetAllProcessesListVector() {
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	std::vector<DWORD> processIds;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
		return processIds;
	}

	cProcesses = cbNeeded / sizeof(DWORD);

	for (unsigned int i = 0; i < cProcesses; i++) {
		if (aProcesses[i] != 0) {
			processIds.push_back(aProcesses[i]);
			//PrintProcessNameAndID(aProcesses[i]);
		}
	}
	return processIds;
}
void ProcessManipulation::PrintThisProcessModules(HANDLE hProcess)
{
	std::vector<ModuleInfo> modulesInfo = GetAllModulesOfThisProcess(hProcess);
	for (const ModuleInfo& module : modulesInfo) {
		std::cout << "MODULE: " << std::setw(30) << std::left << Wstring_to_StdString(module.name) << " ADDR: 0x" << std::hex << std::setw(16) << std::setfill(' ') << module.address << std::endl;

	}
}
std::vector<ModuleInfo> ProcessManipulation::GetAllModulesOfThisProcess(HANDLE hProcess) {
	std::vector<ModuleInfo> modules;
	HMODULE hMods[1024];
	DWORD cbNeeded;
	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
		for (int i = 0; i < cbNeeded / sizeof(HMODULE); ++i) {
			TCHAR szModName[MAX_PATH];
			GetModuleBaseName(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR));

			ModuleInfo moduleInfo;
			moduleInfo.name = szModName;
			moduleInfo.address = (DWORD)hMods[i];
			modules.push_back(moduleInfo);
		}
	}
	return modules;
}
bool CompareProcesses(HANDLE hProcess1, uintptr_t processId2) {
	DWORD processId1 = GetProcessId(hProcess1);
	return processId1 == static_cast<DWORD>(processId2);
}

bool CompareProcessName(HANDLE hProcess, const std::wstring& processName) {
	WCHAR processNameBuffer[MAX_PATH];
	if (GetProcessImageFileNameW(hProcess, processNameBuffer, MAX_PATH)) {
		std::wstring processNameFromHandle(processNameBuffer);
		return processNameFromHandle == processName;
	}
	return false;
}
void ProcessManipulation::PrintThisProcessModules2(System::Windows::Forms::Form^ mainForm) {
	uintptr_t PID = 0;
	System::String^ ProcessName_s = ((System::Windows::Forms::TextBox^)mainForm->Controls->Find("textBox_ProcessName", true)[0])->Text;
	std::wstring targetProcessName = SystemString_to_Wstring(ProcessName_s);
	System::String^ PID_s = ((System::Windows::Forms::TextBox^)mainForm->Controls->Find("textBox_PID", true)[0])->Text;
	std::wstring pid_ws = SystemString_to_Wstring(PID_s);
	if (!pid_ws.empty()) {
		std::vector<DWORD> processList = ProcessManipulation::GetAllProcessesListVector();
		DWORD pid_dw = std::stoul(pid_ws);
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid_dw);
		if (hProcess == NULL) {
			GetLastErrorAndPrintInfo();
			return;
		}
		WCHAR targetProcessName[MAX_PATH];
		if (!GetProcessImageFileNameW(hProcess, targetProcessName, MAX_PATH)) {
			GetLastErrorAndPrintInfo();
			CloseHandle(hProcess);
			return;
		}
		CloseHandle(hProcess);
		for (int i = 0; i < processList.size(); ++i) {
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processList[i]);
			if (processList[i] == pid_dw) {
				std::wstring processName = GetProcessName(hProcess);
				std::cout << Wstring_to_StdString(processName) << std::endl;
				PID = GetProcessId(hProcess);
				std::cout << PID << std::endl;
				ProcessManipulation::PrintThisProcessModules(hProcess);
				return;
			}
			CloseHandle(hProcess);
		}
	}
	else {
		std::vector<DWORD> processList = ProcessManipulation::GetAllProcessesListVector();
		for (int i = 0; i < processList.size(); ++i) {
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processList[i]);
			std::wstring processName = GetProcessName(hProcess);
			if (processName == targetProcessName) {
				std::cout << Wstring_to_StdString(processName) << std::endl;
				PID = GetProcessId(hProcess);
				std::cout << PID << std::endl;
				ProcessManipulation::PrintThisProcessModules(hProcess);
				return;
			}
			CloseHandle(hProcess);
		}
	}
}
uintptr_t ProcessManipulation::GetProcessIDbyProcessName(System::Windows::Forms::Form^ mainForm) {
	uintptr_t PID = 0;
	System::String^ ProcessName_s = ((System::Windows::Forms::TextBox^)mainForm->Controls->Find("textBox_ProcessName", true)[0])->Text;
	std::wstring targetProcessName = SystemString_to_Wstring(ProcessName_s);
	std::vector<DWORD> processList = ProcessManipulation::GetAllProcessesListVector();
	for (int i = 0; i < processList.size(); ++i) {
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processList[i]);
		std::wstring processName = GetProcessName(hProcess);
		if (processName == targetProcessName) {
			std::cout << Wstring_to_StdString(processName) << std::endl;
			PID = GetProcessId(hProcess);
			CloseHandle(hProcess);
			std::cout << PID << std::endl;
			return PID;
		}
		CloseHandle(hProcess);
	}
	//System::Windows::Forms::MessageBox::Show("Can't find process with that PID.");
	return PID;
}


//
//void* calculatePointer(uint64_t stackTop, uint64_t integerAddress) {
//	// Assuming stack grows from high address to low address
//	// stackTop is the highest address in the stack
//	uint64_t stackBottom = stackTop - 0x778CFCB0;
//
//	// Calculate the offset of the integer within the stack
//	uint64_t offset = 0x00C3F640 - 0x00C3F808;
//
//	// Calculate the pointer to the integer
//	void* ptr = (void*)(0x00C3F808 + 0x00C3F634);
//
//	return ptr;
//}
void* calculatePointer(uint64_t stackTop, uint64_t integerAddress) {
	// Assuming stack grows from high address to low address
	// and integerAddress is within the stack range

	// Calculate the offset of the integer within the stack
	uint64_t offset = stackTop - integerAddress;

	// Calculate the pointer to the integer
	void* ptr = (void*)(integerAddress - offset);

	return ptr;
}

int TestRRUN() {
	// Example addresses
	uint64_t stackTop = 0x00C3F808; // Example stack top address
	uint64_t integerAddress = 0x00C3F634; // Example integer address within the stack

	void* integerPointer = calculatePointer(stackTop, integerAddress);

	std::cout << "Pointer to integer: " << integerPointer << std::endl;

	return 0;
}

void ProcessManipulation::GetAppropriate_LpBufferSize_BasedOnProcessArchitecture(System::Windows::Forms::Form^ mainForm, uintptr_t& lpBufferSize)
{
	uintptr_t processArchitectureType = ((System::Windows::Forms::ComboBox^)mainForm->Controls->Find("comboBox_ProcessArchitectureType", true)[0])->SelectedIndex;
	if (processArchitectureType == ProcessArchitectureType::Byte8) {
		lpBufferSize = 8;
	}
	if (processArchitectureType == ProcessArchitectureType::Byte4) {
		lpBufferSize = 4;
	}
	if (processArchitectureType == ProcessArchitectureType::Byte2) {
		lpBufferSize = 2;
	}
	if (processArchitectureType == ProcessArchitectureType::Byte1) {
		lpBufferSize = 1;
	}
	//TestRRUN();
}

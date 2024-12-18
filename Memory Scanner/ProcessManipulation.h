
#ifndef PROCESS_MANIPULATION_H
#define PROCESS_MANIPULATION_H

#include "MemoryPointerScanner.h"

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



class ProcessManipulation {
public:
	static uintptr_t GetPID(System::Windows::Forms::Form^ mainForm);
	static BOOL CheckProcess_x86_x64(HANDLE hProcess);
	static void PrintProcessNameAndID(DWORD processID);
	static std::wstring GetProcessName(HANDLE hProcess);
	static std::vector<DWORD> GetAllProcessesListVector();
	static void PrintThisProcessModules(HANDLE hProcess);
	static std::vector<ModuleInfo> GetAllModulesOfThisProcess(HANDLE hProcess);
	void PrintThisProcessModules2(System::Windows::Forms::Form^ mainForm);
	static uintptr_t GetProcessIDbyProcessName(System::Windows::Forms::Form^ mainForm);
	static void GetAppropriate_LpBufferSize_BasedOnProcessArchitecture(System::Windows::Forms::Form^ mainForm, uintptr_t& lpBufferSize);
};


#endif
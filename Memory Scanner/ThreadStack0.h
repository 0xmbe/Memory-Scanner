#pragma once
#include <vector>
#include "PointerStruct.h"

//uintptr_t lpBufferSize;
constexpr int PTR_SIZE = sizeof(StackPtrType);
constexpr int STACK_SIZE = 1024 * PTR_SIZE;
//StackPtrType _threadStack0 ;


class THREADSTACK0 {
	//StackPtrType _threadStack0 = 0;
	uint64_t Get_ThreadStack_top(HANDLE process_handle, HANDLE thread_handle, LPCWSTR moduleName);
	void Display_Threadstack_Stacktop_Buffer(HANDLE process_handle, uint64_t stacktop, MODULEINFO& moduleInfo, std::vector<uint64_t>& threadstacks);
	void PointerScannerFor_IntValue(HANDLE process_handle, DWORD thread_id, LPCWSTR kernelModuleName, LPCWSTR moduleName);
	std::vector<uint64_t> find_threadstack_for_thread(HANDLE process_handle, DWORD thread_id, LPCWSTR kernelModuleName, LPCWSTR moduleName);
	DWORD get_first_thread_id(DWORD pid);

	

public:
	void ThreadStack_Runner_PointerScannerFor_IntValue(DWORD pid, LPCWSTR kernelModuleName, LPCWSTR moduleName);
	void ThreadStack_Runner_find_threadstack_for_thread(DWORD pid, LPCWSTR kernelModuleName, LPCWSTR moduleName);
	
	static void GetAssemblyThreadstack0(System::Windows::Forms::Form^ mainForm);
};



std::stringstream Convert_UnsignedLongLongAddress_DEC_to_HEX(unsigned long long th);

void SetThreadstack0TextToMainform(unsigned long long th);

void OnlyReadPointedValues(uintptr_t searchInt, int lastI, uintptr_t* addressPointers, HANDLE hProcess, uintptr_t lpBufferSize, bool result, bool& matchFound);

void Display_Read_PointedValues(_POINTER_MAP& pointerMap, uintptr_t searchInt, int lastI, uintptr_t* addressPointers, HANDLE hProcess, uintptr_t lpBufferSize, bool result, bool& matchFound);

int CountNumberOfNonZeroValuesInArray(uintptr_t* thisArray);
std::string GetValueFromTextBoxInStdString(System::Windows::Forms::Form^ mainForm, System::String^ textBoxName);

//void SetValueToTextBoxInStdString(System::Windows::Forms::Form^ mainForm, System::String^ textBoxName, std::string value);

wchar_t* Convert_StdString_to_Wchar(std::string stdString);

void THREADSTACK0_Setup_find_threadstack_for_thread(System::Windows::Forms::Form^ mainForm);

void THREADSTACK0_Setup_PointerScannerFor_IntValue(System::Windows::Forms::Form^ mainForm);

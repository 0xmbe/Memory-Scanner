
#ifndef MEMORY_POINTER_SCANNER_H
#define MEMORY_POINTER_SCANNER_H
#pragma once
#include <wtypes.h>
#include <string>
#include <vector>
#include "PointerStruct.h"


enum DataType
{
	Pointer_to_Int,
	String,
	Array,
	Pointer
};
enum PointerLevel
{
	L1,
	L2,
	L3,
	L4,
};
enum ProcessArchitectureType
{
	Byte8,
	Byte4,
	Byte2,
	Byte1
};
struct ModuleInfo
{
	std::wstring name;
	DWORD address;
};
void test();
int Test1();
std::string Wstring_to_StdString(std::wstring wstring);
std::wstring SystemString_to_Wstring(System::String^ systemString);
void TestRun();
void DebugToOutput(const std::string& messageType, std::string messageText, System::Windows::Forms::Form^ mainForm);
std::string longPtrToHex(LONG_PTR ptr);
unsigned __int64 Convert_StdStringToUint64(const std::string& hexString);
uintptr_t Convert_StdString_to_uintptr_t(const std::string& hexString);
System::String^ Convert_StdString_to_SystemString(const std::string stdstr);
std::string Convert_SystemString_to_StdString(System::String^ ss);
void WriteToSelectedMemoryAddress(System::Windows::Forms::Form^ mainForm);
void WriteToMemoryAddress_INT(System::Windows::Forms::Form^ mainForm);
void WriteToMemoryAddress_HEX(System::Windows::Forms::Form^ mainForm);
void GetLastErrorAndPrintInfo();
StackPtrType FindPointerInStackOfThatProcessThatPointsToSpecificAddress(HANDLE hProcess, StackPtrType stackBase, StackPtrType stackSize, StackPtrType targetAddress);
StackPtrType calculateOffset(StackPtrType threadstack0, StackPtrType pointerAddress);
void FindPointerToSelectedMemoryAddress(System::Windows::Forms::Form^ mainForm);
void DisplayPointerMapLayout(_POINTER_MAP& pointerMap,uintptr_t*  addressPointers, bool& result, bool& matchFound);
void ScanSelectedMemoryAddress(System::Windows::Forms::Form^ mainForm);
void ScanPointerDepthMap(_POINTER_MAP& pointerMap, uintptr_t* addressPointers, bool& result, HANDLE hProcess, uintptr_t& address, uintptr_t lpBufferSize, int& lastI, bool& matchFound);
void DisplayScanPointerDepthMap(_POINTER_MAP& pointerMap,uintptr_t*  addressPointers, bool& result, HANDLE hProcess, uintptr_t& address, uintptr_t lpBufferSize, int& lastI, bool& matchFound);






#endif
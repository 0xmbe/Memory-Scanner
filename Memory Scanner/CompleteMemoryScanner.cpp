/*
A memory scanner for Wesnoth that allows a user to search, filter, and edit memory inside the process. This code can be adapted to
any target and is intended to show how tools like CheatEngine work.

The scanner has three main operations:
- search
- filter
- write

The search operation will scan all memory from 0x00000000 to 0x7FFFFFFF and use ReadProcessMemory to determine if the address holds
a certain value. Due to ReadProcessMemory failing if a process doesn't have access to an address, the memory is scanned in blocks.
Any values that match are saved to res.txt

The filter operation iterates over all addresses in res.txt to determine if they match the provided value. If so, they are saved
to res_fil.txt. At the end, res_fil.txt is copied over to res.txt.

The write operation uses WriteProcessMemory to write a passed value to all address in res.txt

CreateToolhelp32Snapshot is used to find the Wesnoth process and OpenProcess is used to retrieve a handle.

The full explanation for how this code works is available at: https://gamehacking.academy/lesson/35
*/
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <stdio.h>

#include <vector>
#include <Psapi.h>
#include <cwchar>
#include "CompleteMemoryScanner.h"
#include "ThreadStack0.h"
#include "MemoryPointerScanner.h"
//#include "ThreadStack0.cpp"


//#include <cwchar>
//#include <cstdlib>

#define size 0x00000808

// The search function scans all memory from 0x00000000 to 0x7FFFFFFF for the passed value
void search(const HANDLE process, unsigned long long passed_val) {
	FILE* temp_file = NULL;
	fopen_s(&temp_file, "res.txt", "w");

	unsigned char* buffer = (unsigned char*)calloc(1, size);
	DWORD bytes_read = 0;

	for (DWORD i = 0x00000000; i < 0x7FFFFFFF; i += size) {
		ReadProcessMemory(process, (void*)i, buffer, size, &bytes_read);

		for (int j = 0; j < size - 4; j += 4) {
			DWORD val = 0;
			memcpy(&val, &buffer[j], 4);
			if (val == passed_val) {
				fprintf(temp_file, "%x\n", i + j);
				auto address = i + j;
				std::cout << "<>READ: " << std::dec << (unsigned long long)val << "\t\tADDR: 0x" << std::hex << std::uppercase << address << std::endl;
			}
		}
	}

	fclose(temp_file);

	free(buffer);
}

// The filter function takes a list of addresses in res.txt and checks to see
// if they match the provided value. If so, they are written to res_fil.txt
// After the initial pass, filter writes all the addresses in res_fil.txt to res.txt
void filter(const HANDLE process, unsigned long long passed_val) {
	FILE* temp_file = NULL;
	FILE* temp_file_filter = NULL;
	fopen_s(&temp_file, "res.txt", "r");
	fopen_s(&temp_file_filter, "res_fil.txt", "w");

	DWORD address = 0;
	while (fscanf_s(temp_file, "%x\n", &address) != EOF) {
		DWORD val = 0;
		DWORD bytes_read = 0;

		ReadProcessMemory(process, (void*)address, &val, 4, &bytes_read);
		if (val == passed_val) {
			fprintf(temp_file_filter, "%x\n", address);
		}
	}

	fclose(temp_file);
	fclose(temp_file_filter);

	fopen_s(&temp_file, "res.txt", "w");
	fopen_s(&temp_file_filter, "res_fil.txt", "r");
	while (fscanf_s(temp_file_filter, "%x\n", &address) != EOF) {
		fprintf(temp_file, "%x\n", address);
	}

	fclose(temp_file);
	fclose(temp_file_filter);

	remove("res_fil.txt");
}

// The write function writes a value to every address in res.txt
void write(const HANDLE process, unsigned long long passed_val) {
	FILE* temp_file = NULL;
	fopen_s(&temp_file, "res.txt", "r");

	DWORD address = 0;
	while (fscanf_s(temp_file, "%x\n", &address) != EOF) {
		DWORD bytes_written = 0;

		WriteProcessMemory(process, (void*)address, &passed_val, 4, &bytes_written);
	}

	fclose(temp_file);
}
bool compareStrings(const wchar_t* wideStr, const char* multiByteStr) {
	size_t length = std::mbstowcs(nullptr, multiByteStr, 0) + 1;// Convert char* to wchar_t* using mbstowcs_s
	wchar_t* convertedStr = new wchar_t[length];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, convertedStr, length, multiByteStr, _TRUNCATE);
	bool result = wcscmp(wideStr, convertedStr) == 0;// Compare the wide character strings	
	delete[] convertedStr;// Clean up
	return result;
}
wchar_t* Convert_CharArray_to_WideChar(const char* narrowString) {
	size_t requiredSize = mbstowcs(nullptr, narrowString, 0);
	if (requiredSize == 0) {
		return nullptr;
	}
	wchar_t* wideString = new wchar_t[requiredSize + 1];
	mbstowcs(wideString, narrowString, requiredSize);
	return wideString;
}
// The main function is retrieving a process handle to Wesnoth, parsing the program's arguments and passing 
// execution to the proper operation
//int main(int argc, char** argv) {
//int main(int argc, char* argv[]) {
//int CompleteMemoryScanner_Scan(int argc, const char* argv[]) {			// THOSE UP ARE TO RUN FROM OUTSIDE WITH CMD
//int CompleteMemoryScanner_Scan(int argc, char* argv[]) {
int CompleteMemoryScanner_Scan(WCHAR* targetProcessName, const char* command, unsigned long long value) {
	/*if (argc < 4) {
		std::cout << "Usage: MemoryScannerTest.exe <targetProcessName.exe> <command> <searchInteger>" << std::endl;
		std::cout << "Available commands: <search>, <filter>, <write>" << std::endl;
		return 0;
	}*/
	HANDLE process_snapshot = 0;
	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// The snapshot code is a reduced version of the example code provided by Microsoft at 
	// https://docs.microsoft.com/en-us/windows/win32/toolhelp/taking-a-snapshot-and-viewing-processes
	process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	Process32First(process_snapshot, &pe32);

	std::cout << targetProcessName << std::endl;

	do {
		if (wcscmp(pe32.szExeFile, targetProcessName) == 0) {
			// Retrieve a process handle so that we can read and write the game's memory
			HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, true, pe32.th32ProcessID);

			// Depending on the first argument, pass execution to the search, filter, or write operations
			if (strcmp(command, "search") == 0) {
				std::cout << "search\n";
				search(process, value);
			}
			else if (strcmp(command, "filter") == 0) {
				std::cout << "filter\n";
				filter(process, value);
			}
			else if (strcmp(command, "write") == 0) {
				std::cout << "write\n";
				write(process, value);
			}
			std::cout << "end" << std::endl;
			// Close the process handle
			CloseHandle(process);
			break;
		}
	} while (Process32Next(process_snapshot, &pe32));

	return 0;
}
wchar_t* convert_wstring_to_wchar_t_array(std::wstring& str) {
	size_t len = str.length() + 1; // +1 for null terminator
	wchar_t* result = new wchar_t[len];
	wcscpy_s(result, len, str.c_str());
	return result;
}
void CompleteMemoryScanner_Run(System::Windows::Forms::Form^ mainForm) {
	System::String^ ProcessName_s = ((System::Windows::Forms::TextBox^)mainForm->Controls->Find("textBox_ProcessName", true)[0])->Text;
	std::wstring targetProcessName_w = SystemString_to_Wstring(ProcessName_s);
	wchar_t* targetProcessName = convert_wstring_to_wchar_t_array(targetProcessName_w);

	std::string searchInt_stds = GetValueFromTextBoxInStdString(mainForm, "textBox_ValueToReadINT");
	unsigned long long searchInt_ulong = std::stoll(searchInt_stds);

	const char* command = "search";

	CompleteMemoryScanner_Scan(targetProcessName, command, searchInt_ulong);
}




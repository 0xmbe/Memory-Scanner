#pragma once

#ifndef INTERFACE_H
#define INTERFACE_H

#include "ProcessManipulation.h"
#include "MemoryPointerScanner.h"
#include "ThreadStack0.h"
#include "CompleteMemoryScanner.h"

#include <map>
#include <msclr/marshal.h>
#include <string>
#include <array>
#include <functional>
#include <iostream>


//using namespace System;
//using namespace System::Windows::Forms;

/// <summary>
/// DISPATCHER FOR INTERFACE
/// THIS GETS THE CALLER BY BUTTON TEXT
/// SET WHAT EACH BUTTON DOES HERE
/// Include .h librarys with managed only here.
/// </summary>
/// <param name="wstring"></param>
/// <param name="mainForm"></param>
void MethodDispatcher(std::wstring wstring, System::Windows::Forms::Form^ mainForm) {
	if (wstring == L"List All Modules") {
		ProcessManipulation pm;
		pm.PrintThisProcessModules2(mainForm);
		return;
	}
	else if (wstring == L"THREADSTACK0") {
		THREADSTACK0_Setup_find_threadstack_for_thread(mainForm);
		return;
	}
	else if (wstring == L"Scan Memory Address") {
		ScanSelectedMemoryAddress(mainForm);
		return;
	}
	else if (wstring == L"Find Pointer To Memory Address") {
		FindPointerToSelectedMemoryAddress(mainForm);
		return;
	}
	else if (wstring == L"Find Pointer To Int") {
		THREADSTACK0_Setup_PointerScannerFor_IntValue(mainForm);
		return;
	}
	else if (wstring == L"Complete Memory Scanner For Int") {
		CompleteMemoryScanner_Run(mainForm);
		return;
	}
	else {
	MessageBox(NULL, L"Function in Method Dispatcher not found!", L"ERROR", MB_OK);
	}
}
void mainINTERFACE_string(System::String^ arg) {

}
void TestMethod(int x) {
	int a = 5;
}
void TestMethod2() {
	int a = 5;
}
//--------------------------------------------------------------------------------------
void mainINTERFACE_object_to_buttonText(System::Object^ object, System::Windows::Forms::Form^ mainForm, System::String^ methodString) {
	System::String^ buttonText = ((System::Windows::Forms::ButtonBase^)object)->Text;
	std::wstring wstring = SystemString_to_Wstring(buttonText);
	
	MethodDispatcher(wstring, mainForm);
}



#endif
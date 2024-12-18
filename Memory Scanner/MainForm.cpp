#include "HackingProgramMainForm.h"
#include "MemoryPointerScanner.h"

//using namespace HackingProgram;

namespace HackingProgram {
	using namespace System;
	using namespace System::Windows::Forms;


	/// STARTUP main
	/// IF SOMETHING CHANGES IT, RETURN TO:
	/// 	int main_form() { ...
	/// GO: PROJECT PROPERTIES -> LINKER -> ADVANCED -> ENTRY POINT
	/// SET: main_form
	/// GO: PROJECT PROPERTIES -> LINKER -> SYSTEM -> SUB SYSTEM
	/// SET: CONSOLE

	[STAThread]
		int main_form()		
	{


		Application::EnableVisualStyles();
		Application::SetCompatibleTextRenderingDefault(false);
		HackingProgram::MainForm mainForm;

		Application::Run(% mainForm);

		return 0;
	}

	void RunOnInit() {

	}

	/*extern "C" __declspec(dllexport) void MyFunction(const char* text) {
		std::cout << text << std::endl;
	}*/
}
#pragma once

#include "_INTERFACE.h"

#include "MemoryPointerScanner.h"


#include "ThreadStack0_old.h"			// can delete later
#include "ThreadStack0.h"


#include "DrawOverlay.h"
#include "7Zip.h"				// TEMP TEST


namespace HackingProgram {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace System::Windows;
	using namespace System::Windows::Forms;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm()
		{
			SetProcessDPIAware();			// -> SET ONLY THIS TO GET SHARP TEXT ON FORM

			InitializeComponent();


			/// CUSTOM INITS:
			//comboBox_DataType->SelectedIndex = 0;
			//comboBox_PointerLevel->SelectedIndex = 0;
			comboBox_ProcessArchitectureType->SelectedIndex = 1;

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^ textBox_PID;
	protected:

	private: System::Windows::Forms::GroupBox^ groupBox1;



	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::GroupBox^ groupBox3;
	private: System::Windows::Forms::TextBox^ textBox_MemoryAddress;
	private: System::Windows::Forms::RichTextBox^ richTextBox_Output;


	private: System::Windows::Forms::GroupBox^ groupBox4;
	private: System::Windows::Forms::GroupBox^ groupBox5;


	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::GroupBox^ groupBox7;
	private: System::Windows::Forms::TextBox^ textBox_ValueToWriteINT;
	private: System::Windows::Forms::GroupBox^ groupBox8;
	private: System::Windows::Forms::TextBox^ textBox_ProcessName;
	private: System::Windows::Forms::GroupBox^ groupBox2;
	private: System::Windows::Forms::TextBox^ textBox_ValueToWriteHEX;
	private: System::Windows::Forms::GroupBox^ groupBox6;
	private: System::Windows::Forms::RadioButton^ radioButton_ValueToWriteHEX;
	private: System::Windows::Forms::RadioButton^ radioButton_ValueToWriteINT;
	private: System::Windows::Forms::ComboBox^ comboBox_ProcessArchitectureType;

	private: System::Windows::Forms::GroupBox^ groupBox9;
	private: System::Windows::Forms::GroupBox^ groupBox10;
	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::Button^ button4;

	private: System::Windows::Forms::GroupBox^ groupBox11;
	private: System::Windows::Forms::TextBox^ textBox_AssemblyName;
	private: System::Windows::Forms::GroupBox^ groupBox12;
	private: System::Windows::Forms::TextBox^ textBox_KernelAssemblyName;
	private: System::Windows::Forms::Button^ button5;
	private: System::Windows::Forms::Button^ button6;
	private: System::Windows::Forms::Button^ button7;
	private: System::Windows::Forms::GroupBox^ groupBox13;
	private: System::Windows::Forms::TextBox^ textBox_ThreadStack;
	private: System::Windows::Forms::Button^ button8;
	private: System::Windows::Forms::GroupBox^ groupBox14;
	private: System::Windows::Forms::TextBox^ textBox_ValueToReadINT;

	private: System::Windows::Forms::Button^ button10;






	protected:

	protected:


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBox_PID = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->textBox_MemoryAddress = (gcnew System::Windows::Forms::TextBox());
			this->richTextBox_Output = (gcnew System::Windows::Forms::RichTextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->groupBox7 = (gcnew System::Windows::Forms::GroupBox());
			this->textBox_ValueToWriteINT = (gcnew System::Windows::Forms::TextBox());
			this->groupBox8 = (gcnew System::Windows::Forms::GroupBox());
			this->textBox_ProcessName = (gcnew System::Windows::Forms::TextBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->textBox_ValueToWriteHEX = (gcnew System::Windows::Forms::TextBox());
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->radioButton_ValueToWriteHEX = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton_ValueToWriteINT = (gcnew System::Windows::Forms::RadioButton());
			this->comboBox_ProcessArchitectureType = (gcnew System::Windows::Forms::ComboBox());
			this->groupBox9 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox10 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox13 = (gcnew System::Windows::Forms::GroupBox());
			this->textBox_ThreadStack = (gcnew System::Windows::Forms::TextBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->groupBox12 = (gcnew System::Windows::Forms::GroupBox());
			this->textBox_KernelAssemblyName = (gcnew System::Windows::Forms::TextBox());
			this->groupBox11 = (gcnew System::Windows::Forms::GroupBox());
			this->textBox_AssemblyName = (gcnew System::Windows::Forms::TextBox());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->groupBox14 = (gcnew System::Windows::Forms::GroupBox());
			this->textBox_ValueToReadINT = (gcnew System::Windows::Forms::TextBox());
			this->button10 = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->groupBox7->SuspendLayout();
			this->groupBox8->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox6->SuspendLayout();
			this->groupBox9->SuspendLayout();
			this->groupBox10->SuspendLayout();
			this->groupBox13->SuspendLayout();
			this->groupBox12->SuspendLayout();
			this->groupBox11->SuspendLayout();
			this->groupBox14->SuspendLayout();
			this->SuspendLayout();
			// 
			// textBox_PID
			// 
			this->textBox_PID->Dock = System::Windows::Forms::DockStyle::Fill;
			this->textBox_PID->Location = System::Drawing::Point(3, 18);
			this->textBox_PID->Name = L"textBox_PID";
			this->textBox_PID->Size = System::Drawing::Size(125, 22);
			this->textBox_PID->TabIndex = 0;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->textBox_PID);
			this->groupBox1->Location = System::Drawing::Point(12, 12);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(131, 57);
			this->groupBox1->TabIndex = 1;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"PID";
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->textBox_MemoryAddress);
			this->groupBox3->Location = System::Drawing::Point(12, 75);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(239, 50);
			this->groupBox3->TabIndex = 3;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Memory Address (hex) Value";
			// 
			// textBox_MemoryAddress
			// 
			this->textBox_MemoryAddress->Dock = System::Windows::Forms::DockStyle::Fill;
			this->textBox_MemoryAddress->Location = System::Drawing::Point(3, 18);
			this->textBox_MemoryAddress->Name = L"textBox_MemoryAddress";
			this->textBox_MemoryAddress->Size = System::Drawing::Size(233, 22);
			this->textBox_MemoryAddress->TabIndex = 0;
			this->textBox_MemoryAddress->Text = L"0x00C3FD60";
			// 
			// richTextBox_Output
			// 
			this->richTextBox_Output->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->richTextBox_Output->Dock = System::Windows::Forms::DockStyle::Fill;
			this->richTextBox_Output->Font = (gcnew System::Drawing::Font(L"Consolas", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->richTextBox_Output->ForeColor = System::Drawing::SystemColors::InactiveBorder;
			this->richTextBox_Output->Location = System::Drawing::Point(3, 18);
			this->richTextBox_Output->Name = L"richTextBox_Output";
			this->richTextBox_Output->Size = System::Drawing::Size(813, 348);
			this->richTextBox_Output->TabIndex = 4;
			this->richTextBox_Output->Text = L"";
			this->richTextBox_Output->TextChanged += gcnew System::EventHandler(this, &MainForm::richTextBox_Output_TextChanged);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(18, 187);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(225, 30);
			this->button1->TabIndex = 5;
			this->button1->Text = L"Scan Memory Address";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MainForm::button1_Click);
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->richTextBox_Output);
			this->groupBox4->Location = System::Drawing::Point(12, 587);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(819, 369);
			this->groupBox4->TabIndex = 2;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Output";
			// 
			// groupBox5
			// 
			this->groupBox5->Location = System::Drawing::Point(94, 576);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(239, 57);
			this->groupBox5->TabIndex = 2;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L"Main Tools";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(6, 133);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(225, 30);
			this->button2->TabIndex = 6;
			this->button2->Text = L"Write To Memory Address";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MainForm::button2_Click);
			// 
			// groupBox7
			// 
			this->groupBox7->Controls->Add(this->textBox_ValueToWriteINT);
			this->groupBox7->Location = System::Drawing::Point(24, 21);
			this->groupBox7->Name = L"groupBox7";
			this->groupBox7->Size = System::Drawing::Size(207, 50);
			this->groupBox7->TabIndex = 4;
			this->groupBox7->TabStop = false;
			this->groupBox7->Text = L"Int";
			// 
			// textBox_ValueToWriteINT
			// 
			this->textBox_ValueToWriteINT->Dock = System::Windows::Forms::DockStyle::Fill;
			this->textBox_ValueToWriteINT->Location = System::Drawing::Point(3, 18);
			this->textBox_ValueToWriteINT->Name = L"textBox_ValueToWriteINT";
			this->textBox_ValueToWriteINT->Size = System::Drawing::Size(201, 22);
			this->textBox_ValueToWriteINT->TabIndex = 0;
			this->textBox_ValueToWriteINT->Text = L"1111";
			// 
			// groupBox8
			// 
			this->groupBox8->Controls->Add(this->textBox_ProcessName);
			this->groupBox8->Location = System::Drawing::Point(257, 12);
			this->groupBox8->Name = L"groupBox8";
			this->groupBox8->Size = System::Drawing::Size(239, 57);
			this->groupBox8->TabIndex = 2;
			this->groupBox8->TabStop = false;
			this->groupBox8->Text = L"Process Name";
			// 
			// textBox_ProcessName
			// 
			this->textBox_ProcessName->Dock = System::Windows::Forms::DockStyle::Fill;
			this->textBox_ProcessName->Location = System::Drawing::Point(3, 18);
			this->textBox_ProcessName->Name = L"textBox_ProcessName";
			this->textBox_ProcessName->Size = System::Drawing::Size(233, 22);
			this->textBox_ProcessName->TabIndex = 0;
			this->textBox_ProcessName->Text = L"TargetProgram.exe";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->textBox_ValueToWriteHEX);
			this->groupBox2->Location = System::Drawing::Point(24, 77);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(207, 50);
			this->groupBox2->TabIndex = 5;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Hex";
			// 
			// textBox_ValueToWriteHEX
			// 
			this->textBox_ValueToWriteHEX->Dock = System::Windows::Forms::DockStyle::Fill;
			this->textBox_ValueToWriteHEX->Location = System::Drawing::Point(3, 18);
			this->textBox_ValueToWriteHEX->Name = L"textBox_ValueToWriteHEX";
			this->textBox_ValueToWriteHEX->Size = System::Drawing::Size(201, 22);
			this->textBox_ValueToWriteHEX->TabIndex = 0;
			this->textBox_ValueToWriteHEX->Text = L"1E240";
			// 
			// groupBox6
			// 
			this->groupBox6->Controls->Add(this->button2);
			this->groupBox6->Controls->Add(this->radioButton_ValueToWriteHEX);
			this->groupBox6->Controls->Add(this->radioButton_ValueToWriteINT);
			this->groupBox6->Controls->Add(this->groupBox7);
			this->groupBox6->Controls->Add(this->groupBox2);
			this->groupBox6->Location = System::Drawing::Point(257, 75);
			this->groupBox6->Name = L"groupBox6";
			this->groupBox6->Size = System::Drawing::Size(239, 180);
			this->groupBox6->TabIndex = 5;
			this->groupBox6->TabStop = false;
			this->groupBox6->Text = L"Memory Writer";
			// 
			// radioButton_ValueToWriteHEX
			// 
			this->radioButton_ValueToWriteHEX->AutoSize = true;
			this->radioButton_ValueToWriteHEX->Location = System::Drawing::Point(6, 101);
			this->radioButton_ValueToWriteHEX->Name = L"radioButton_ValueToWriteHEX";
			this->radioButton_ValueToWriteHEX->Size = System::Drawing::Size(17, 16);
			this->radioButton_ValueToWriteHEX->TabIndex = 7;
			this->radioButton_ValueToWriteHEX->TabStop = true;
			this->radioButton_ValueToWriteHEX->UseVisualStyleBackColor = true;
			// 
			// radioButton_ValueToWriteINT
			// 
			this->radioButton_ValueToWriteINT->AutoSize = true;
			this->radioButton_ValueToWriteINT->Checked = true;
			this->radioButton_ValueToWriteINT->Location = System::Drawing::Point(6, 45);
			this->radioButton_ValueToWriteINT->Name = L"radioButton_ValueToWriteINT";
			this->radioButton_ValueToWriteINT->Size = System::Drawing::Size(17, 16);
			this->radioButton_ValueToWriteINT->TabIndex = 6;
			this->radioButton_ValueToWriteINT->TabStop = true;
			this->radioButton_ValueToWriteINT->UseVisualStyleBackColor = true;
			this->radioButton_ValueToWriteINT->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButton_ValueToWriteINT_CheckedChanged);
			this->radioButton_ValueToWriteINT->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainForm::radioButton_ValueToWriteINT_Paint);
			// 
			// comboBox_ProcessArchitectureType
			// 
			this->comboBox_ProcessArchitectureType->Dock = System::Windows::Forms::DockStyle::Fill;
			this->comboBox_ProcessArchitectureType->FormattingEnabled = true;
			this->comboBox_ProcessArchitectureType->Items->AddRange(gcnew cli::array< System::Object^  >(4) {
				L"8 Byte", L"4 Byte", L"2 Byte",
					L"1 Byte"
			});
			this->comboBox_ProcessArchitectureType->Location = System::Drawing::Point(3, 18);
			this->comboBox_ProcessArchitectureType->Name = L"comboBox_ProcessArchitectureType";
			this->comboBox_ProcessArchitectureType->Size = System::Drawing::Size(96, 24);
			this->comboBox_ProcessArchitectureType->TabIndex = 6;
			// 
			// groupBox9
			// 
			this->groupBox9->Controls->Add(this->comboBox_ProcessArchitectureType);
			this->groupBox9->Location = System::Drawing::Point(149, 12);
			this->groupBox9->Name = L"groupBox9";
			this->groupBox9->Size = System::Drawing::Size(102, 57);
			this->groupBox9->TabIndex = 6;
			this->groupBox9->TabStop = false;
			this->groupBox9->Text = L"Process type";
			// 
			// groupBox10
			// 
			this->groupBox10->Controls->Add(this->groupBox13);
			this->groupBox10->Controls->Add(this->button3);
			this->groupBox10->Controls->Add(this->groupBox12);
			this->groupBox10->Controls->Add(this->groupBox11);
			this->groupBox10->Location = System::Drawing::Point(12, 346);
			this->groupBox10->Name = L"groupBox10";
			this->groupBox10->Size = System::Drawing::Size(484, 153);
			this->groupBox10->TabIndex = 7;
			this->groupBox10->TabStop = false;
			this->groupBox10->Text = L"Threadstack";
			// 
			// groupBox13
			// 
			this->groupBox13->Controls->Add(this->textBox_ThreadStack);
			this->groupBox13->Location = System::Drawing::Point(6, 67);
			this->groupBox13->Name = L"groupBox13";
			this->groupBox13->Size = System::Drawing::Size(230, 57);
			this->groupBox13->TabIndex = 3;
			this->groupBox13->TabStop = false;
			this->groupBox13->Text = L"Threadstack0 Address";
			// 
			// textBox_ThreadStack
			// 
			this->textBox_ThreadStack->Dock = System::Windows::Forms::DockStyle::Fill;
			this->textBox_ThreadStack->Location = System::Drawing::Point(3, 18);
			this->textBox_ThreadStack->Name = L"textBox_ThreadStack";
			this->textBox_ThreadStack->Size = System::Drawing::Size(224, 22);
			this->textBox_ThreadStack->TabIndex = 0;
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(6, 31);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(225, 30);
			this->button3->TabIndex = 7;
			this->button3->Text = L"THREADSTACK0";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MainForm::button3_Click);
			// 
			// groupBox12
			// 
			this->groupBox12->Controls->Add(this->textBox_KernelAssemblyName);
			this->groupBox12->Location = System::Drawing::Point(245, 21);
			this->groupBox12->Name = L"groupBox12";
			this->groupBox12->Size = System::Drawing::Size(236, 50);
			this->groupBox12->TabIndex = 3;
			this->groupBox12->TabStop = false;
			this->groupBox12->Text = L"Kernel Assembly Name";
			// 
			// textBox_KernelAssemblyName
			// 
			this->textBox_KernelAssemblyName->Dock = System::Windows::Forms::DockStyle::Fill;
			this->textBox_KernelAssemblyName->Location = System::Drawing::Point(3, 18);
			this->textBox_KernelAssemblyName->Name = L"textBox_KernelAssemblyName";
			this->textBox_KernelAssemblyName->Size = System::Drawing::Size(230, 22);
			this->textBox_KernelAssemblyName->TabIndex = 0;
			this->textBox_KernelAssemblyName->Text = L"kernel32.dll";
			// 
			// groupBox11
			// 
			this->groupBox11->Controls->Add(this->textBox_AssemblyName);
			this->groupBox11->Location = System::Drawing::Point(245, 84);
			this->groupBox11->Name = L"groupBox11";
			this->groupBox11->Size = System::Drawing::Size(230, 50);
			this->groupBox11->TabIndex = 2;
			this->groupBox11->TabStop = false;
			this->groupBox11->Text = L"Assembly Name";
			// 
			// textBox_AssemblyName
			// 
			this->textBox_AssemblyName->Dock = System::Windows::Forms::DockStyle::Fill;
			this->textBox_AssemblyName->Location = System::Drawing::Point(3, 18);
			this->textBox_AssemblyName->Name = L"textBox_AssemblyName";
			this->textBox_AssemblyName->Size = System::Drawing::Size(224, 22);
			this->textBox_AssemblyName->TabIndex = 0;
			this->textBox_AssemblyName->Text = L"ntdll.dll";
			// 
			// button8
			// 
			this->button8->Location = System::Drawing::Point(271, 297);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(225, 30);
			this->button8->TabIndex = 11;
			this->button8->Text = L"Find Pointer To Int";
			this->button8->UseVisualStyleBackColor = true;
			this->button8->Click += gcnew System::EventHandler(this, &MainForm::button8_Click);
			// 
			// button7
			// 
			this->button7->Location = System::Drawing::Point(271, 261);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(225, 30);
			this->button7->TabIndex = 8;
			this->button7->Text = L"Find Pointer To Memory Address";
			this->button7->UseVisualStyleBackColor = true;
			this->button7->Click += gcnew System::EventHandler(this, &MainForm::button7_Click);
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(257, 541);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(225, 30);
			this->button6->TabIndex = 10;
			this->button6->Text = L"Delete Overlay";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &MainForm::button6_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(257, 505);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(225, 30);
			this->button4->TabIndex = 8;
			this->button4->Text = L"List All Modules";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MainForm::button4_Click);
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(12, 541);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(225, 30);
			this->button5->TabIndex = 9;
			this->button5->Text = L"Show Overlay";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &MainForm::button5_Click_1);
			// 
			// groupBox14
			// 
			this->groupBox14->Controls->Add(this->textBox_ValueToReadINT);
			this->groupBox14->Location = System::Drawing::Point(12, 131);
			this->groupBox14->Name = L"groupBox14";
			this->groupBox14->Size = System::Drawing::Size(239, 50);
			this->groupBox14->TabIndex = 4;
			this->groupBox14->TabStop = false;
			this->groupBox14->Text = L"Int (dec) Value at Memory Address";
			// 
			// textBox_ValueToReadINT
			// 
			this->textBox_ValueToReadINT->Dock = System::Windows::Forms::DockStyle::Fill;
			this->textBox_ValueToReadINT->Location = System::Drawing::Point(3, 18);
			this->textBox_ValueToReadINT->Name = L"textBox_ValueToReadINT";
			this->textBox_ValueToReadINT->Size = System::Drawing::Size(233, 22);
			this->textBox_ValueToReadINT->TabIndex = 0;
			this->textBox_ValueToReadINT->Text = L"10050";
			// 
			// button10
			// 
			this->button10->Location = System::Drawing::Point(18, 223);
			this->button10->Name = L"button10";
			this->button10->Size = System::Drawing::Size(225, 48);
			this->button10->TabIndex = 13;
			this->button10->Text = L"Complete Memory Scanner For Int";
			this->button10->UseVisualStyleBackColor = true;
			this->button10->Click += gcnew System::EventHandler(this, &MainForm::button10_Click);
			// 
			// MainForm
			// 
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
			this->ClientSize = System::Drawing::Size(503, 576);
			this->Controls->Add(this->button10);
			this->Controls->Add(this->groupBox14);
			this->Controls->Add(this->button8);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->groupBox10);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->button7);
			this->Controls->Add(this->groupBox9);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox6);
			this->Controls->Add(this->groupBox8);
			this->Controls->Add(this->groupBox5);
			this->Controls->Add(this->groupBox4);
			this->Controls->Add(this->groupBox1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"MainForm";
			this->Text = L"MemoryPointerScanner";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox7->ResumeLayout(false);
			this->groupBox7->PerformLayout();
			this->groupBox8->ResumeLayout(false);
			this->groupBox8->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox6->ResumeLayout(false);
			this->groupBox6->PerformLayout();
			this->groupBox9->ResumeLayout(false);
			this->groupBox10->ResumeLayout(false);
			this->groupBox13->ResumeLayout(false);
			this->groupBox13->PerformLayout();
			this->groupBox12->ResumeLayout(false);
			this->groupBox12->PerformLayout();
			this->groupBox11->ResumeLayout(false);
			this->groupBox11->PerformLayout();
			this->groupBox14->ResumeLayout(false);
			this->groupBox14->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e)
	{
		//ScanSelectedMemoryAddress(this);
		mainINTERFACE_object_to_buttonText(sender, this,"ScanSelectedMemoryAddress");
	}
	private: System::Void richTextBox_Output_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		this->richTextBox_Output->ScrollToCaret();
	}
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e)
	{
		WriteToSelectedMemoryAddress(this);
	}
	private: System::Void radioButton_ValueToWriteINT_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		// DELETE
	}
	private: System::Void radioButton_ValueToWriteINT_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
		if (radioButton_ValueToWriteINT->Checked == true) {
			textBox_ValueToWriteINT->Enabled = true;
			textBox_ValueToWriteHEX->Enabled = false;
		}
		if (radioButton_ValueToWriteHEX->Checked == true) {
			textBox_ValueToWriteINT->Enabled = false;
			textBox_ValueToWriteHEX->Enabled = true;
		}
	}

	private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
		//THREADSTACK0::THREADSTACK0_Run1(this);
		mainINTERFACE_object_to_buttonText(sender, this,"");
	}
	private: System::Void button4_Click(System::Object^ sender, System::EventArgs^ e) {
		mainINTERFACE_object_to_buttonText(sender, this,"");
	}
	private: System::Void button5_Click(System::Object^ sender, System::EventArgs^ e) {
		mainINTERFACE_object_to_buttonText(sender, this,"");
	}
	private: System::Void button5_Click_1(System::Object^ sender, System::EventArgs^ e) {
		ShowOverlay();
	}
	private: System::Void button6_Click(System::Object^ sender, System::EventArgs^ e) {
		DeleteOverlay();
	}
	private: System::Void button7_Click(System::Object^ sender, System::EventArgs^ e) {
		mainINTERFACE_object_to_buttonText(sender, this,"");
	}
	private: System::Void button8_Click(System::Object^ sender, System::EventArgs^ e) {
		mainINTERFACE_object_to_buttonText(sender, this,"");
	}
	private: System::Void button10_Click(System::Object^ sender, System::EventArgs^ e) {
		mainINTERFACE_object_to_buttonText(sender, this,"");
	}
	};
}

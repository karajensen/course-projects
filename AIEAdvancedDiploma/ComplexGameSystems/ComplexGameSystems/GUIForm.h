////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GUIForm.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vcclr.h>
#include "GUITypes.h"

namespace GUI
{
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /**
    * Main GUI Window Form, usable with the designer 
    */
    public ref class GUIForm : public System::Windows::Forms::Form
    {
    public:

        /**
        * Constructor 
        */
        GUIForm() :
            m_pinnedSimForm(nullptr)
        {
            InitializeComponent();
            CreateSimulationForm();

            m_pauseButton->BackgroundImage = Image::FromFile(".//Resources//pause.png");
            m_saveButton->BackgroundImage = Image::FromFile(".//Resources//save.png");
            m_borderButton->BackgroundImage = Image::FromFile(".//Resources//border.png");
        }

        /**
        * Determines and returns the window instance of the gui
        */
        HINSTANCE GetWindowInstance()
        {
            Form^ simForm = (Form^)*m_pinnedSimForm;
            IntPtr instance = System::Runtime::InteropServices::Marshal::GetHINSTANCE(
                System::Reflection::Assembly::GetExecutingAssembly()->GetModules()[0]);
            return static_cast<HINSTANCE>(instance.ToPointer());
        }

        /**
        * Determines and returns the window handle of the gui
        */
        HWND GetWindowHandle()
        {
            Form^ simForm = (Form^)*m_pinnedSimForm;
            IntPtr handle = simForm->Handle;
            return static_cast<HWND>(handle.ToPointer());
        }

        /**
        * @return the size of the window
        */
        System::Drawing::Size GetWindowSize()
        {
            return ClientSize;
        }

        /**
        * Initializes the GUI
        */
        void Initialize(GuiRequestCallbacks* requestCallbacks)
        {
            m_requestCallbacks = requestCallbacks;
        }

    protected:

        /**
        * Finaliser: Clean up any resources being used 
        */
        !GUIForm()
        {
            #ifdef _DEBUG
            OutputDebugString(L"GUIForm::!GUIForm\n");
            #endif

            if (components)
            {
                delete components;
                components = nullptr;
            }
            if (m_pinnedSimForm)
            {
                //free gcroot handle
                gcroot<Form^>* ptr = reinterpret_cast<gcroot<Form^>*>(m_pinnedSimForm);
                m_pinnedSimForm = nullptr;
                delete ptr;
            }
        }

        /**
        * Destructor: Will automatically suppress finalisation if called
        */
        ~GUIForm()
        {
            this->!GUIForm(); 
        }

        /**
        * Overrides the form message handler. This prevents the
        * window from redrawing when the alt key is pressed
        * @param m The windows message to handle
        */
        virtual void WndProc(System::Windows::Forms::Message% m) override
        {
            if (m.Msg != WM_UPDATEUISTATE)
            {
                System::Windows::Forms::Form::WndProc(m);
            }
        }

    private:

        /**
        * Creates the main simulation form
        */
        void CreateSimulationForm()
        {
            Form^ simulatorForm = gcnew Form();
            simulatorForm->TopLevel = false;
            simulatorForm->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            simulatorForm->Dock = System::Windows::Forms::DockStyle::Fill;
            simulatorForm->Visible = true;
            simulatorForm->Size = System::Drawing::Size(800, 600);
            simulatorForm->Location = System::Drawing::Point(0, 0);
            simulatorForm->BackColor = System::Drawing::Color::FromArgb(190, 190, 195);

            m_pinnedSimForm = new gcroot<Form^>(simulatorForm);
            m_mainPanel->Controls->Add((Form^)*m_pinnedSimForm);
        }

        /**
        * On GUI Close Event
        */
        System::Void GUIForm_FormClosed(System::Object^ sender, FormClosedEventArgs^ e)
        {
            m_requestCallbacks->closeApplication();
        }

        /**
        * On Track bar value change
        */
        System::Void TrackBarValueChanged(System::Object^ sender, System::EventArgs^ e) 
        {
            m_requestCallbacks->sendValueRequest(VECTORIZATION, 
                m_trackBar->Value / (float)m_trackBar->Maximum);
        }

        /**
        * On Border button click
        */
        System::Void BorderButtonClick(System::Object^  sender, System::EventArgs^ e)
        {
            m_requestCallbacks->sendRequest(BORDER);
        }

        /**
        * On Pause button click
        */
        System::Void PauseButtonClick(System::Object^ sender, System::EventArgs^ e) 
        {
            m_requestCallbacks->sendRequest(PAUSE);
        }

        /**
        * On Save button click
        */
        System::Void SaveButtonClick(System::Object^ sender, System::EventArgs^ e)
        {
            m_requestCallbacks->sendRequest(SAVE);
        }

        /**
        * Required method for Designer support
        */        
        #pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->m_mainPanel = (gcnew System::Windows::Forms::Panel());
            this->m_trackBarPanel = (gcnew System::Windows::Forms::Panel());
            this->m_borderButton = (gcnew System::Windows::Forms::Button());
            this->m_saveButton = (gcnew System::Windows::Forms::Button());
            this->m_pauseButton = (gcnew System::Windows::Forms::Button());
            this->m_trackBar = (gcnew System::Windows::Forms::TrackBar());
            this->m_mainPanel->SuspendLayout();
            this->m_trackBarPanel->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->m_trackBar))->BeginInit();
            this->SuspendLayout();
            // 
            // m_mainPanel
            // 
            this->m_mainPanel->Controls->Add(this->m_trackBarPanel);
            this->m_mainPanel->Location = System::Drawing::Point(0, 0);
            this->m_mainPanel->Name = L"m_mainPanel";
            this->m_mainPanel->Size = System::Drawing::Size(640, 480);
            this->m_mainPanel->TabIndex = 0;
            // 
            // m_trackBarPanel
            // 
            this->m_trackBarPanel->BackColor = System::Drawing::Color::Transparent;
            this->m_trackBarPanel->Controls->Add(this->m_borderButton);
            this->m_trackBarPanel->Controls->Add(this->m_saveButton);
            this->m_trackBarPanel->Controls->Add(this->m_pauseButton);
            this->m_trackBarPanel->Controls->Add(this->m_trackBar);
            this->m_trackBarPanel->Location = System::Drawing::Point(6, 431);
            this->m_trackBarPanel->Name = L"m_trackBarPanel";
            this->m_trackBarPanel->Size = System::Drawing::Size(231, 44);
            this->m_trackBarPanel->TabIndex = 1;
            // 
            // m_borderButton
            // 
            this->m_borderButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(230)), static_cast<System::Int32>(static_cast<System::Byte>(230)),
                static_cast<System::Int32>(static_cast<System::Byte>(230)));
            this->m_borderButton->Cursor = System::Windows::Forms::Cursors::Hand;
            this->m_borderButton->FlatAppearance->BorderSize = 0;
            this->m_borderButton->FlatAppearance->CheckedBackColor = System::Drawing::Color::Gray;
            this->m_borderButton->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Gray;
            this->m_borderButton->FlatAppearance->MouseOverBackColor = System::Drawing::Color::DarkGray;
            this->m_borderButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->m_borderButton->Location = System::Drawing::Point(82, 7);
            this->m_borderButton->Margin = System::Windows::Forms::Padding(0);
            this->m_borderButton->Name = L"m_borderButton";
            this->m_borderButton->Size = System::Drawing::Size(32, 32);
            this->m_borderButton->TabIndex = 1;
            this->m_borderButton->TabStop = false;
            this->m_borderButton->UseVisualStyleBackColor = false;
            this->m_borderButton->Click += gcnew System::EventHandler(this, &GUIForm::BorderButtonClick);
            // 
            // m_saveButton
            // 
            this->m_saveButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(230)), static_cast<System::Int32>(static_cast<System::Byte>(230)),
                static_cast<System::Int32>(static_cast<System::Byte>(230)));
            this->m_saveButton->Cursor = System::Windows::Forms::Cursors::Hand;
            this->m_saveButton->FlatAppearance->BorderSize = 0;
            this->m_saveButton->FlatAppearance->CheckedBackColor = System::Drawing::Color::Gray;
            this->m_saveButton->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Gray;
            this->m_saveButton->FlatAppearance->MouseOverBackColor = System::Drawing::Color::DarkGray;
            this->m_saveButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->m_saveButton->Location = System::Drawing::Point(44, 7);
            this->m_saveButton->Margin = System::Windows::Forms::Padding(0);
            this->m_saveButton->Name = L"m_saveButton";
            this->m_saveButton->Size = System::Drawing::Size(32, 32);
            this->m_saveButton->TabIndex = 0;
            this->m_saveButton->TabStop = false;
            this->m_saveButton->UseVisualStyleBackColor = false;
            this->m_saveButton->Click += gcnew System::EventHandler(this, &GUIForm::SaveButtonClick);
            // 
            // m_pauseButton
            // 
            this->m_pauseButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(230)), static_cast<System::Int32>(static_cast<System::Byte>(230)),
                static_cast<System::Int32>(static_cast<System::Byte>(230)));
            this->m_pauseButton->Cursor = System::Windows::Forms::Cursors::Hand;
            this->m_pauseButton->FlatAppearance->BorderSize = 0;
            this->m_pauseButton->FlatAppearance->CheckedBackColor = System::Drawing::Color::Gray;
            this->m_pauseButton->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Gray;
            this->m_pauseButton->FlatAppearance->MouseOverBackColor = System::Drawing::Color::DarkGray;
            this->m_pauseButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->m_pauseButton->Location = System::Drawing::Point(6, 7);
            this->m_pauseButton->Margin = System::Windows::Forms::Padding(0);
            this->m_pauseButton->Name = L"m_pauseButton";
            this->m_pauseButton->Size = System::Drawing::Size(32, 32);
            this->m_pauseButton->TabIndex = 0;
            this->m_pauseButton->TabStop = false;
            this->m_pauseButton->UseVisualStyleBackColor = false;
            this->m_pauseButton->Click += gcnew System::EventHandler(this, &GUIForm::PauseButtonClick);
            // 
            // m_trackBar
            // 
            this->m_trackBar->AutoSize = false;
            this->m_trackBar->BackColor = System::Drawing::SystemColors::AppWorkspace;
            this->m_trackBar->LargeChange = 1;
            this->m_trackBar->Location = System::Drawing::Point(120, 8);
            this->m_trackBar->Name = L"m_trackBar";
            this->m_trackBar->Size = System::Drawing::Size(104, 30);
            this->m_trackBar->TabIndex = 0;
            this->m_trackBar->TabStop = false;
            this->m_trackBar->ValueChanged += gcnew System::EventHandler(this, &GUIForm::TrackBarValueChanged);
            // 
            // GUIForm
            // 
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
            this->BackColor = System::Drawing::Color::DimGray;
            this->ClientSize = System::Drawing::Size(640, 480);
            this->Controls->Add(this->m_mainPanel);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximumSize = System::Drawing::Size(656, 519);
            this->MinimumSize = System::Drawing::Size(656, 519);
            this->Name = L"GUIForm";
            this->Text = L"Complex Game Systems";
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &GUIForm::GUIForm_FormClosed);
            this->m_mainPanel->ResumeLayout(false);
            this->m_trackBarPanel->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->m_trackBar))->EndInit();
            this->ResumeLayout(false);

        }
        #pragma endregion
        
        GuiRequestCallbacks* m_requestCallbacks;   ///< Allows sending a GUI request
        gcroot<Form^>* m_pinnedSimForm;            ///< pinned as native needs window handle

        /**
        * Designer specific components
        */
        System::ComponentModel::Container^ components;
        System::Windows::Forms::TrackBar^ m_trackBar;
        System::Windows::Forms::Panel^ m_trackBarPanel;
        System::Windows::Forms::Button^  m_pauseButton;
        System::Windows::Forms::Button^  m_saveButton;
        System::Windows::Forms::Button^  m_borderButton;
        System::Windows::Forms::Panel^ m_mainPanel;
    };
}

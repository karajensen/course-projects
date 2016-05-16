////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GUIForm.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vcclr.h>
#include "GUICallbacks.h"

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
        GUIForm(void) :
            m_callbacks(nullptr),
            m_pinnedSimForm(nullptr)
        {
            InitializeComponent();
            CreateSimulationForm();
        }

        /**
        * Determines and returns the window/instance handles of the gui
        * @return The handles to the simulation window/instance
        */
        WindowHandle GetWindowHandle()
        {
            Form^ simForm = (Form^)*m_pinnedSimForm;
            WindowHandle window;

            IntPtr handle = simForm->Handle;
            IntPtr instance = System::Runtime::InteropServices::Marshal::GetHINSTANCE(
                System::Reflection::Assembly::GetExecutingAssembly()->GetModules()[0]);

            window.handle = static_cast<HWND>(handle.ToPointer());
            window.instance = static_cast<HINSTANCE>(instance.ToPointer());
            return window;
        }

        /**
        * Recieves the native callbacks and fills in the spinbox values 
        * @param callbacks The callbacks from the native application
        */
        void SetCallbacks(GuiCallbacks* callbacks)
        {
            m_callbacks = callbacks;
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
            m_callbacks->quitFn();
        }

        /**
        * On Track bar value change
        */
        System::Void TrackBarValueChanged(System::Object^  sender, System::EventArgs^  e) 
        {
            m_callbacks->setVectorizationAmount(m_trackBar->Value);
        }

        /**
        * Required method for Designer support
        */        
        #pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->m_mainPanel = (gcnew System::Windows::Forms::Panel());
            this->m_trackBarPanel = (gcnew System::Windows::Forms::Panel());
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
            this->m_mainPanel->Size = System::Drawing::Size(624, 441);
            this->m_mainPanel->TabIndex = 0;
            // 
            // m_trackBarPanel
            // 
            this->m_trackBarPanel->Controls->Add(this->m_trackBar);
            this->m_trackBarPanel->Location = System::Drawing::Point(6, 394);
            this->m_trackBarPanel->Name = L"m_trackBarPanel";
            this->m_trackBarPanel->Size = System::Drawing::Size(114, 40);
            this->m_trackBarPanel->TabIndex = 1;
            // 
            // m_trackBar
            // 
            this->m_trackBar->AutoSize = false;
            this->m_trackBar->BackColor = System::Drawing::SystemColors::AppWorkspace;
            this->m_trackBar->LargeChange = 1;
            this->m_trackBar->Location = System::Drawing::Point(5, 5);
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
            this->ClientSize = System::Drawing::Size(624, 441);
            this->Controls->Add(this->m_mainPanel);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximumSize = System::Drawing::Size(640, 480);
            this->MinimumSize = System::Drawing::Size(640, 480);
            this->Name = L"GUIForm";
            this->Text = L"Complex Game Systems";
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &GUIForm::GUIForm_FormClosed);
            this->m_mainPanel->ResumeLayout(false);
            this->m_trackBarPanel->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->m_trackBar))->EndInit();
            this->ResumeLayout(false);

        }
        #pragma endregion
        
        GuiCallbacks* m_callbacks;      ///< Callbacks for the gui
        gcroot<Form^>* m_pinnedSimForm; ///< pinned as native needs window handle

        /**
        * Designer specific components
        */
        System::ComponentModel::Container^ components;
        System::Windows::Forms::TrackBar^ m_trackBar;
        System::Windows::Forms::Panel^ m_trackBarPanel;
        System::Windows::Forms::Panel^ m_mainPanel;
    };
}

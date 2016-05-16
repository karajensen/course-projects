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
            CreateButtons();
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
        * Creates and adds a button to the gui
        * @param button A tracker reference to the button control 
        * @param image The image filename for the button icon 
        * @param tip A tooltip description for the button 
        * @param index The index for the button, used to determine positioning 
        * @param callback The method to call when the button is clicked 
        */
        void CreateButton(Button^% button, 
                          String^ image, 
                          String^ tip, 
                          int index, 
                          EventHandler^ callback)
        {
            button = gcnew Button();
            button->Click += callback;
            CreateControl(button, image, tip, index);
        }

        /**
        * On GUI Close Event
        */
        System::Void GUIFormClosed(System::Object^ sender, FormClosedEventArgs^ e) 
        {
            m_callbacks->quitFn();
        }

        /**
        * On Test Button Press
        */
        System::Void TestClick(System::Object^ sender, System::EventArgs^ e)
        {
            m_callbacks->testButton();
        }

        /**
        * Creates the gui buttons
        */
        void CreateButtons()
        {
            int index = 0;
            String^ path = "Resources//Sprites//";
            m_pressedColor = Color::DarkGray;
            m_unpressedColor = Color::FromArgb(230, 230, 230);

            CreateButton(m_testButton, path + "resetcam.png", "Reset the camera",
                index++, gcnew System::EventHandler(this, &GUIForm::TestClick));
        }

        /**
        * Creates a gui control
        * @param control The control to add to the gui 
        * @param image The image filename for the control icon 
        * @param tip A tooltip description for the control 
        * @param index The index for the control, used to determine positioning    
        */        
        void CreateControl(ButtonBase^ control, String^ image, String^ tip, int index)
        {
            const int buttonSize = 32;
            const int border = 2;

            m_guiPanel->Controls->Add(control);
            control->TabIndex = 0;
            control->TabStop = false;
            control->UseVisualStyleBackColor = false;
            control->BackgroundImage = Image::FromFile(image);
            control->Cursor = System::Windows::Forms::Cursors::Hand;
            control->Location = System::Drawing::Point(border, (index*buttonSize)+border);
            control->Margin = System::Windows::Forms::Padding(0);
            control->MaximumSize = System::Drawing::Size(buttonSize, buttonSize);
            control->MinimumSize = System::Drawing::Size(buttonSize, buttonSize);
            control->Size = System::Drawing::Size(buttonSize, buttonSize);
            control->FlatAppearance->BorderSize = 0;
            control->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            control->FlatAppearance->CheckedBackColor = m_pressedColor;
            control->FlatAppearance->MouseDownBackColor = m_pressedColor;
            control->FlatAppearance->MouseOverBackColor = Color::FromArgb(150, 150, 240);
            control->BackColor = m_unpressedColor;

            ToolTip^ tooltip = gcnew ToolTip();
            tooltip->SetToolTip(control, tip);
        }

        /**
        * Required method for Designer support
        */        
        #pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->m_mainPanel = (gcnew System::Windows::Forms::Panel());
            this->m_guiPanel = (gcnew System::Windows::Forms::Panel());
            this->m_mainPanel->SuspendLayout();
            this->SuspendLayout();
            // 
            // m_mainPanel
            // 
            this->m_mainPanel->Controls->Add(this->m_guiPanel);
            this->m_mainPanel->Location = System::Drawing::Point(0, 0);
            this->m_mainPanel->Name = L"m_mainPanel";
            this->m_mainPanel->Size = System::Drawing::Size(800, 600);
            this->m_mainPanel->TabIndex = 0;
            // 
            // m_guiPanel
            // 
            this->m_guiPanel->BackColor = System::Drawing::Color::Gray;
            this->m_guiPanel->Location = System::Drawing::Point(12, 12);
            this->m_guiPanel->Name = L"m_guiPanel";
            this->m_guiPanel->Size = System::Drawing::Size(36, 484);
            this->m_guiPanel->TabIndex = 14;
            // 
            // GUIForm
            // 
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
            this->BackColor = System::Drawing::Color::DimGray;
            this->ClientSize = System::Drawing::Size(788, 588);
            this->Controls->Add(this->m_mainPanel);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximumSize = System::Drawing::Size(806, 633);
            this->MinimumSize = System::Drawing::Size(806, 633);
            this->Name = L"GUIForm";
            this->Text = L"Complex Game Systems";
            this->FormClosed += gcnew FormClosedEventHandler(this, &GUIForm::GUIFormClosed);
            this->m_mainPanel->ResumeLayout(false);
            this->ResumeLayout(false);

        }
        #pragma endregion
        
        GuiCallbacks* m_callbacks;      ///< Callbacks for the gui
        gcroot<Form^>* m_pinnedSimForm; ///< pinned as native needs window handle
        Color m_unpressedColor;         ///< Color of buttons when unpressed
        Color m_pressedColor;           ///< Color of buttons when pressed
        Button^ m_testButton;           ///< Button for resetting the camera

        /**
        * Designer specific components
        */
        System::ComponentModel::Container^ components;  ///< Auto-Required designer variable.
        System::Windows::Forms::Panel^ m_mainPanel;     ///< Panel for holding main simulation form
        System::Windows::Forms::Panel^ m_guiPanel;      ///< Panel for gui buttons        
    };
}

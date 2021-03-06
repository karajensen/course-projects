////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GUIWrapper.cpp
////////////////////////////////////////////////////////////////////////////////////////

#define EXPORTAPI
#include "GUIWrapper.h"
#include "GUIForm.h"
#include <vcclr.h>
#include <Windows.h>

namespace GUI
{
    typedef gcroot<GUIForm^>* GuiPtr;

    GuiWrapper::GuiWrapper() :
        m_guiForm(nullptr)
    {
        //gcroot will point to updated address as gc moves object
        GUIForm^ gui = gcnew GUIForm();
        GuiPtr ptr = new gcroot<GUIForm^>(gui);
        m_guiForm = (void*)ptr;
    }

    GuiWrapper::~GuiWrapper()
    {
        #ifdef _DEBUG
        OutputDebugString(L"GuiWrapper::~GuiWrapper\n");
        #endif

        if (m_guiForm)
        {
            //free gcroot handle
            GuiPtr ptr = reinterpret_cast<GuiPtr>(m_guiForm);
            m_guiForm = nullptr;
            delete ptr;
        }
    }

    void GuiWrapper::Initialize(GuiRequestCallbacks* requestCallbacks)
    {
        GuiPtr ptr = reinterpret_cast<GuiPtr>(m_guiForm);
        GUIForm^ form = ((GUIForm^)*ptr);

        System::Drawing::Size size = form->GetWindowSize();
        HWND hwnd = form->GetWindowHandle();
        HINSTANCE hinstance = form->GetWindowInstance();

        requestCallbacks->sendWindowHandle(hwnd, hinstance);
        requestCallbacks->sendWindowSize(size.Width, size.Height);

        form->Initialize(requestCallbacks);
    }

    void GuiWrapper::Update()
    {
        System::Windows::Forms::Application::DoEvents();
    }

    void GuiWrapper::Show()
    {
        GuiPtr ptr = reinterpret_cast<GuiPtr>(m_guiForm);
        ((GUIForm^)*ptr)->Show();
    }
}

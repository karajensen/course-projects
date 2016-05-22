////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - GUIWrapper.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "GUITypes.h"
#include <memory>

#ifdef EXPORTAPI
#define GUIINTAPI __declspec(dllexport)
#else
#define GUIINTAPI __declspec(dllimport)
#endif

namespace GUI
{
    /**
    * Interface that native code will talk to
    */
    class GUIINTAPI GuiWrapper
    {
    public:

        GuiWrapper();
        ~GuiWrapper();

        /**
        * Initializes the GUI
        * @note pointer to callbacks send as mutex/thread not supported in C++/CLI
        */
        void Initialize(GuiRequestCallbacks* requestCallbacks);

        /**
        * Ticks the GUI
        * @return whether the update was successful
        */
        bool Update();

        /**
        * @return the handle to the simulation window
        */
        WindowHandle GetWindowHandle();

        /**
        * Shows the GUI Window
        */
        void Show();

    private:

        void* m_guiForm; ///< pointer to managed gui form
    };
}
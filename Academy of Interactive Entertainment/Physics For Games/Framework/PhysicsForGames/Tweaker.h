////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Tweaker.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AntTweakBar.h"
#include <string>
#include <functional>
#include <vector>
#include <memory>
#include <array>
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

class Input;

/**
* Tweak bar for manipulating the scene
*/
class Tweaker
{
public:

    /**
    * Constructor
    * @param size The size of the window
    */
    Tweaker(const glm::ivec2& size);

    /**
    * Destructor
    */
    ~Tweaker();

    /**
    * Renders the GUI tweak bar
    */
    void Render();

    /**
    * Adds a callback on reset
    */
    void AddResetFn(std::function<void(void)> fn);

    /**
    * Resets the tweak bar
    */
    void Reset();

    /**
    * Updates the tweak bar
    */
    void Update(Input& input);

    /**
    * Sets the group used
    * @param group The group to set
    */
    void SetGroup(std::string group);

    /**
    * Creates a new button in the tweak bar
    * @param label What the button will display
    * @param callback The function to use when pressed
    */
    void AddButton(std::string label, 
                   std::function<void(void)> callback);

    /**
    * Creates a new readonly entry for displaying a string
    * @param label What to display the entry as
    * @param getter function to retrieve the string value
    */
    void AddStrEntry(std::string label, 
                     std::function<const std::string(void)> getter);

    /**
    * Creates a new readonly entry for displaying a string
    * @param label What to display the entry as
    * @param getter function to retrieve the string value
    * @param getter function to set the string value
    */
    void AddEnumEntry(std::string label, 
                      std::function<const int(void)> getter,
                      std::function<void(const int)> setter,
                      const std::vector<const char*>& values);

    /**
    * Creates a new tweakable entry for a colour
    * @param label What to display the entry as
    * @param getter function to retrieve the color value
    * @param setter function to set the color value
    */
    void AddColorEntry(std::string label,
                       std::function<const glm::vec4(void)> getter,
                       std::function<void(const glm::vec4)> setter);

    /**
    * Creates a new tweakable entry for an index
    * @param label What to display the entry as
    * @param getter Callback to get the index
    * @param setter Callback for set the index
    * @param max The maximum value the entry can obtain
    */
    void AddIntEntry(std::string label, 
                     std::function<const int(void)> getter,
                     std::function<void(const int)> setter,
                     unsigned int max);

    /**
    * Creates a new tweakable entry for an value
    * @param label What to display the entry as
    * @param getter Callback to get the value
    * @param setter Callback for set the value
    * @param step The differece between values
    * @param precision The amount of decimal points to display
    */
    void AddFltEntry(std::string label, 
                     std::function<const float(void)> getter,
                     std::function<void(const float)> setter,
                     float step,
                     int precision);

    /**
    * Creates a new tweakable entry for a bool
    * @param label What to display the entry as
    * @param getter Callback to get the value
    * @param setter Callback for set the value
    */
    void AddBoolEntry(std::string label,
                      std::function<const bool(void)> getter,
                      std::function<void(const bool)> setter);

    /**
    * Base for a callback entry for allowing templating in cpp
    * @note required to be public for templating
    */
    struct Entry
    {
        virtual void Getter(void* value) const = 0;
        virtual void Setter(const void* value) = 0;
    };

private:

    /**
    * Prevent Copying
    */
    Tweaker(const Tweaker&) = delete;
    Tweaker& operator=(const Tweaker&) = delete;

    /**
    * Fixed size of all string entries
    */
    enum
    {
        STR_BUFFER_SIZE = 128
    };

    /**
    * Data for a button
    */
    struct Button
    {
        std::function<void(void)> callback = nullptr;
    };

    /**
    * Data for a string
    * @note fixed character buffer used in place of 
    * dynamic string / TwCopyStdStringToLibrary for performance
    */
    struct Label
    {
        bool modifiable = false;
        char buffer[STR_BUFFER_SIZE];
        std::string value;
        std::function<const std::string(void)> getter = nullptr;
    };

    /**
    * Clears all current entries
    */
    void ClearEntries();

    /**
    * Fills the internal buffer of a label
    */
    void FillBufffer(Label& label);

    /**
    * Helper function to get a new name for an entry
    */
    std::string GetName();

    /**
    * Helper function to get a definition for a tweakable entry
    * @param label What to display the entry as
    */
    std::string Definition(std::string label) const;

    /**
    * Helper function to get a definition for a tweakable entry
    * @param label What to display the entry as
    * @param min The minimum value the entry can obtain
    * @param max The maximum value the entry can obtain
    */
    std::string Definition(std::string label, int min, int max) const;

    /**
    * Helper function to get a definition for a tweakable entry
    * @param label What to display the entry as
    * @param step The differece between values
    * @param precision The amount of decimal points to display
    */
    std::string Definition(std::string label, float step, int precision) const;

    /**
    * Determines if there is an error and logs
    */
    void LogTweakError() const;

    /**
    * Allows adding lambda callbacks to the tweak 
    * bar due to the tweaker requiring stdcall convention
    * @parma clientData Address of the button to call
    */
    static void TW_CALL CallButton(void *clientData);

    /**
    * Allows adding lambda callbacks to the tweak 
    * bar due to the tweaker requiring stdcall convention
    * @param value The value to be set
    * @param clientData Address of the button to call
    */
    static void TW_CALL SetCallback(const void *value, void *clientData);

    /**
    * Allows adding lambda callbacks to the tweak 
    * bar due to the tweaker requiring stdcall convention
    * @param value The value to be set
    * @param clientData Address of the button to call
    */
    static void TW_CALL GetCallback(void *value, void *clientData);

    int m_count = 0;                                    ///< Number of entries added
    std::string m_group;                                ///< Global group currently set
    std::vector<std::unique_ptr<Label>> m_labels;       ///< Data for the tweak bar strings
    std::vector<std::unique_ptr<Button>> m_buttons;     ///< Data for the tweak bar buttons
    std::vector<std::unique_ptr<Entry>> m_entries;      ///< Data for the tweak bar getter/setters
    std::function<void(void)> m_resetFn = nullptr;      ///< Callback to reset the tweak bar
    CTwBar* m_tweakbar = nullptr;                       ///< Internal tweak bar
};
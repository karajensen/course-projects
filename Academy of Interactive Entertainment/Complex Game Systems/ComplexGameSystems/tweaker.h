////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <functional>
#include <vector>
#include <memory>
#include "AntTweakBar.h"
#include "directxcommon.h"

/**
* Read only tweak bar for displaying diagnostics
*/
class Tweaker
{
public:

    /**
    * Constructor
    */
    Tweaker(ID3D11Device* device, const POINT& size);

    /**
    * Destructor
    */
    ~Tweaker();

    /**
    * Updates the tweak bar
    */
    void Update();

    /**
    * Sets the group used
    * @param group The group to set
    */
    void SetGroup(std::string group);

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
    */
    void AddStrEntry(std::string label, std::string entry);

    /**
    * Creates a new tweakable entry for an int
    * @param label What to display the entry as
    * @param getter Callback to get the int
    */
    void AddIntEntry(std::string label, 
                     std::function<const int(void)> getter);

    /**
    * Creates a new tweakable entry for an value
    * @param label What to display the entry as
    * @param getter Callback to get the value
    */
    void AddFltEntry(std::string label, 
                     std::function<const float(void)> getter);

    /**
    * Creates a new tweakable entry for an value
    * @param label What to display the entry as
    * @param getter Callback to get the value
    */
    void AddDblEntry(std::string label, 
                     std::function<const double(void)> getter);

    /**
    * Base for a callback entry for allowing templating in cpp
    */
    struct Entry
    {
        virtual void Getter(void* value) const = 0;
    };

private:

    /**
    * Fixed size of all string entries
    */
    enum
    {
        STR_BUFFER_SIZE = 128
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
    * Determines if there is an error and logs
    */
    void LogTweakError() const;

    /**
    * Allows adding lambda callbacks to the tweak 
    * bar due to the tweaker requiring stdcall convention
    * @param value The value to be set
    * @param clientData Address of the button to call
    */
    static void TW_CALL GetCallback(void *value, void *clientData);

    int m_count = 0;                                    ///< Number of entries added
    std::string m_group;                                ///< Global group currently set
    CTwBar* m_tweakBar = nullptr;                       ///< Tweak bar to fill in, not owned by this class
    std::vector<std::unique_ptr<Label>> m_labels;       ///< Data for the tweak bar strings
    std::vector<std::unique_ptr<Entry>> m_entries;      ///< Data for the tweak bar getters
};
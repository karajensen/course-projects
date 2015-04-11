////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "tweaker.h"
#include <algorithm>

namespace
{
    /**
    * Data for a getter/setter callback
    */
    template <typename T> struct TweakableEntry : public Tweaker::Entry
    {
        virtual void Getter(void* value) const override
        {
            if (std::is_same<std::string, T>::value)
            {
                T source = getter();
                CopyString(value, &source);
            }
            else
            {
                *static_cast<T*>(value) = getter();
            }
        }

        virtual void Setter(const void* value) override
        {
            setter(*static_cast<const T*>(value));
        }

        std::function<const T(void)> getter = nullptr;
        std::function<void(const T)> setter = nullptr;
    };

    /**
    * Helper function for copying a string for the tweak bar
    */
    void CopyString(void* destination, void* source)
    {
        TwCopyStdStringToLibrary(
            *static_cast<std::string*>(destination),
            *static_cast<std::string*>(source));
    }
}

Tweaker::Tweaker(CTwBar* tweakbar) :
    m_tweakBar(tweakbar)
{
}

std::string Tweaker::Definition(std::string label, unsigned int max) const
{
    return " max=" + std::to_string(max) + " min=0" + Definition(label);
}

std::string Tweaker::Definition(std::string label, float step) const
{
    return " step=" + std::to_string(step) + Definition(label);
}

std::string Tweaker::Definition(std::string label) const
{
    return " group='" + m_group + "'" + " label='" + label + "' ";
}

void Tweaker::SetGroup(std::string group)
{
    m_group = group;
}

void TW_CALL Tweaker::CallButton(void *clientData)
{
    static_cast<Button*>(clientData)->callback();
}

void TW_CALL Tweaker::SetCallback(const void *value, void *clientData)
{
    static_cast<Entry*>(clientData)->Setter(value);
}

void TW_CALL Tweaker::GetCallback(void *value, void *clientData)
{
    static_cast<Entry*>(clientData)->Getter(value);
}

void Tweaker::AddEntry(std::string label,
                       void* entry, 
                       TwType type, 
                       bool readonly)
{
    if (readonly)
    {
        TwAddVarRO(m_tweakBar, GetName(), type,
            entry, Definition(label).c_str());
    }
    else
    {
        TwAddVarRW(m_tweakBar, GetName(), type,
            entry, Definition(label).c_str());
    }
}

void Tweaker::AddEntry(std::string label, 
                       void* entry, 
                       TwType type, 
                       float step)
{
    TwAddVarRW(m_tweakBar, GetName(), type, entry, 
        Definition(label, step).c_str());
}

void Tweaker::AddEntry(std::string label,
                       std::function<const int(void)> getter,
                       std::function<void(const int)> setter,
                       unsigned int max)
{
    const auto index = m_entries.size();
    auto entry = std::make_unique<TweakableEntry<int>>();
    entry->getter = getter;
    entry->setter = setter;
    m_entries.emplace_back(std::move(entry));
    
    TwAddVarCB(m_tweakBar, GetName(), TW_TYPE_INT32, SetCallback,
        GetCallback, m_entries[index].get(), Definition(label, max).c_str());
}

void Tweaker::AddEntry(std::string label, 
                       std::function<const std::string(void)> getter)
{
    const auto index = m_entries.size();
    auto entry = std::make_unique<TweakableEntry<std::string>>();
    entry->getter = getter;
    m_entries.emplace_back(std::move(entry));
    
    TwAddVarCB(m_tweakBar, GetName(), TW_TYPE_STDSTRING, nullptr,
        GetCallback, m_entries[index].get(), Definition(label).c_str());
}

void Tweaker::AddEntry(std::string label, unsigned int size)
{
    const auto index = m_entries.size();
    auto entry = std::make_unique<TweakableEntry<unsigned int>>();
    entry->getter = [size](){ return size; };
    m_entries.emplace_back(std::move(entry));
    
    TwAddVarCB(m_tweakBar, GetName(), TW_TYPE_UINT32, nullptr,
        GetCallback, m_entries[index].get(), Definition(label).c_str());
}

void Tweaker::AddButton(std::string label, 
                        std::function<void(void)> callback)
{
    const auto index = m_buttons.size();
    m_buttons.push_back(std::make_unique<Button>());
    m_buttons[index]->callback = callback;
    
    TwAddButton(m_tweakBar, GetName(),
        CallButton, m_buttons[index].get(), Definition(label).c_str());
}

const char* Tweaker::GetName()
{
    ++m_count;
    return ("Entry" + std::to_string(m_count)).c_str();
}

void Tweaker::ClearEntries()
{
    m_count = 0;
    m_entries.clear();
    m_buttons.clear();
    m_group.clear();
}
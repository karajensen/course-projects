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
    template <typename T> 
    struct TweakableEntry : public Tweaker::Entry
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
    * Data for a getter/setter callback for an array of values
    */
    template <typename T> 
    struct TweakableSubGroup : public Tweaker::SubGroup
    {
        virtual void Getter(void* value) const override
        {
            const std::vector<T> array = getter();
            for (unsigned int i = 0; i < values.size(); ++i)
            {
                static_cast<T*>(value)[i] = array[i];
            }
        }

        virtual void Setter(const void* value) override
        {
            std::vector<T> array;
            for (unsigned int i = 0; i < values.size(); ++i)
            {
                array.push_back(static_cast<const T*>(value)[i]);
            }
            setter(array);
        }

        std::function<const std::vector<T>(void)> getter = nullptr;
        std::function<void(const std::vector<T>)> setter = nullptr;
    };

    /**
    * Helper function for copying a string for the tweak bar
    * @param destination The string to copy to
    * @param source The string to copy from
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

std::string Tweaker::Definition(std::string label, float step, int precision) const
{
    const int defaultPrecision = 3;
    return " step=" + std::to_string(step) + 
        " precision=" + std::to_string(precision == 0 ? defaultPrecision : precision) + 
        Definition(label);
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
                       float step, 
                       int precision)
{
    TwAddVarRW(m_tweakBar, GetName(), type, entry, 
        Definition(label, step, precision).c_str());
}

void Tweaker::AddEntry(std::string label,
                       std::function<const float(void)> getter,
                       std::function<void(const float)> setter)
{
    const auto index = m_entries.size();
    auto entry = std::make_unique<TweakableEntry<float>>();
    entry->getter = getter;
    entry->setter = setter;
    m_entries.emplace_back(std::move(entry));
    
    TwAddVarCB(m_tweakBar, GetName(), TW_TYPE_FLOAT, SetCallback,
        GetCallback, m_entries[index].get(), Definition(label, 0.1f).c_str());
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
    m_subGroups.clear();
}

template <typename T> 
TwType Tweaker::MakeSubGroup(std::vector<std::pair<std::string, T*>>& entries,
                             TwType type,
                             float step)
{
    auto entry = std::make_unique<TweakableSubGroup<T>>();
    auto* subgroup = entry.get();

    entry->getter = [subgroup]() -> const std::vector<T>
    {
        std::vector<T> values;
        for (void* value : subgroup->values)
        {
            values.push_back(*static_cast<T*>(value));
        }
        return values;
    };

    entry->setter = [subgroup](const std::vector<T> values)
    {
        for (unsigned int i = 0; i < values.size(); ++i)
        {
            *static_cast<T*>(subgroup->values[i]) = values[i];
        }
    };

    std::vector<TwStructMember> members;
    for (unsigned int count = 0; count < entries.size(); ++count)
    {
        auto& pair = entries[count];
        entry->values.push_back(pair.second);
    
        // Note name doesn't need to be unqiue
        // Definition doesn't support label/group
        members.emplace_back();
        members[count].Name = pair.first.c_str();
        members[count].Type = type;
        members[count].Offset = sizeof(T)*count;
        members[count].DefString = std::string(step == 0.0f ? 
            "" : "step=" + std::to_string(step)).c_str();
    }
    
    m_subGroups.push_back(std::move(entry));

    return TwDefineStruct(GetName(), &members[0], members.size(), 
        sizeof(T)*members.size(), nullptr, nullptr);
}

void Tweaker::AddSubGroup(std::string label,
                          std::vector<std::pair<std::string, float*>>& entries,
                          float step)
{
    const TwType subgroupType = MakeSubGroup(entries, TW_TYPE_FLOAT, step);
    auto& entry = m_subGroups[m_subGroups.size()-1];

    TwAddVarCB(m_tweakBar, GetName(), subgroupType, 
        SetCallback, GetCallback, entry.get(), Definition(label).c_str());
}
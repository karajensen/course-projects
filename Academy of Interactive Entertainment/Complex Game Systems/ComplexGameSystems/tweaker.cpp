////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "tweaker.h"
#include <assert.h>
#include <sstream>
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
            *static_cast<T*>(value) = getter();
        }

        std::function<const T(void)> getter = nullptr;
    };
}

Tweaker::Tweaker(ID3D11Device* device, const POINT& size)
{
    TwInit(TW_DIRECT3D11, device);
    TwWindowSize(size.x, size.y);

    const std::string barname = "Diagnostics";
    m_tweakBar = TwNewBar(barname.c_str());

    const int border = 10;
    std::ostringstream stream;
    stream << barname << " label='Diagnostics' "
        << "position='" << border << " " << border << "' "
        << "size='200 200'"
        << "alpha=180 text=light valueswidth=80 color='0 0 0' "
        << "refresh=0.05 iconified=false resizable=true "
        << "fontsize=2 fontresizable=false ";
    TwDefine(stream.str().c_str());
}

Tweaker::~Tweaker()
{
    if (m_tweakBar)
    {
        TwDeleteBar(m_tweakBar);
    }
    TwTerminate();
}

void Tweaker::Update()
{
    for (auto& label : m_labels)
    {
        if (label->modifiable)
        {
            const std::string value = label->getter();
            if (value != label->value)
            {
                assert(value.size() < STR_BUFFER_SIZE);
                label->value = value;
                FillBufffer(*label);
            }
        }
    }

    TwDraw();
}

std::string Tweaker::Definition(std::string label) const
{
    return " precision=" + std::to_string(8) + 
        " group='" + m_group + "'" + " label='" + label + "' ";
}

void Tweaker::SetGroup(std::string group)
{
    m_group = group;
}

void TW_CALL Tweaker::GetCallback(void *value, void *clientData)
{
    static_cast<Entry*>(clientData)->Getter(value);
}

void Tweaker::AddDblEntry(std::string label, std::function<const double(void)> getter)
{
    const auto index = m_entries.size();
    auto entry = std::make_unique<TweakableEntry<double>>();
    entry->getter = getter;
    m_entries.emplace_back(std::move(entry));

    TwAddVarCB(m_tweakBar, GetName().c_str(), TW_TYPE_DOUBLE, nullptr,
        GetCallback, m_entries[index].get(), Definition(label).c_str());

    LogTweakError();
}

void Tweaker::AddFltEntry(std::string label, std::function<const float(void)> getter)
{
    const auto index = m_entries.size();
    auto entry = std::make_unique<TweakableEntry<float>>();
    entry->getter = getter;
    m_entries.emplace_back(std::move(entry));
    
    TwAddVarCB(m_tweakBar, GetName().c_str(), TW_TYPE_FLOAT, nullptr,
        GetCallback, m_entries[index].get(), Definition(label).c_str());

    LogTweakError();
}

void Tweaker::AddIntEntry(std::string label, std::function<const int(void)> getter)
{
    const auto index = m_entries.size();
    auto entry = std::make_unique<TweakableEntry<int>>();
    entry->getter = getter;
    m_entries.emplace_back(std::move(entry));
    
    TwAddVarCB(m_tweakBar, GetName().c_str(), TW_TYPE_INT32, nullptr,
        GetCallback, m_entries[index].get(), Definition(label).c_str());

    LogTweakError();
}

void Tweaker::FillBufffer(Label& label)
{
    for (unsigned int i = 0; i < label.value.size(); ++i)
    {
        label.buffer[i] = label.value[i];
    }
    label.buffer[label.value.size()] = '\0';
}

void Tweaker::AddStrEntry(std::string label, std::function<const std::string(void)> getter)
{
    const auto index = m_labels.size();
    m_labels.push_back(std::make_unique<Label>());
    
    const std::string value = getter();
    m_labels[index]->getter = getter;
    m_labels[index]->modifiable = true;
    m_labels[index]->value = value;
    FillBufffer(*m_labels[index]);

    TwAddVarRO(m_tweakBar, GetName().c_str(), 
        TW_TYPE_CSSTRING(STR_BUFFER_SIZE),
        m_labels[index]->buffer, Definition(label).c_str());

    LogTweakError();
}

void Tweaker::AddStrEntry(std::string label, std::string value)
{
    const auto index = m_labels.size();
    m_labels.push_back(std::make_unique<Label>());
    m_labels[index]->modifiable = false;
    m_labels[index]->value = value;
    FillBufffer(*m_labels[index]);

   TwAddVarRO(m_tweakBar, GetName().c_str(), 
       TW_TYPE_CSSTRING(STR_BUFFER_SIZE),
       m_labels[index]->buffer, Definition(label).c_str());

    LogTweakError();
}

std::string Tweaker::GetName()
{
    return "Entry" + std::to_string(++m_count);
}

void Tweaker::LogTweakError() const
{
    const char* error = TwGetLastError();
    if (error != nullptr)
    {
        MessageBox(0, ("Tweaker: Error for " + m_group + " - " +
            std::to_string(m_count)).c_str(), "ERROR", MB_OK);
    }
}
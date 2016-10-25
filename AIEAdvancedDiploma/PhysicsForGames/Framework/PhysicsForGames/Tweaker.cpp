////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Tweaker.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Tweaker.h"
#include "Utilities.h"
#include "Input.h"
#include <assert.h>
#include <algorithm>
#include <sstream>

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

        virtual void Setter(const void* value) override
        {
            setter(*static_cast<const T*>(value));
        }

        std::function<const T(void)> getter = nullptr;
        std::function<void(const T)> setter = nullptr;
    };
}

Tweaker::Tweaker(const glm::ivec2& size)
{
    TwInit(TW_OPENGL_CORE, nullptr);
    TwWindowSize(size.x, size.y);

    const std::string barname = "GamesForPhysics";
    m_tweakbar = TwNewBar(barname.c_str());

    const int border = 10;
    std::ostringstream stream;
    stream << barname << " label='Physics For Games' "
        << "position='" << border << " " << border << "' "
        << "size='250 450' "
        << "alpha=180 text=light valueswidth=80 color='0 0 0' "
        << "refresh=0.05 iconified=false resizable=true "
        << "fontsize=2 fontresizable=false ";
    TwDefine(stream.str().c_str());
}

Tweaker::~Tweaker()
{
    if (m_tweakbar)
    {
        TwDeleteBar(m_tweakbar);
    }
    TwTerminate();
}

void Tweaker::Render()
{
    TwDraw();
}

void Tweaker::AddResetFn(std::function<void(void)> fn)
{
    m_resetFn = fn;
}

void Tweaker::Reset()
{
    TwRemoveAllVars(m_tweakbar);
    ClearEntries();

    if (m_resetFn)
    {
        m_resetFn();
    }
}

void Tweaker::Update(Input& input)
{
    if (input.HasMouseMoved())
    {
        TwMouseMotion(input.X(), input.Y());
    }

    if (input.WasMousePressed())
    {
        TwMouseButton(TW_MOUSE_PRESSED, TW_MOUSE_LEFT);
    }

    if (input.WasMouseReleased())
    {
        TwMouseButton(TW_MOUSE_RELEASED, TW_MOUSE_LEFT);
    }

    for (auto& key : input.GetKeys())
    {
        if (key.second.pressed)
        {
            TwKeyPressed(key.second.code, 0);
        }
    }

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
}

std::string Tweaker::Definition(std::string label, int min, int max) const
{
    return " max=" + std::to_string(max) + 
           " min=" + std::to_string(min) + 
           Definition(label);
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

void Tweaker::AddFltEntry(std::string label,
                          std::function<const float(void)> getter,
                          std::function<void(const float)> setter,
                          float step,
                          int precision)
{
    const auto index = m_entries.size();
    auto entry = std::make_unique<TweakableEntry<float>>();
    entry->getter = getter;
    entry->setter = setter;
    m_entries.emplace_back(std::move(entry));
    
    TwAddVarCB(m_tweakbar, GetName().c_str(), TW_TYPE_FLOAT, SetCallback,
        GetCallback, m_entries[index].get(), Definition(label, step, precision).c_str());

    LogTweakError();
}

void Tweaker::AddIntEntry(std::string label,
                          std::function<const int(void)> getter,
                          std::function<void(const int)> setter,
                          unsigned int max)
{
    const auto index = m_entries.size();
    auto entry = std::make_unique<TweakableEntry<int>>();
    entry->getter = getter;
    entry->setter = setter;
    m_entries.emplace_back(std::move(entry));
    
    TwAddVarCB(m_tweakbar, GetName().c_str(), TW_TYPE_INT32, 
        SetCallback, GetCallback, m_entries[index].get(), 
        Definition(label, 0, static_cast<int>(max)).c_str());

    LogTweakError();
}

void Tweaker::AddBoolEntry(std::string label,
                           std::function<const bool(void)> getter,
                           std::function<void(const bool)> setter)
{
    const auto index = m_entries.size();
    auto entry = std::make_unique<TweakableEntry<bool>>();
    entry->getter = getter;
    entry->setter = setter;
    m_entries.emplace_back(std::move(entry));

    TwAddVarCB(m_tweakbar, GetName().c_str(), TW_TYPE_BOOLCPP,
        SetCallback, GetCallback, m_entries[index].get(),
        Definition(label).c_str());

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

void Tweaker::AddStrEntry(std::string label, 
                          std::function<const std::string(void)> getter)
{
    const auto index = m_labels.size();
    m_labels.push_back(std::make_unique<Label>());
    
    const std::string value = getter();
    m_labels[index]->getter = getter;
    m_labels[index]->modifiable = true;
    m_labels[index]->value = value;
    FillBufffer(*m_labels[index]);

    TwAddVarRO(m_tweakbar, GetName().c_str(), 
        TW_TYPE_CSSTRING(STR_BUFFER_SIZE),
        m_labels[index]->buffer, Definition(label).c_str());

    LogTweakError();
}

void Tweaker::AddButton(std::string label, 
                        std::function<void(void)> callback)
{
    const auto index = m_buttons.size();
    m_buttons.push_back(std::make_unique<Button>());
    m_buttons[index]->callback = callback;
    
    TwAddButton(m_tweakbar, GetName().c_str(),
        CallButton, m_buttons[index].get(), Definition(label).c_str());

    LogTweakError();
}

std::string Tweaker::GetName()
{
    return "Entry" + std::to_string(++m_count);
}

void Tweaker::ClearEntries()
{
    m_entries.clear();
    m_buttons.clear();
    m_group.clear();
}

void Tweaker::LogTweakError() const
{
    const char* error = TwGetLastError();
    if (error != nullptr)
    {
        LogError("Tweaker: Error for " + m_group + " - " + std::to_string(m_count));
    }
}

void Tweaker::AddColorEntry(std::string label,
                            std::function<const glm::vec4(void)> getter,
                            std::function<void(const glm::vec4)> setter)
{
    typedef std::array<float, 4> Colour;

    const auto index = m_entries.size();
    auto entry = std::make_unique<TweakableEntry<Colour>>();

    entry->getter = [getter]() -> const Colour
    {
        const glm::vec4 colour = getter();
        return { colour.r, colour.g, colour.b, colour.a };
    };
    
    entry->setter = [setter](const Colour value)
    {
        setter(glm::vec4(value[0], value[1], value[2], value[3]));
    };

    m_entries.emplace_back(std::move(entry));

    TwAddVarCB(m_tweakbar, GetName().c_str(), TW_TYPE_COLOR4F,
        SetCallback, GetCallback, m_entries[index].get(),
        Definition(label).c_str());

    LogTweakError();
}

void Tweaker::AddEnumEntry(std::string label,
                           std::function<const int(void)> getter,
                           std::function<void(const int)> setter,
                           const std::vector<const char*>& values)
{
    std::vector<TwEnumVal> enumValues;
    for(int i = 0; i < static_cast<int>(values.size()); ++i)
    {
        enumValues.push_back({ i, values[i] });
    }

    TwType enumType = TwDefineEnum(
        ("enum_" + GetName()).c_str(), 
        &enumValues[0], values.size());

    const auto index = m_entries.size();
    auto entry = std::make_unique<TweakableEntry<int>>();
    entry->getter = getter;
    entry->setter = setter;
    m_entries.emplace_back(std::move(entry));

    TwAddVarCB(m_tweakbar, GetName().c_str(), enumType,
        SetCallback, GetCallback, m_entries[index].get(),
        Definition(label).c_str());

    LogTweakError();
}
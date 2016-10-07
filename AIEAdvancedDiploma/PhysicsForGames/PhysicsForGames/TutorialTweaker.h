////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TutorialTweaker.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>
#include <functional>
#include <vector>

class PhysicsScene;
class PhysicsObject;
class SquareBody;
class Plane;
class CircleBody;
class Tweaker;

class TutorialTweaker
{
public:

    /**
    * Constructor
    * @param tweaker The tweak bar to add to
    * @param setInt/setFlt Setters for tutorial data
    * @param getInt/getFlt Getters for tutorial data
    */
    TutorialTweaker(Tweaker& tweaker,
                    std::function<void(const char*, int)> setInt,
                    std::function<void(const char*, float)> setFlt,
                    std::function<int(const char*)> getInt,
                    std::function<float(const char*)> getFlt);

    /**
    * Sets the group for the tweak bar
    */
    void SetGroup(const char* group);

    /**
    * Creates a new button in the tweak bar
    * @param label What the button will display
    * @param callback The function to use when pressed
    */
    void AddButton(std::string label, 
                   std::function<void(void)> callback);

    /**
    * Adds a tweakable float value to the tweak bar
    * @param name The name of the value to add
    * @param label The label to show on the tweak bar
    * @param precision The number of decimal points to show
    * @param onSet Callback to use when value is set
    */
    void AddTweakableFlt(const char* name, 
                         const char* label,
                         float step,
                         int precision,
                         std::function<void(void)> onSet = nullptr);

    /**
    * Adds a tweakable int value to the tweak bar
    * @param name The name of the value to add
    * @param label The label to show on the tweak bar
    * @param onSet Callback to use when value is set
    */
    void AddTweakableInt(const char* name, 
                         const char* label,
                         std::function<void(void)> onSet = nullptr);

    /**
    * Adds a tweakable float value to the tweak bar
    * @param name The name of the value to add
    * @param label The label to show on the tweak bar
    * @param precision The number of decimal points to show
    * @param min/max The min and max values to go to
    * @param onSet Callback to use when value is set
    */
    void AddTweakableFlt(const char* name, 
                         const char* label,
                         float step,
                         float min,
                         float max,
                         int precision,
                         std::function<void(void)> onSet = nullptr);

    /**
    * Adds a tweakable int value to the tweak bar
    * @param name The name of the value to add
    * @param label The label to show on the tweak bar
    * @param min/max The min and max values to go to
    * @param onSet Callback to use when value is set
    */
    void AddTweakableInt(const char* name, 
                         const char* label,
                         int min,
                         int max,
                         std::function<void(void)> onSet = nullptr);

    /**
    * Adds a tweakable bool value to the tweak bar
    * @param name The name of the value to add
    * @param label The label to show on the tweak bar
    * @param onSet Callback to use when value is set
    */
    void AddTweakableBool(const char* name, 
                         const char* label,
                         std::function<void(void)> onSet = nullptr);

    /**
    * Adds a body to the tweak bar
    * @param obj The body to add
    * @param label The label to show on the tweak bar
    * @param onSet Callback to use when value is set
    */
    void AddTweakbleSquare(SquareBody* obj,
                           const std::string& label,
                           std::function<void(void)> onSet = nullptr);

    /**
    * Adds a body to the tweak bar
    * @param obj The body to add
    * @param label The label to show on the tweak bar
    * @param onSet Callback to use when value is set
    */
    void AddTweakbleObject(PhysicsObject* obj,
                           const std::string& label,
                           std::function<void(void)> onSet = nullptr);

    /**
    * Adds a plane to the tweak bar
    * @param obj The body to add
    * @param label The label to show on the tweak bar
    * @param onSet Callback to use when value is set
    */
    void AddTweakblePlane(Plane* obj,
                          const std::string& label,
                          std::function<void(void)> onSet = nullptr);

private:

    std::function<void(const char*, int)> m_setInt = nullptr;
    std::function<void(const char*, float)> m_setFlt = nullptr;
    std::function<int(const char*)> m_getInt = nullptr;
    std::function<float(const char*)> m_getFlt = nullptr;
    Tweaker& m_tweaker;
};
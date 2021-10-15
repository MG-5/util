#pragma once

#include "helpers/freertos.hpp"

namespace util
{
class LedBase
{
public:
    virtual ~LedBase() = default;

    /// Turns on LED, will call the inherited turn on process of derived class.
    virtual void turnOn() final
    {
        ledState = LedState::Normal;
        turnOnInherited();
    }

    /// Turns off LED, will call the inherited turn off process of derived class.
    virtual void turnOff() final
    {
        ledState = LedState::Normal;
        turnOffInherited();
    }

    /// Enables blinking at given frequency.
    /// To turn off blinking simply use turnOn()/turnOff() functions.
    /// \param frequency Blink frequency - 50% of period time will be on/off
    void setBlinking(units::si::Frequency frequency)
    {
        ledState = LedState::Blinking;
        this->blinkFrequency = frequency;
    }

    /// Enables flashing.
    /// It fires two flashs every second.
    /// To turn off flashing simply use turnOn()/turnOff() functions.
    void setFlashing()
    {
        ledState = LedState::Flashing;
    }

    /// Updates the state of LED for blinking/flashing purposes.
    /// Should be called periodically.
    /// \param currentTicks the current system/task tick counter
    void updateState(uint32_t currentTicks)
    {
        switch (ledState)
        {
        case LedState::Blinking:
        {
            bool state = pdMS_TO_TICKS(currentTicks) % toOsTicks(blinkFrequency) /
                         toOsTicks(blinkFrequency) / 2;

            state ? turnOnInherited() : turnOffInherited();
        }
        break;

        case LedState::Flashing:
        {
            auto state = pdMS_TO_TICKS(currentTicks) % pdMS_TO_TICKS(1000) / pdMS_TO_TICKS(100);

            (state == 0 || state == 2) ? turnOnInherited() : turnOffInherited();
        }
        break;

        case LedState::Normal:
        default:
            break;
        }
    }

protected:
    /// Each derived class should implement the turn on process if needed
    virtual void turnOnInherited() = 0;

    /// Each derived class should implement the turn off process if needed
    virtual void turnOffInherited() = 0;

    enum class LedState
    {
        Normal,
        Blinking,
        Flashing
    };

    LedState ledState = LedState::Normal;
    units::si::Frequency blinkFrequency = 1.0_Hz;
};

//--------------------------------------------------------------------------------------------------
class MultiColorLedBase : public LedBase
{
public:
    void turnOnInherited() override
    {
        isOn = true;
        update();
    }

    void turnOffInherited() override
    {
        isOn = false;
        update();
    }

protected:
    virtual void update() = 0;
    bool isOn = false;
};

} // namespace util
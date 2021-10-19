#pragma once

#include "helpers/freertos.hpp"

namespace util
{
class LedBase
{
public:
    virtual ~LedBase() = default;

    /// Turns on LED and resets it's blinking/flash state.
    void turnOn()
    {
        ledState = LedState::Normal;
        isOn = true;
    }

    /// Turns off LED and resets it's blinking/flash state.
    void turnOff()
    {
        ledState = LedState::Normal;
        isOn = false;
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
            isOn = currentTicks % toOsTicks(blinkFrequency) / (toOsTicks(blinkFrequency) / 2);
        }
        break;

        case LedState::Flashing:
        {
            auto state = pdMS_TO_TICKS(currentTicks) % pdMS_TO_TICKS(1000) / pdMS_TO_TICKS(100);
            isOn = (state == 0 || state == 2);
        }
        break;

        case LedState::Normal:
        default:
            break;
        }

        update();
    }

protected:
    enum class LedState
    {
        Normal,
        Blinking,
        Flashing
    };

    LedState ledState = LedState::Normal;
    units::si::Frequency blinkFrequency = 1.0_Hz;

    /// Each derived class should implement the on/off process
    virtual void update() = 0;

    /// Normally used by inherited update functions to get the on/off state of LED object
    bool isOn = false;
};

//--------------------------------------------------------------------------------------------------
template <typename LedColor>
class MultiColorLedBase : public LedBase
{
public:
    /// Set the color of LED without changing its state (on/off etc).
    /// To change the state of any LED object, use the turnOn()/turnOff() functions.
    /// \param ledColor  color at which LED should lights up, if turned on
    /// \param frequency Blink frequency - 50% of period time will be on/off
    void setColor(LedColor ledColor)
    {
        currentColor = ledColor;
    }

    /// Enables blinking with given color at given frequency.
    /// To turn off blinking simply use turnOn()/turnOff() functions.
    /// \param ledColor  color at which LED should blink
    /// \param frequency Blink frequency - 50% of period time will be on/off
    void setColorBlinking(LedColor ledColor, units::si::Frequency frequency)
    {
        currentColor = ledColor;
        setBlinking(frequency);
    }

protected:
    /// Each derived class should implement it own enum class for colors and pass them as template
    /// parameter.
    LedColor currentColor{};
};

} // namespace util
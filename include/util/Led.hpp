#pragma once

#include "helpers/freertos.hpp"
#include "util/PwmOutput.hpp"
#include "util/gpio.hpp"

#include <limits>

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

namespace binary_led
{
//--------------------------------------------------------------------------------------------------
class SingleLed : public LedBase
{
public:
    explicit SingleLed(Gpio gpio) : ledGpio{gpio} {};

    void turnOnInherited() override
    {
        ledGpio.write(Gpio::High);
    }

    void turnOffInherited() override
    {
        ledGpio.write(Gpio::Low);
    }

private:
    Gpio ledGpio;
};

enum class DualLedColor
{
    Red,
    Green,
    Yellow
};

//--------------------------------------------------------------------------------------------------

/// only for red and green LEDs in bundle
class DualLed : public MultiColorLedBase
{
public:
    DualLed(Gpio ledRedGpio, Gpio ledGreenGpio)
        : ledRedGpio{ledRedGpio}, ledGreenGpio{ledGreenGpio} {};

    void setColor(DualLedColor ledColor)
    {
        this->color = ledColor;
        update();
    }

private:
    void update() override
    {
        if (MultiColorLedBase::isOn)
        {
            ledRedGpio.write((color == DualLedColor::Red || color == DualLedColor::Yellow));
            ledGreenGpio.write((color == DualLedColor::Green || color == DualLedColor::Yellow));
        }
        else
        {
            ledRedGpio.write(Gpio::Low);
            ledGreenGpio.write(Gpio::Low);
        }
    }

    Gpio ledRedGpio;
    Gpio ledGreenGpio;
    DualLedColor color = DualLedColor::Red;
};
} // namespace binary_led

namespace pwm_led
{

//--------------------------------------------------------------------------------------------------
template <typename TimerResolution>
class SingleLed : public LedBase
{

public:
    explicit SingleLed(PwmOutput<TimerResolution> pwmOutput) : pwmOutput(pwmOutput){};

    void turnOnInherited() override
    {
        pwmOutput.setMaximumPwm();
    }

    void turnOffInherited() override
    {
        pwmOutput.setPwmValue(0);
    }

private:
    PwmOutput<TimerResolution> pwmOutput;
};

//--------------------------------------------------------------------------------------------------
enum class DualLedColor
{
    Red,
    Green,
    Yellow,
    Orange
};

//--------------------------------------------------------------------------------------------------
template <typename TimerResolution>
class DualLed : public MultiColorLedBase
{
public:
    DualLed(PwmOutput<TimerResolution> ledRedPwmOutput,
            PwmOutput<TimerResolution> ledGreenPwmOutput)
        : ledRedPwmOutput{ledRedPwmOutput}, ledGreenPwmOutput{ledGreenPwmOutput} {};

    void setColor(DualLedColor ledColor)
    {
        this->color = ledColor;
        update();
    }

private:
    void update() override
    {
        if (MultiColorLedBase::isOn)
        {
            switch (color)
            {
            case DualLedColor::Red:
                ledRedPwmOutput.setMaximumPwm();
                ledGreenPwmOutput.setPwmValue(0);
                break;

            case DualLedColor::Yellow:
                ledRedPwmOutput.setMaximumPwm();
                ledGreenPwmOutput.setMaximumPwm();
                break;

            case DualLedColor::Orange:
                ledRedPwmOutput.setMaximumPwm();
                ledGreenPwmOutput.setPwmValue(std::numeric_limits<TimerResolution>::max() / 2);
                break;

            case DualLedColor::Green:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setMaximumPwm();
                break;

            default:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setPwmValue(0);
                break;
            }
        }
        else
        {
            ledRedPwmOutput.setPwmValue(0);
            ledGreenPwmOutput.setPwmValue(0);
        }
    }

    PwmOutput<TimerResolution> ledRedPwmOutput;
    PwmOutput<TimerResolution> ledGreenPwmOutput;
    DualLedColor color = DualLedColor::Red;
};

//--------------------------------------------------------------------------------------------------
enum class TripleLedColor
{
    Red,
    Green,
    Blue,
    Yellow,
    Orange,
    Purple,
    Turquoise
};

//--------------------------------------------------------------------------------------------------
template <typename TimerResolution>
class TripleLed : public MultiColorLedBase
{
public:
    TripleLed(PwmOutput<TimerResolution> ledRedPwmOutput,
              PwmOutput<TimerResolution> ledGreenPwmOutput,
              PwmOutput<TimerResolution> ledBluePwmOutput)
        : ledRedPwmOutput{ledRedPwmOutput}, ledGreenPwmOutput{ledGreenPwmOutput},
          ledBluePwmOutput{ledBluePwmOutput} {};

    void setColor(TripleLedColor ledColor)
    {
        this->color = ledColor;
        update();
    }

private:
    void update() override
    {
        if (MultiColorLedBase::isOn)
        {
            switch (color)
            {
            case TripleLedColor::Red:
                ledRedPwmOutput.setMaximumPwm();
                ledGreenPwmOutput.setPwmValue(0);
                ledBluePwmOutput.setPwmValue(0);
                break;

            case TripleLedColor::Yellow:
                ledRedPwmOutput.setMaximumPwm();
                ledGreenPwmOutput.setMaximumPwm();
                ledBluePwmOutput.setPwmValue(0);
                break;

            case TripleLedColor::Orange:
                ledRedPwmOutput.setMaximumPwm();
                ledGreenPwmOutput.setPwmValue(std::numeric_limits<TimerResolution>::max() / 2);
                ledBluePwmOutput.setPwmValue(0);
                break;

            case TripleLedColor::Green:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setMaximumPwm();
                ledBluePwmOutput.setPwmValue(0);
                break;

            case TripleLedColor::Blue:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setPwmValue(0);
                ledBluePwmOutput.setMaximumPwm();
                break;

            case TripleLedColor::Turquoise:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setMaximumPwm();
                ledBluePwmOutput.setPwmValue(std::numeric_limits<TimerResolution>::max() / 2);
                break;

            case TripleLedColor::Purple:
                ledRedPwmOutput.setPwmValue(std::numeric_limits<TimerResolution>::max());
                ledGreenPwmOutput.setPwmValue(0);
                ledBluePwmOutput.setPwmValue(std::numeric_limits<TimerResolution>::max() / 2);
                break;

            default:
                ledRedPwmOutput.setPwmValue(0);
                ledGreenPwmOutput.setPwmValue(0);
                ledBluePwmOutput.setPwmValue(0);
                break;
            }
        }
        else
        {
            ledRedPwmOutput.setPwmValue(0);
            ledGreenPwmOutput.setPwmValue(0);
            ledBluePwmOutput.setPwmValue(0);
        }
    }

    TripleLedColor color = TripleLedColor::Red;

    PwmOutput<TimerResolution> ledRedPwmOutput;
    PwmOutput<TimerResolution> ledGreenPwmOutput;
    PwmOutput<TimerResolution> ledBluePwmOutput;
};

} // namespace pwm_led

} // namespace util
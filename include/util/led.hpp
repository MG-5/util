#pragma once

#include "util/PwmOutput.hpp"
#include "util/gpio.hpp"

#include <limits>

namespace util
{
class LedBase
{
public:
    virtual ~LedBase() = default;

    void turnOn()
    {
        ledState = LedState::Normal;
        turnOnInherited();
    }

    void turnOff()
    {
        ledState = LedState::Normal;
        turnOffInherited();
    }

    void setBlinking(units::si::Frequency blinkFrequency)
    {
        ledState = LedState::Blinking;
        this->blinkFrequency = blinkFrequency;
    }

    void setFlashing()
    {
        ledState = LedState::Flashing;
        this->blinkFrequency = blinkFrequency;
    }

protected:
    virtual void turnOnInherited() = 0;
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

namespace binary
{
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
} // namespace binary

namespace pwm
{
template <typename TimerResolution>
class SingleLed : public LedBase
{

public:
    explicit SingleLed(PwmOutput<TimerResolution> pwmOutput) : pwmOutput(pwmOutput){};

    void turnOnInherited() override
    {
        pwmOutput.setMaximumPwm();
    }

    void turnOff() override
    {
        pwmOutput.setPwmValue(0);
    }

private:
    PwmOutput<TimerResolution> pwmOutput;
};

enum class DualLedColor
{
    Red,
    Green,
    Yellow,
    Orange
};

template <typename TimerResolution>
class DualLed : public MultiColorLedBase
{
public:
    DualLed(PwmOutput<TimerResolution> ledRedPwmOutput,
            PwmOutput<TimerResolution> ledGreenPwmOutput)
        : ledRedPwmOutput{ledRedPwmOutput}, ledGreenPwmOutput{ledGreenPwmOutput} {};

    void setColor(DualLedColor ledColor) override
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

template <typename TimerResolution>
class TripleLed : public MultiColorLedBase
{
public:
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
                ledBluePwmOutput.setMaximumPwm(std::numeric_limits<TimerResolution>::max() / 2);
                break;

            case TripleLedColor::Purple:
                ledRedPwmOutput.setPwmValue(std::numeric_limits<TimerResolution>::max());
                ledGreenPwmOutput.setPwmValue(0);
                ledBluePwmOutput.setMaximumPwm(std::numeric_limits<TimerResolution>::max() / 2);
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

} // namespace pwm

} // namespace util

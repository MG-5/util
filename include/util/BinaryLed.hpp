#pragma once
#include "LedBase.hpp"
#include "util/gpio.hpp"

namespace util::binary_led
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
} // namespace util::binary_led
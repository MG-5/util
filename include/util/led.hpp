#pragma once

#include "util/gpio.hpp"

namespace util
{
class ILed
{
public:
    virtual ~ILed() = default;

    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
};

class UniColorLed : public ILed
{
public:
    UniColorLed(Gpio gpio) : gpio{gpio}
    {
    }

    void turnOn() override
    {
        gpio.write(Gpio::High);
    }

    void turnOff() override
    {
        gpio.write(Gpio::Low);
    }

private:
    Gpio gpio;
};

enum class LedColor
{
    Off,
    Red,
    Green,
    Yellow
};

class BiColorLed : public ILed
{
public:
    BiColorLed(Gpio led1, Gpio led2) : led1{led1}, led2{led2}
    {
    }

    void turnOn() override
    {
        isOn = true;
        update();
    }

    void turnOff() override
    {
        isOn = false;
        update();
    }

    void setColor(LedColor color)
    {
        this->color = color;
        update();
    }

private:
    void update()
    {
        if (isOn)
        {
            led1.write((color == LedColor::Red || color == LedColor::Yellow) ? Gpio::High
                                                                             : Gpio::Low);
            led2.write((color == LedColor::Green || color == LedColor::Yellow) ? Gpio::High
                                                                               : Gpio::Low);
        }
        else
        {
            led1.write(Gpio::Low);
            led2.write(Gpio::Low);
        }
    }

    Gpio led1;
    Gpio led2;
    LedColor color;
    bool isOn{true};
};
} // namespace util

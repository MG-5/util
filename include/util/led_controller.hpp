#pragma once

#include "util/led_effect.hpp"

namespace util
{
class LedController : public ILed
{
public:
    LedController(ILed &led) : led{led}
    {
    }

    void turnOn() override
    {
        removeEffect();
        led.turnOn();
    }

    void turnOff() override
    {
        removeEffect();
        led.turnOff();
    }

    void update()
    {
        if (effect == nullptr)
            return;

        if (effect->getState())
            led.turnOn();
        else
            led.turnOff();
    }

    void setEffect(const ILedEffect &effect)
    {
        this->effect = &effect;
    }

    void removeEffect()
    {
        effect = nullptr;
    }

private:
    ILed &led;
    const ILedEffect *effect{nullptr};
};
} // namespace util

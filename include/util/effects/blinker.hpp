#pragma once

#include "util/led_effect.hpp"

namespace util::effects
{
class Blinker : public util::ILedEffect
{
public:
    Blinker() = default;

    void update() override
    {
        state = !state;
    }

    bool getState() const override
    {
        return state;
    }

private:
    bool state = false;
};
} // namespace util::effects

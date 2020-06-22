#pragma once

#include "util/led.hpp"

namespace util
{
class ILedEffect
{
public:
    virtual ~ILedEffect() = default;

    virtual void update() = 0;
    virtual bool getState() const = 0;
};
} // namespace util

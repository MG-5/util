#pragma once

#include "hal_header.h"
#include <cstdint>

namespace util
{
class Gpio
{
public:
    using State = bool;
    static constexpr State Low = false;
    static constexpr State High = true;

public:
    Gpio(GPIO_TypeDef *port, uint16_t pin) : port{port}, pin{pin}
    {
    }

    State read()
    {
        return HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET ? High : Low;
    }

    void write(State state)
    {
        HAL_GPIO_WritePin(port, pin, state == Low ? GPIO_PIN_RESET : GPIO_PIN_SET);
    }

private:
    GPIO_TypeDef *const port;
    const uint16_t pin;
};
} // namespace util
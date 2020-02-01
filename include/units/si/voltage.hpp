#pragma once

#include "units/si.hpp"

namespace units::si
{
using Voltage = Value<SiUnit<2, 1, -3, -1, 0, 0, 0>>;
} // namespace units::si

constexpr units::si::Voltage operator""_V(const long double magnitude)
{
    return units::si::Voltage(magnitude);
}

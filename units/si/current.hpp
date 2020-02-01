#pragma once

#include "units/si.hpp"

namespace units::si
{
using Current = Value<SiUnit<0, 0, 0, 1, 0, 0, 0>>;
} // namespace units::si

constexpr units::si::Current operator""_A(const long double magnitude)
{
    return units::si::Current(magnitude);
}

#pragma once

#include "units/si.hpp"

namespace
{
constexpr auto KilometresPerHourFactor = 3.6f;
}

namespace units::si
{
using Velocity = Value<SiUnit<1, 0, -1, 0, 0, 0, 0>>;
} // namespace units::si

constexpr units::si::Velocity operator"" _m_s(const long double magnitude)
{
    return units::si::Velocity(magnitude);
}

constexpr units::si::Velocity operator"" _km_h(const long double magnitude)
{
    return units::si::Velocity(magnitude / KilometresPerHourFactor);
}

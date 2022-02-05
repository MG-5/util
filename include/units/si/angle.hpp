#pragma once

#include "units/si.hpp"
#include <cmath>

namespace units::si
{
using Angle = Value<SiUnit<0, 0, 0, 0, 0, 0, 0>>;
} // namespace units::si

namespace
{
constexpr auto DegreesFactor = M_PIf32 / 180.0f;
} // namespace

namespace units::si::scale
{
constexpr auto deg = Scale<Angle::Unit>{1.0f / DegreesFactor};
} // namespace units::si::scale

constexpr units::si::Angle operator"" _rad(const long double magnitude)
{
    return units::si::Angle(magnitude);
}

constexpr units::si::Angle operator"" _deg(const long double magnitude)
{
    return units::si::Angle(magnitude * DegreesFactor);
}

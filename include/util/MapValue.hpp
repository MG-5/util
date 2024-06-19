#pragma once

#include <algorithm>
#include <core/SafeAssert.h>

namespace util
{
template <typename InputType, typename OutputType>
inline OutputType mapValue(const InputType fromMin, const InputType fromMax, const OutputType toMin,
                           const OutputType toMax, const InputType value)
{
    // excluding everything not unit tested and not needed with devices
    SafeAssert(fromMin < fromMax);
    SafeAssert(toMin < toMax);
    SafeAssert(!(fromMax == 0 && fromMin < 0));
    SafeAssert(!(toMax == 0 && toMin < 0));

    // Clamp the input value to the from range
    const InputType clampedValue = (value < fromMin)   ? fromMin
                                   : (value > fromMax) ? fromMax
                                                       : value;

    // Calculate the scaling factor
    const auto fromRange = fromMax - fromMin;
    const auto toRange = toMax - toMin;
    const auto scaledValue = (clampedValue - fromMin) * toRange / fromRange;

    // Compute the mapped value
    return toMin + scaledValue;
}
} // namespace util
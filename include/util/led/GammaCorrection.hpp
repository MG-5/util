#pragma once

#include "gcem.hpp"
#include <array>
#include <cmath>

namespace util::led::pwm
{

template <size_t NumberOfResolutionBits, float StartOffset = 1.0f>
class GammaCorrection
{
private:
    static constexpr auto GammaFactor = 2.2f;

public:
    static constexpr size_t MaxResolutionValue = (1 << NumberOfResolutionBits) - 1;
    using GammaTable = std::array<size_t, MaxResolutionValue + 1>; // +1 for zero

    static constexpr GammaTable createGammaTable()
    {
        GammaTable gammaTable{};

        for (size_t i = 1; i < MaxResolutionValue + 1; i++)
        {
            const auto Logarithm = gcem::pow(
                static_cast<float>(i) / static_cast<float>(MaxResolutionValue), GammaFactor);

            gammaTable[i] =
                StartOffset + gcem::round(Logarithm * (MaxResolutionValue - StartOffset));
        }

        return gammaTable;
    }

    static constexpr GammaTable LookUpTable = createGammaTable();
};
} // namespace util::led::pwm
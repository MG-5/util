#pragma once

#include "gcem.hpp"
#include <array>
#include <cmath>

namespace util::led::pwm
{

template <size_t NumberOfResolutionBits>
class GammaCorrection
{
private:
    static constexpr auto StartOffset = 1.0f;
    static constexpr auto GammaFactor = 2.2f;

public:
    static constexpr auto MaxResolutionValue = (1 << NumberOfResolutionBits) - 1;
    using GammaTable = std::array<size_t, MaxResolutionValue + 1>; // +1 for zero

    static constexpr GammaTable createGammaTable()
    {
        GammaTable gammaTable{};

        for (auto i = 1; i < MaxResolutionValue + 1; i++)
        {
            const auto Logarithm = gcem::pow(
                static_cast<float>(i) / static_cast<float>(MaxResolutionValue), GammaFactor);

            gammaTable[i] =
                StartOffset + gcem::round(Logarithm * (MaxResolutionValue - StartOffset));
        }

        return gammaTable;
    }

    static constexpr GammaTable GammaCorrectionLUT = createGammaTable();
};
} // namespace util::led::pwm
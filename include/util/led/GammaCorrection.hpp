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
    static constexpr auto NumberOfEntries = (1 << NumberOfResolutionBits) - 1;

public:
    using GammaTable = std::array<size_t, NumberOfEntries + 1>;

    static constexpr GammaTable createGammaTable()
    {
        GammaTable gammaTable{};

        for (auto i = 1; i < NumberOfEntries + 1; i++)
        {
            const auto Logarithm =
                gcem::pow(static_cast<float>(i) / static_cast<float>(NumberOfEntries), GammaFactor);

            gammaTable[i] = StartOffset + gcem::round(Logarithm * (NumberOfEntries - StartOffset));
        }

        return gammaTable;
    }

    static constexpr GammaTable GammaCorrectionLUT = createGammaTable();
};
} // namespace util::led::pwm
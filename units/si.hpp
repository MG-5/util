#pragma once

#include <cstdint>

namespace units
{
template <int M, int Kg, int S, int A, int K, int Mo, int C>
struct SiUnit
{
    enum
    {
        metre = M,
        kilogram = Kg,
        second = S,
        ampere = A,
        kelvin = K,
        mole = Mo,
        candela = C
    };
};

template <typename SiUnit>
class Value
{
private:
    float magnitude{0};

public:
    constexpr explicit Value(const float magnitude) noexcept
        : magnitude{magnitude}
    {
    }

    constexpr float getMagnitude() const noexcept
    {
        return magnitude;
    }
};

template <int M, int Kg, int S, int A, int K, int Mo, int C>
constexpr Value<SiUnit<M, Kg, S, A, K, Mo, C>>
operator-(const Value<SiUnit<M, Kg, S, A, K, Mo, C>> &op) noexcept
{
    return Value<SiUnit<M, Kg, S, A, K, Mo, C>>(-op.getMagnitude());
}

template <int M, int Kg, int S, int A, int K, int Mo, int C>
constexpr Value<SiUnit<M, Kg, S, A, K, Mo, C>>
operator+(const Value<SiUnit<M, Kg, S, A, K, Mo, C>> &lhs,
          const Value<SiUnit<M, Kg, S, A, K, Mo, C>> &rhs) noexcept
{
    return Value<SiUnit<M, Kg, S, A, K, Mo, C>>(lhs.getMagnitude() + rhs.getMagnitude());
}

template <int M, int Kg, int S, int A, int K, int Mo, int C>
constexpr Value<SiUnit<M, Kg, S, A, K, Mo, C>>
operator-(const Value<SiUnit<M, Kg, S, A, K, Mo, C>> &lhs,
          const Value<SiUnit<M, Kg, S, A, K, Mo, C>> &rhs) noexcept
{
    return Value<SiUnit<M, Kg, S, A, K, Mo, C>>(lhs.getMagnitude() - rhs.getMagnitude());
}

template <int M1, int Kg1, int S1, int A1, int K1, int Mo1, int C1, int M2, int Kg2, int S2, int A2,
          int K2, int Mo2, int C2>
constexpr Value<SiUnit<M1 + M2, Kg1 + Kg2, S1 + S2, A1 + A2, K1 + K2, Mo1 + Mo2, C1 + C2>>
operator*(const Value<SiUnit<M1, Kg1, S1, A1, K1, Mo1, C1>> &lhs,
          const Value<SiUnit<M2, Kg2, S2, A2, K2, Mo2, C2>> &rhs) noexcept
{
    return Value<SiUnit<M1 + M2, Kg1 + Kg2, S1 + S2, A1 + A2, K1 + K2, Mo1 + Mo2, C1 + C2>>(
        lhs.getMagnitude() * rhs.getMagnitude());
}

template <int M1, int Kg1, int S1, int A1, int K1, int Mo1, int C1, int M2, int Kg2, int S2, int A2,
          int K2, int Mo2, int C2>
constexpr Value<SiUnit<M1 - M2, Kg1 - Kg2, S1 - S2, A1 - A2, K1 - K2, Mo1 - Mo2, C1 - C2>>
operator/(const Value<SiUnit<M1, Kg1, S1, A1, K1, Mo1, C1>> &lhs,
          const Value<SiUnit<M2, Kg2, S2, A2, K2, Mo2, C2>> &rhs) noexcept
{
    return Value<SiUnit<M1 - M2, Kg1 - Kg2, S1 - S2, A1 - A2, K1 - K2, Mo1 - Mo2, C1 - C2>>(
        lhs.getMagnitude() / rhs.getMagnitude());
}
} // namespace units

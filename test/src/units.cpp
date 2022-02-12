#include <gtest/gtest.h>
#include <units/si/scalar.hpp>
#include <numbers>

using namespace units::si;

TEST(units, abs)
{
    EXPECT_FLOAT_EQ((1.0_).abs(), (-1.0_).abs());
    EXPECT_FLOAT_EQ((1.0_).abs(), (1.0_).abs());

    EXPECT_FLOAT_EQ(units::abs(1.0_).getMagnitude(), units::abs(-1.0_).getMagnitude());
    EXPECT_FLOAT_EQ(units::abs(1.0_).getMagnitude(), units::abs(1.0_).getMagnitude());
}

TEST(units, min)
{
    static constexpr auto Small = 1.0_;
    static constexpr auto Negative = -1.0_;
    static constexpr auto Big = 10.0_;

    EXPECT_FLOAT_EQ(units::min(Small, Big).getMagnitude(), Small.getMagnitude());
    EXPECT_FLOAT_EQ(units::min(Big, Small).getMagnitude(), Small.getMagnitude());
    EXPECT_FLOAT_EQ(units::min(Small, Negative).getMagnitude(), Negative.getMagnitude());
    EXPECT_FLOAT_EQ(units::min(Negative, Small).getMagnitude(), Negative.getMagnitude());
    EXPECT_FLOAT_EQ(units::min(Negative, Big).getMagnitude(), Negative.getMagnitude());
}

TEST(units, max)
{
    static constexpr auto Small = 1.0_;
    static constexpr auto Negative = -1.0_;
    static constexpr auto Big = 10.0_;

    EXPECT_FLOAT_EQ(units::max(Small, Big).getMagnitude(), Big.getMagnitude());
    EXPECT_FLOAT_EQ(units::max(Big, Small).getMagnitude(), Big.getMagnitude());
    EXPECT_FLOAT_EQ(units::max(Small, Negative).getMagnitude(), Small.getMagnitude());
    EXPECT_FLOAT_EQ(units::max(Negative, Small).getMagnitude(), Small.getMagnitude());
    EXPECT_FLOAT_EQ(units::max(Negative, Big).getMagnitude(), Big.getMagnitude());
}
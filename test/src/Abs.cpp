#include <gtest/gtest.h>
#include <units/si/scalar.hpp>
#include <numbers>

using namespace units::si;

TEST(units_si, abs)
{
    EXPECT_FLOAT_EQ((1.0_).abs(), (-1.0_).abs() );
    EXPECT_FLOAT_EQ((1.0_).abs(), (1.0_).abs() );
}
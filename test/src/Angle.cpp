#include <gtest/gtest.h>
#include <units/si/angle.hpp>

using namespace units::si;

TEST(units_angle, scalar)
{
    EXPECT_FLOAT_EQ((90.0_deg).getMagnitude(), Angle (M_PIf32/2).getMagnitude() );
    EXPECT_FLOAT_EQ((90.0_deg).getMagnitude(), M_PIf32/2.0 );
    EXPECT_FLOAT_EQ((90.0_deg).getMagnitude(scale::rad), Angle(M_PIf32/2.0).getMagnitude(scale::rad));
}
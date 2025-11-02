#include "../operators/conditions.h"
#include "../metadata/cell.h"
#include <gtest/gtest.h>

namespace DBCPP_Operators
{
    TEST(ConditionsTest, EqualsIntWorks) {
        DataRow row {{{1},{"ala"}}};
        Equals<int> eqOperartor{ 1,0 };
        EXPECT_TRUE(eqOperartor.IsMatch(row));
        Equals<int> eqOperartor2{ 2,0 };
        EXPECT_FALSE(eqOperartor2.IsMatch(row));
    }

    TEST(ConditionsTest, EqualsStringWorks) {
        DataRow row {{{1},{"ala"}}};
        Equals<std::string> eqOperartor{ "ala",1 };
        EXPECT_TRUE(eqOperartor.IsMatch(row));
        Equals<std::string> eqOperartor2{ "ola",1 };
        EXPECT_FALSE(eqOperartor2.IsMatch(row));
    }
}
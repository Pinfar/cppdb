// #include "../operators/conditions.h"
// #include "../metadata/cell.h"
// #include <gtest/gtest.h>
// #include <memory>
// #include <vector>

// namespace DBCPP_Operators
// {
//     TEST(ConditionsTest, EqualsIntWorks) {
//         std::unique_ptr<DataRow> row {new DataRow{{{1},{"ala"}}}};
//         Equals<int> eqOperartor{ 1,0 };
//         EXPECT_TRUE(eqOperartor.IsMatch(row));
//         Equals<int> eqOperartor2{ 2,0 };
//         EXPECT_FALSE(eqOperartor2.IsMatch(row));
//     }

//     TEST(ConditionsTest, EqualsStringWorks) {
//         std::unique_ptr<DataRow> row {new DataRow{{{1},{"ala"}}}};
//         Equals<std::string> eqOperartor{ "ala",1 };
//         EXPECT_TRUE(eqOperartor.IsMatch(row));
//         Equals<std::string> eqOperartor2{ "ola",1 };
//         EXPECT_FALSE(eqOperartor2.IsMatch(row));
//     }
// }
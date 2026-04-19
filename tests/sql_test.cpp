#include <gtest/gtest.h>
#include "../utils/test_dbengine.h"

namespace DBCPP::SqlTest{
    static void RunTest(std::string sql, std::string expectedOutput)
    {
        DBCPP::Utils::TestDbEngine engine;
        std::string result = engine.RunSql(sql);
        EXPECT_EQ(result,  expectedOutput);
    }

    TEST(SqlTest, SqlCanBeExecuted) 
    {
        RunTest("select Column2,Column1 from Table1 where Column1=21","Column2;Column1\nG;21\n");
    }

    TEST(SqlTest, SqlWithProjection) 
    {
        RunTest("select Column1,Column2 from Table1 where Column1=21","Column1;Column2\n21;G\n");
    }

    TEST(SqlTest, SqlWithStringCondition) 
    {
        RunTest("select Column1,Column2 from Table1 where Column2='C'","Column1;Column2\n4;C\n");
    }
}
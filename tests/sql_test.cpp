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
}
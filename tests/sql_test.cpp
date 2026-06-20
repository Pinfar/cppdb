#include <gtest/gtest.h>
#include "../utils/test_dbengine.h"

namespace DBCPP::SqlTest{
    static void RunTest(std::string sql, const std::string& expectedOutput)
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

    TEST(SqlTest, SqlWithStringConditionReversed) 
    {
        RunTest("select Column1,Column2 from Table1 where 'C'=Column2","Column1;Column2\n4;C\n");
    }

    TEST(SqlTest, SqlWithGreaterThan) 
    {
        RunTest("select Column1,Column2 from Table1 where Column1>22","Column1;Column2\n24;I\n");
    }

    TEST(SqlTest, SqlWithLessThan) 
    {
        RunTest("select Column1,Column2 from Table1 where Column1<2","Column1;Column2\n1;A\n");
    }

    TEST(SqlTest, SqlWithAnd) 
    {
        RunTest("select Column1,Column2 from Table1 where Column1>21 and Column1<24","Column1;Column2\n22;H\n");
    }

    TEST(SqlTest, SqlWithOr) 
    {
        RunTest("select Column1,Column2 from Table1 where Column1>21 and Column1<24 or Column1=1","Column1;Column2\n1;A\n22;H\n");
    }
    
    TEST(SqlTest, SqlWithOr2) 
    {
        RunTest("select Column1,Column2 from Table1 where Column1=1 or Column1>21 and Column1<24","Column1;Column2\n1;A\n22;H\n");
    }
}
#include "../operators/conditions.h"
#include "../operators/where_operator.h"
#include "../operators/projection_operator.h"
#include "../operators/full_scan.h"
#include "../metadata/cell.h"
#include "../utils/formatter.h"
#include "../storage/dbwriter.h"
#include "../storage/dbreader.h"
#include <gtest/gtest.h>
#include <memory>
#include "../operators/execution_plan.h"
#include "../sql_interface/scanner.h"
#include "../sql_interface/parser.h"
#include "../query_engine/compiler.h"
#include "../utils/test_dbengine.h"

namespace DBCPP_Operators
{
    TEST(WhereOperatorTest, IteratingWithWhereWorks) 
    {
        DBCPP::Utils::TestDbEngine engine;
        auto scan = std::make_unique<FullScanOperator>(engine.GetStorage(), "Table1");
        auto condition = std::make_unique<Equals<int>>(21, 0);
        WhereOperator whereOperator{
            std::move(scan),
            std::move(condition)
        };    
        auto& tableData = engine.GetStorage()->getDatabaseHeaderPage().tables[0].definition;
        std::string result = GetSerializedOpearatorOutput(whereOperator, tableData);
        EXPECT_EQ(result,  "Column1;Column2\n21;G\n");
    }

    TEST(WhereOperatorTest, IteratingWithProjectionWorks) 
    {
        DBCPP::Utils::TestDbEngine engine;
        auto scan = std::make_unique<FullScanOperator>(engine.GetStorage(), "Table1");
        auto condition = std::make_unique<Equals<int>>(21, 0);
        auto whereOperator = std::make_unique<WhereOperator>(
            std::move(scan),
            std::move(condition)
        );  
        auto projectionOperator = std::make_unique<ProjectionOperator>(
            std::move(whereOperator),
            ProjectionDefinition{{1,0}}
        );  
        auto tableData = projectionOperator->GetMetadata();
        std::string result = GetSerializedOpearatorOutput(*projectionOperator, tableData);
        EXPECT_EQ(result,  "Column2;Column1\nG;21\n");
        
        projectionOperator->Reset();
        std::string result2 = GetSerializedOpearatorOutput(*projectionOperator, tableData);
        EXPECT_EQ(result2,  "Column2;Column1\nG;21\n");
    }

    TEST(WhereOperatorTest, CreatingExecutionPlanWorks) 
    {
        DBCPP::Utils::TestDbEngine engine;

        auto scan = std::unique_ptr<PlanNode>( new PlanNode{
            DbOperator::TableAccess,
            { std::string("Table1")},
            {},
            {}
        }); 
        auto where = std::unique_ptr<PlanNode>( new PlanNode{
            DbOperator::Filter,
            {ConditionDefinition{ConditionType::Eq,ColumnType::Int, 0, 21, -1 }},
            std::move(scan),
            {}
        });         
        auto projectionOperator = std::unique_ptr<PlanNode>( new PlanNode{
            DbOperator::Projection,
            {ProjectionDefinition{{1,0}}},
            std::move(where),
            {}
        }); 
        ExecutionPlan executionPlan {
            std::move(projectionOperator)
        };
        DBCPP_Storage::DBReader reader {engine.GetStorage()};
        auto execution = reader.ExecutePlan(std::move(executionPlan));
          
        auto tableData = execution->GetMetadata();
        std::string result = GetSerializedOpearatorOutput(*execution, tableData);
        EXPECT_EQ(result,  "Column2;Column1\nG;21\n");
        
        execution->Reset();
        std::string result2 = GetSerializedOpearatorOutput(*execution, tableData);
        EXPECT_EQ(result2,  "Column2;Column1\nG;21\n");
    }
}
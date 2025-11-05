#include "../operators/conditions.h"
#include "../operators/where_operator.h"
#include "../operators/projection_operator.h"
#include "../operators/full_scan.h"
#include "../metadata/cell.h"
#include "../utils/formatter.h"
#include "../storage/dbwriter.h"
#include <gtest/gtest.h>
#include <memory>

namespace DBCPP_Operators
{
    DataRow CreateRow(int value1, std::string value2){
        return DataRow{
                std::vector<DataCell>{{value1},{value2}}
            };;
    }

    std::unique_ptr<StorageEngine> InitStorage(){
        std::unique_ptr<StorageEngine> engine{new StorageEngine({},{})};
        DBCPP_Storage::DBWriter writer {*engine, {3}};
        writer.CreateTable(TableDefinition{
            "Table1",
            {
                Column {
                    "Column1",
                    ColumnType::Int
                },
                Column {
                    "Column2",
                    ColumnType::String
                },
            }
        });
        std::vector numbers {1,2,4,11,12,14,21,22,24};
        std::vector letters {"A","B","C","D","E","F","G","H","I"};
        for(int i=0;i<numbers.size();i++){
            writer.InsertRecord("Table1", CreateRow(numbers[i], letters[i]));
        }
        return std::move(engine);
    }

    TEST(WhereOperatorTest, IteratingWithWhereWorks) 
    {
        auto engine = InitStorage();
        auto scan = std::make_unique<FullScanOperator>(engine.get(), "Table1");
        auto condition = std::make_unique<Equals<int>>(21, 0);
        WhereOperator whereOperator{
            std::move(scan),
            std::move(condition)
        };    
        auto& tableData = engine->getDatabaseHeaderPage().tables[0].definition;
        std::string result = GetSerializedOpearatorOutput(whereOperator, tableData);
        EXPECT_EQ(result,  "Column1;Column2\n21;G\n");
    }

    TEST(WhereOperatorTest, IteratingWithProjectionWorks) 
    {
        auto engine = InitStorage();
        auto scan = std::make_unique<FullScanOperator>(engine.get(), "Table1");
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
}
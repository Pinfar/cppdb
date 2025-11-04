#include "../operators/conditions.h"
#include "../operators/where_operator.h"
#include "../operators/projection_operator.h"
#include "../operators/full_scan.h"
#include "../metadata/cell.h"
#include "../utils/formatter.h"
#include <gtest/gtest.h>
#include <memory>

namespace DBCPP_Operators
{
    DataRow CreateRow(int value1, std::string value2){
        return DataRow{
                std::vector<DataCell>{{value1},{value2}}
            };;
    }

    StorageEngine InitStorage(){
        return StorageEngine(DataBaseHeaderPage{{TableHeader{
            2,
            TableDefinition{
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
            },
            {4}
        }}},
        std::vector<DataPage>{
            DataPage{
                4,
                {CreateRow(11, "A"),CreateRow(12, "B"),CreateRow(14, "C")},
                5
            },
            DataPage{
                5,
                {CreateRow(11, "D"),CreateRow(12, "E"),CreateRow(14, "F")},
                10
            },
            DataPage{
                10,
                {CreateRow(21, "G"),CreateRow(22, "H"),CreateRow(24, "I")},
                -1
            }
        });
    }

    TEST(WhereOperatorTest, IteratingWithWhereWorks) 
    {
        StorageEngine engine = InitStorage();
        auto scan = std::make_unique<FullScanOperator>(engine, "Table1");
        auto condition = std::make_unique<Equals<int>>(21, 0);
        WhereOperator whereOperator{
            std::move(scan),
            std::move(condition)
        };    
        auto& tableData = engine.getDatabaseHeaderPage().tables[0].definition;
        std::string result = GetSerializedOpearatorOutput(whereOperator, tableData);
        EXPECT_EQ(result,  "Column1;Column2\n21;G\n");
    }

    TEST(WhereOperatorTest, IteratingWithProjectionWorks) 
    {
        StorageEngine engine = InitStorage();
        auto scan = std::make_unique<FullScanOperator>(engine, "Table1");
        auto condition = std::make_unique<Equals<int>>(21, 0);
        auto whereOperator = std::make_unique<WhereOperator>(
            std::move(scan),
            std::move(condition)
        );  
        auto projectionOperator = std::make_unique<ProjectionOperator>(
            std::move(whereOperator),
            ProjectionDefinition{{1,0}}
        );  
        auto& tableData = engine.getDatabaseHeaderPage().tables[0].definition;
        std::string result = GetSerializedOpearatorOutput(*projectionOperator, tableData);
        EXPECT_EQ(result,  "Column1;Column2\nG;21\n");
    }
}
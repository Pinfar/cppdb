#include <gtest/gtest.h>
#include "../storage/storage.h"
#include "../utils/serialization.h"
#include "../utils/formatter.h"
#include "../operators/full_scan.h"
namespace FullScanTestsNS
{
    using namespace DBCPP_Operators;

    DataRow CreateRow(int value){
        return DataRow{{{value}}};
    }

    StorageEngine InitStorage(){
        return StorageEngine(TableHeader{
            2,
            TableDefinition{
                "Table1",
                {
                    Column {
                        "Column1",
                        ColumnType::Int
                    },
                }
            },
            {4}
        },
        std::vector<DataPage>{
            DataPage{
                4,
                {CreateRow(1),CreateRow(2),CreateRow(4)},
                5
            },
            DataPage{
                5,
                {CreateRow(11),CreateRow(12),CreateRow(14)},
                10
            },
            DataPage{
                10,
                {CreateRow(21),CreateRow(22),CreateRow(24)},
                -1
            }
        });
    }


    TEST(FullScanTests, StorageInitializedCorrectly) {
    StorageEngine engine = InitStorage();
    EXPECT_EQ(engine.getDatabaseHeaderPage().tables.size(), 1);
    }

    void NextElementIs(FullScanOperator& scan, TableDefinition& table, std::string expectedValue){
        ASSERT_TRUE(scan.Next());
        auto& current = scan.Current();
        auto row = GetSerializedRow(current, table);
        ASSERT_EQ(row, expectedValue);
    }

    TEST(FullScanTests, FullScanIteratesOverAllRecords) {
        StorageEngine engine = InitStorage();
        FullScanOperator scan {
            engine, "Table1"
        };
        auto& tableData = engine.getDatabaseHeaderPage().tables[0].definition;
        NextElementIs(scan, tableData, "1");
        NextElementIs(scan, tableData, "2");
        NextElementIs(scan, tableData, "4");
        NextElementIs(scan, tableData, "11");
        NextElementIs(scan, tableData, "12");
        NextElementIs(scan, tableData, "14");
        NextElementIs(scan, tableData, "21");
        NextElementIs(scan, tableData, "22");
        NextElementIs(scan, tableData, "24");
        ASSERT_FALSE(scan.Next());
    }

    

    TEST(FullScanTests, OperatorOutputIsPrintedCorrectly) {
        StorageEngine engine = InitStorage();
        FullScanOperator scan {
            engine, "Table1"
        };
        auto& tableData = engine.getDatabaseHeaderPage().tables[0].definition;
        auto serialized = GetSerializedOpearatorOutput(scan, tableData);
        EXPECT_EQ(serialized,  "Column1\n1\n2\n4\n11\n12\n14\n21\n22\n24\n");
    }
}
#include "../operators/full_scan.h"
#include "../storage/dbwriter.h"
#include "../storage/storage.h"
#include "../utils/formatter.h"
#include "../utils/serialization.h"
#include "metadata/cell.h"
#include <gtest/gtest.h>
namespace FullScanTestsNS
{
DataRow CreateRow(int value)
{
    return {std::vector<DataCell>{{value}}};
}

std::unique_ptr<StorageEngine> InitStorage()
{
    std::unique_ptr<StorageEngine> engine{new StorageEngine({}, {})};
    DBCPP_Storage::DBWriter writer{*engine, {3}};
    writer.CreateTable(TableDefinition{"Table1",
                                       {
                                           Column{"Column1", ColumnType::Int},
                                       }});
    std::vector numbers{1, 2, 4, 11, 12, 14, 21, 22, 24};
    for (auto i : numbers)
    {
        writer.InsertRecord("Table1", CreateRow(i));
    }
    return std::move(engine);
}

TEST(FullScanTests, StorageInitializedCorrectly)
{
    auto engine = InitStorage();
    EXPECT_EQ(engine->getDatabaseHeaderPage().tables.size(), 1);
}

void NextElementIs(DBCPP_Operators::FullScanOperator &scan, const std::string &expectedValue)
{
    ASSERT_TRUE(scan.Next());
    auto current = scan.Current();
    auto row = GetSerializedRow(current);
    ASSERT_EQ(row, expectedValue);
}

TEST(FullScanTests, FullScanIteratesOverAllRecords)
{
    auto engine = InitStorage();
    DBCPP_Operators::FullScanOperator scan{engine.get(), "Table1"};
    auto &tableData = engine->getDatabaseHeaderPage().tables[0].definition;
    NextElementIs(scan, "1");
    NextElementIs(scan, "2");
    NextElementIs(scan, "4");
    NextElementIs(scan, "11");
    NextElementIs(scan, "12");
    NextElementIs(scan, "14");
    NextElementIs(scan, "21");
    NextElementIs(scan, "22");
    NextElementIs(scan, "24");
    ASSERT_FALSE(scan.Next());
}

TEST(FullScanTests, OperatorOutputIsPrintedCorrectly)
{
    auto engine = InitStorage();
    DBCPP_Operators::FullScanOperator scan{engine.get(), "Table1"};
    std::vector<Column> tableData{{"Column1"}};
    auto serialized = GetSerializedOpearatorOutput(scan, tableData);
    EXPECT_EQ(serialized, "Column1\n1\n2\n4\n11\n12\n14\n21\n22\n24\n");
}
} // namespace FullScanTestsNS
#include <gtest/gtest.h>
#include "../utils/formatter.h"
#include "../metadata/cell.h"
#include "../utils/serialization.h"

TEST(FormatterTests, CellConvertedToStringCorrectly) {

    DataCell sCell {std::string("koteczek")};
    auto sCellResult = GetStringValue(sCell);
    EXPECT_EQ(sCellResult,  std::string("koteczek"));

    DataCell iCell {2141628429};
    auto iCellResult = GetStringValue(iCell);
    EXPECT_EQ(iCellResult,  "2141628429");
}

TableDefinition GetSampleTable(){
    return TableDefinition {
        "Table1",
        {{"Column1", ColumnType::String}, {"Column2", ColumnType::Int}}
    };
}

TEST(FormatterTests, RowIsPrintedCorrectly) {
    DataRow drow {std::vector<DataCell>{DataCell{std::string("koteczek")},DataCell{2141628429}}};
    auto row  = std::make_unique<DataRow>(drow);
    auto serialized = GetSerializedRow(row);
    EXPECT_EQ(serialized,  "koteczek;2141628429");
}

TEST(FormatterTests, HeaderIsPrintedCorrectly) {
    auto table = GetSampleTable();
    auto serialized = GetSerializedHeader(table);
    EXPECT_EQ(serialized,  "Column1;Column2");
}

TEST(FormatterTests, TableIsPrintedCorrectly) {
    auto table = GetSampleTable();
    std::vector<std::unique_ptr<DataRow>> rows;
    auto drow = new DataRow{std::vector<DataCell>{{std::string("AAA")}, {10}}};
    rows.push_back(std::make_unique<DataRow>(DataRow{ std::vector<DataCell>{{std::string("AAA")}, {10}}}));
    rows.push_back(std::make_unique<DataRow>(DataRow{ std::vector<DataCell>{{std::string("NNN")}, {20}}}));
    auto serialized = GetSerializedResult(rows, table);
    EXPECT_EQ(serialized,  "Column1;Column2\nAAA;10\nNNN;20\n");
}
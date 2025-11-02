#include <gtest/gtest.h>
#include "../utils/formatter.h"
#include "../metadata/cell.h"
#include "../utils/serialization.h"

TEST(FormatterTests, CellConvertedToStringCorrectly) {

    DataCell sCell {"koteczek"};
    Column sColumn {"Column1", ColumnType::String};
    auto sCellResult = GetStringValue(sCell,sColumn);
    EXPECT_EQ(sCellResult,  "koteczek");

    DataCell iCell {2141628429};
    Column iColumn {"Column2", ColumnType::Int};
    auto iCellResult = GetStringValue(iCell,iColumn);
    EXPECT_EQ(iCellResult,  "2141628429");
}

TableDefinition GetSampleTable(){
    return TableDefinition {
        "Table1",
        {{"Column1", ColumnType::String}, {"Column2", ColumnType::Int}}
    };
}

TEST(FormatterTests, RowIsPrintedCorrectly) {
    auto table = GetSampleTable();
    DataRow row{
        std::vector<DataCell>{
            {"koteczek"},
            {2141628429}
        }
    };
    auto serialized = GetSerializedRow(row, table);
    EXPECT_EQ(serialized,  "koteczek;2141628429");
}

TEST(FormatterTests, HeaderIsPrintedCorrectly) {
    auto table = GetSampleTable();
    auto serialized = GetSerializedHeader(table);
    EXPECT_EQ(serialized,  "Column1;Column2");
}

TEST(FormatterTests, TableIsPrintedCorrectly) {
    auto table = GetSampleTable();
    std::vector<DataRow> rows{
        {{{"AAA"}, {10}}},
        {{{"NNN"}, {20}}}
    };
    auto serialized = GetSerializedResult(rows, table);
    EXPECT_EQ(serialized,  "Column1;Column2\nAAA;10\nNNN;20\n");
}
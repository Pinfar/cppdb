#include <gtest/gtest.h>
#include "../utils/formatter.h"
#include "../metadata/cell.h"
#include "../utils/serialization.h"

TEST(FormatterTests, CellConvertedToStringCorrectly) {

    DataCell sCell {8, {0x6b, 0x6f , 0x74 , 0x65 , 0x63 , 0x7a , 0x65 , 0x6b}};
    Column sColumn {"Column1", ColumnType::String};
    auto sCellResult = GetStringValue(sCell,sColumn);
    EXPECT_EQ(sCellResult,  "koteczek");

    DataCell iCell {4, {13, 168, 166, (unsigned char)127}};
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
            {8, {0x6b, 0x6f , 0x74 , 0x65 , 0x63 , 0x7a , 0x65 , 0x6b}},
            {4, {13, 168, 166, (unsigned char)127}}
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
        {{Serialize("AAA"), Serialize(10)}},
        {{Serialize("NNN"), Serialize(20)}}
    };
    auto serialized = GetSerializedResult(rows, table);
    EXPECT_EQ(serialized,  "Column1;Column2\nAAA;10\nNNN;20\n");
}
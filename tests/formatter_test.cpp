#include <gtest/gtest.h>
#include "../utils/formatter.h"
#include "../metadata/cell.h"

template<typename T>
struct FormatterTestCase{
    std::vector<unsigned char> bytes;
    T expectedResult;
};

TEST(FormatterTests, IntDeserializedCorrectly) {

    std::array testCases{
        FormatterTestCase<int>{{22, (unsigned char)-42, 18, 0}, 1234454},
        FormatterTestCase<int>{{13, 168, 0, 0}, 43021},
        FormatterTestCase<int>{{13, 168, 166, (unsigned char)127}, 2141628429},
        FormatterTestCase<int>{{243, 87, 89, (unsigned char)128}, -2141628429}
    };
    for(auto& testCase : testCases){
        DataCell cell {
            4,
            testCase.bytes
        };
        int result = DeserializeAsInt(cell);
        EXPECT_EQ(result, testCase.expectedResult);
    }
}

TEST(FormatterTests, StringDeserializedCorrectly) {

    std::array testCases{
        FormatterTestCase<std::string>{{0x6b, 0x6f , 0x74 , 0x65 , 0x63 , 0x7a , 0x65 , 0x6b}, "koteczek"},
    };
    for(auto& testCase : testCases){
        DataCell cell {
            4,
            testCase.bytes
        };
        std::string result = DeserializeAsString(cell);
        EXPECT_EQ(result, testCase.expectedResult);
    }
}

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
#include <gtest/gtest.h>
#include "../utils/serialization.h"
#include "../metadata/cell.h"

template<typename T>
struct SerializationTestCase{
    std::vector<unsigned char> bytes;
    T expectedResult;
};

TEST(SerializationTests, IntDeserializedCorrectly) {

    std::array testCases{
        SerializationTestCase<int>{{22, (unsigned char)-42, 18, 0}, 1234454},
        SerializationTestCase<int>{{13, 168, 0, 0}, 43021},
        SerializationTestCase<int>{{13, 168, 166, (unsigned char)127}, 2141628429},
        SerializationTestCase<int>{{243, 87, 89, (unsigned char)128}, -2141628429}
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

TEST(SerializationTests, StringDeserializedCorrectly) {

    std::array testCases{
        SerializationTestCase<std::string>{{0x6b, 0x6f , 0x74 , 0x65 , 0x63 , 0x7a , 0x65 , 0x6b}, "koteczek"},
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
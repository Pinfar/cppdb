#include "../sql_interface/parser.h"
#include <gtest/gtest.h>

namespace DBCPP::SqlInterface
{
    TEST(ParserTest, EmptyQueryEmitsEOF) {
        std::string source = "";
        Parser parser {&source};
        auto result = parser.tokenizeSource();
        EXPECT_EQ(result.size(), 1);
        EXPECT_EQ(result[0].type, TokenType::Eof);
    }

    TEST(ParserTest, PrimitivesAreEmited){
        std::string source = ",.=<>><";
        Parser parser {&source};
        auto result = parser.tokenizeSource();
        ASSERT_EQ(result.size(), 7);
        EXPECT_EQ(result[0].type, TokenType::Comma);
        EXPECT_EQ(result[1].type, TokenType::Dot);
        EXPECT_EQ(result[2].type, TokenType::Eq);
        EXPECT_EQ(result[3].type, TokenType::Neq);
        EXPECT_EQ(result[4].type, TokenType::Gt);
        EXPECT_EQ(result[5].type, TokenType::Lt);
        EXPECT_EQ(result[6].type, TokenType::Eof);
    }

    TEST(ParserTest, WhitespacesAreHandled){
        std::string source = "   .  . .\t.\r\n..";
        Parser parser {&source};
        auto result = parser.tokenizeSource();
        ASSERT_EQ(result.size(), 7);
        for(int i=0;i<result.size()-1;i++)
        {
            EXPECT_EQ(result[i].type, TokenType::Dot);
        }
        EXPECT_EQ(result[6].type, TokenType::Eof);
    }

    TEST(ParserTest, MetadataWorks){
        std::string source = "   <>   ";
        Parser parser {&source};
        auto result = parser.tokenizeSource();
        ASSERT_EQ(result.size(), 2);
        auto token = result[0];
        EXPECT_EQ(token.type, TokenType::Neq);
        EXPECT_EQ(token.source->substr(token.beginPosition, token.length), "<>");
        EXPECT_EQ(result[1].type, TokenType::Eof);
    }


}
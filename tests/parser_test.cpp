#include "../sql_interface/parser.h"
#include <gtest/gtest.h>

namespace DBCPP::SqlInterface
{
    static void checkOrderOfTokens(std::vector<Token> actualTokens, std::vector<TokenType> expectedOrder){
        ASSERT_EQ(actualTokens.size(), expectedOrder.size()+1);
        for(int i=0;i<expectedOrder.size();i++){
            EXPECT_EQ(actualTokens[i].type, expectedOrder[i]);
        }
        EXPECT_EQ(actualTokens[actualTokens.size()-1].type, TokenType::Eof);
    }

    static void checkOrderOfTokens(std::string source, std::vector<TokenType> expectedOrder)
    {
        Parser parser {&source};
        auto result = parser.tokenizeSource();
        checkOrderOfTokens(result, expectedOrder);
    }

    TEST(ParserTest, EmptyQueryEmitsEOF) {
        std::string source = "";
        Parser parser {&source};
        auto result = parser.tokenizeSource();
        checkOrderOfTokens(result,{});
    }

    TEST(ParserTest, PrimitivesAreEmited){
        checkOrderOfTokens(",.=<>><",{
            TokenType::Comma,
            TokenType::Dot,
            TokenType::Eq,
            TokenType::Neq,
            TokenType::Gt,
            TokenType::Lt
        });
    }

    TEST(ParserTest, WhitespacesAreHandled){
        checkOrderOfTokens("   .  . .\t.\r\n..",{
            TokenType::Dot,
            TokenType::Dot,
            TokenType::Dot,
            TokenType::Dot,
            TokenType::Dot,
            TokenType::Dot
        });
    }

    TEST(ParserTest, LinesAndPositionsAreCorrect){
        std::string source = "..\n\n.  .\n.";
        Parser parser {&source};
        auto result = parser.tokenizeSource();
        ASSERT_EQ(result.size(), 6);
        EXPECT_EQ(result[0].line, 1);
        EXPECT_EQ(result[0].position, 1);
        EXPECT_EQ(result[1].line, 1);
        EXPECT_EQ(result[1].position, 2);
        EXPECT_EQ(result[2].line, 3);
        EXPECT_EQ(result[2].position, 1);
        EXPECT_EQ(result[3].line, 3);
        EXPECT_EQ(result[3].position, 4);
        EXPECT_EQ(result[4].line, 4);
        EXPECT_EQ(result[4].position, 1);
    }

    TEST(ParserTest, LinesAndPositionsOfMulticharTokensAreCorrect){
        std::string source = "  <>";
        Parser parser {&source};
        auto result = parser.tokenizeSource();
        ASSERT_EQ(result.size(), 2);
        EXPECT_EQ(result[0].line, 1);
        EXPECT_EQ(result[0].position, 3);
    }

    TEST(ParserTest, IdentifiersWork){
        checkOrderOfTokens("aaa bbb\nccccccddsa",{
            TokenType::Identifier,
            TokenType::Identifier,
            TokenType::Identifier
        });
    }

    TEST(ParserTest, IdentifiersMixedWithOperatorsWork){
        checkOrderOfTokens("aaa>bbb<>ccc",{
            TokenType::Identifier,
            TokenType::Gt,
            TokenType::Identifier,
            TokenType::Neq,
            TokenType::Identifier
        });
    }

    TEST(ParserTest, SimpleQueryIsTokenized){
        checkOrderOfTokens("select name, surname, pay from workers where name=surname",{
            TokenType::Select,
            TokenType::Identifier,
            TokenType::Comma,
            TokenType::Identifier,
            TokenType::Comma,
            TokenType::Identifier,
            TokenType::From,
            TokenType::Identifier,
            TokenType::Where,
            TokenType::Identifier,
            TokenType::Eq,
            TokenType::Identifier
        });
    }

    TEST(ParserTest, MetadataWorks){
        std::string source = "   <>   ";
        Parser parser {&source};
        auto result = parser.tokenizeSource();
        auto token = result[0];
        EXPECT_EQ(token.source->substr(token.beginOffset, token.length), "<>");
        checkOrderOfTokens(result,{
            TokenType::Neq
        });
    }


}
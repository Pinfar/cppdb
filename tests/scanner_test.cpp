#include "../sql_interface/scanner.h"
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
        Scanner scanner {&source};
        auto result = scanner.tokenizeSource();
        checkOrderOfTokens(result, expectedOrder);
    }

    TEST(ScannerTest, EmptyQueryEmitsEOF) {
        std::string source = "";
        Scanner scanner {&source};
        auto result = scanner.tokenizeSource();
        checkOrderOfTokens(result,{});
    }

    TEST(ScannerTest, PrimitivesAreEmited){
        checkOrderOfTokens(",.=<>><",{
            TokenType::Comma,
            TokenType::Dot,
            TokenType::Eq,
            TokenType::Neq,
            TokenType::Gt,
            TokenType::Lt
        });
    }

    TEST(ScannerTest, WhitespacesAreHandled){
        checkOrderOfTokens("   .  . .\t.\r\n..",{
            TokenType::Dot,
            TokenType::Dot,
            TokenType::Dot,
            TokenType::Dot,
            TokenType::Dot,
            TokenType::Dot
        });
    }

    TEST(ScannerTest, LinesAndPositionsAreCorrect){
        std::string source = "..\n\n.  .\n.";
        Scanner scanner {&source};
        auto result = scanner.tokenizeSource();
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

    TEST(ScannerTest, LinesAndPositionsOfMulticharTokensAreCorrect){
        std::string source = "  <>";
        Scanner scanner {&source};
        auto result = scanner.tokenizeSource();
        ASSERT_EQ(result.size(), 2);
        EXPECT_EQ(result[0].line, 1);
        EXPECT_EQ(result[0].position, 3);
    }

    TEST(ScannerTest, IdentifiersWork){
        checkOrderOfTokens("aaa bbb\nccccccddsa",{
            TokenType::Identifier,
            TokenType::Identifier,
            TokenType::Identifier
        });
    }

    TEST(ScannerTest, IdentifiersWithNumbersWork){
        checkOrderOfTokens("aa123a123_21lksdaf",{
            TokenType::Identifier
        });
    }

    TEST(ScannerTest, IdentifiersMixedWithOperatorsWork){
        checkOrderOfTokens("aaa>bbb<>ccc",{
            TokenType::Identifier,
            TokenType::Gt,
            TokenType::Identifier,
            TokenType::Neq,
            TokenType::Identifier
        });
    }

    TEST(ScannerTest, SimpleQueryIsTokenized){
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

    TEST(ScannerTest, QueryWithNumbersIsTokenized){
        checkOrderOfTokens("select * from workers where name=1230",{
            TokenType::Select,
            TokenType::Star,
            TokenType::From,
            TokenType::Identifier,
            TokenType::Where,
            TokenType::Identifier,
            TokenType::Eq,
            TokenType::Number
        });
    }

    TEST(ScannerTest, MetadataWorks){
        std::string source = "   <>   ";
        Scanner scanner {&source};
        auto result = scanner.tokenizeSource();
        auto token = result[0];
        EXPECT_EQ(token.source->substr(token.beginOffset, token.length), "<>");
        checkOrderOfTokens(result,{
            TokenType::Neq
        });
    }

    static Select_ptr compileSql(std::string& sql) 
    {
        Scanner scanner {&sql};
        auto tokens = scanner.tokenizeSource();
        Parser parser {tokens};
        return parser.Parse();
    }

    TEST(ParserTest, VectorIsMoved) {
        std::string sql = "select a from b";
        Scanner scanner {&sql};
        auto tokens = scanner.tokenizeSource();
        ASSERT_EQ(tokens.size(),5);
        Parser parser {tokens};
        ASSERT_EQ(tokens.size(),0);
    }

    TEST(ParserTest, SelectStatementIsParsed) {
        std::string sql = "select a,b,c from x";
        auto select = compileSql(sql);
        ASSERT_TRUE(select);
        ASSERT_TRUE(select->columnList);
        ASSERT_EQ(select->columnList->columns.size(), 3);
        EXPECT_EQ(select->columnList->columns[0], "a");
        EXPECT_EQ(select->columnList->columns[1], "b");
        EXPECT_EQ(select->columnList->columns[2], "c");
        ASSERT_TRUE(select->from);
        EXPECT_EQ(select->from->tableName, "x");
    }

    TEST(ParserTest, WhereStatementIsParsed) {
        std::string sql = "select a,b,c from x where a=1";
        auto select = compileSql(sql);
        ASSERT_TRUE(select);
        ASSERT_TRUE(select->where);
        ASSERT_TRUE(select->where->condition);
        EXPECT_EQ(select->where->condition->lhs->token.GetTokenValue(), "a");
        EXPECT_EQ(select->where->condition->oper.type, TokenType::Eq);
        EXPECT_EQ(select->where->condition->rhs->token.GetTokenValue(), "1");
    }


}
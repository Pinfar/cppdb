#pragma once
#include "scanner.h"
#include <vector>
#include <memory>
#include <optional>

namespace DBCPP::SqlInterface{
    
    struct ExpressionNode {
        Token token;
    };
    using Expr_ptr = std::unique_ptr<ExpressionNode>;

    struct ConditionNode {
        Expr_ptr lhs;
        Token oper;
        Expr_ptr rhs;
    };
    using ConditionNode_ptr = std::unique_ptr<ConditionNode>;

    //Node defitinitions
    struct WhereNode
    {
        ConditionNode_ptr condition;
    };

    using Where_ptr = std::unique_ptr<WhereNode>;

    struct FromNode 
    {
        std::string tableName;
    };

    using From_ptr = std::unique_ptr<FromNode>;

    struct SelectColumnList 
    {
        std::vector<std::string> columns;
    };

    using SelectColumnList_ptr = std::unique_ptr<SelectColumnList>;

    struct SelectNode
    {
        SelectColumnList_ptr columnList;
        From_ptr from;
        Where_ptr where;
    };

    using Select_ptr = std::unique_ptr<SelectNode>;

    struct CompilationError {
        std::string message;
        Token token;
    };

    class Compiler
    {
        std::vector<Token> m_tokens;
        std::optional<CompilationError> m_compilationError;
        int m_position = 0;

        void Error(std::string message, Token token);
        Token Consume(TokenType type);
        Token Advance();
        Token Peek();
        Select_ptr Select();
        From_ptr From();
        Where_ptr Where();
        SelectColumnList_ptr ColumnList();
        ConditionNode_ptr Condition();
        Expr_ptr Expression();

        public:
            Compiler(std::vector<Token> &tokens): m_tokens(std::move(tokens)){

            }

            Select_ptr Compile();
            CompilationError GetError();
    };
}
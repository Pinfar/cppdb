#pragma once
#include "parser.h"
#include <vector>
#include <memory>
#include <optional>

namespace DBCPP::SqlInterface{
    
    //Node defitinitions
    struct WhereNode
    {
        //I want to implement conditions as a bytecode vm, so let's leave this empty for now
    };

    using Where_ptr = std::unique_ptr<WhereNode>;

    struct FromNode 
    {
        std::string TableName;
        std::unique_ptr<WhereNode> Where;
    };

    using From_ptr = std::unique_ptr<FromNode>;

    struct SelectNode
    {
        std::vector<std::string> Columns;
        From_ptr From;
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

        public:
            Compiler(std::vector<Token> &tokens): m_tokens(std::move(tokens)){

            }

            Select_ptr Compile();
            CompilationError GetError();
    };
}
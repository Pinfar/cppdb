#include "parser.h"
namespace DBCPP::SqlInterface{

    

    Token Parser::nextToken()
    {
        consumeWhitespaces();
        char current = consume();
        switch(current){
            case '\0':
                return Token{TokenType::Eof, m_source, m_position-1, 1 };
            case '.':
                return Token{TokenType::Dot, m_source, m_position-1, 1 };
            case ',':
                return Token{TokenType::Comma, m_source, m_position-1, 1 };
            case '=':
                return Token{TokenType::Eq, m_source, m_position-1, 1 };
            case '<':
                if(peek() == '>')
                {
                    consume();
                    return Token{TokenType::Neq, m_source, m_position-2, 2 };
                }
                else 
                {
                    return Token{TokenType::Lt, m_source, m_position-1, 1 };
                }
            case '>':
                return Token{TokenType::Gt, m_source, m_position-1, 1 };
            default:
                return Token{TokenType::Invalid, m_source, m_position-1, 1 };

        }
    }

    char Parser::consume()
    {
        char current = (*m_source)[m_position];
        m_position++;
        return current;
    }

    char Parser::peek()
    {
        return (*m_source)[m_position];
    }

    void Parser::consumeWhitespaces()
    {
        for(char x = peek(); x == ' ' || x== '\t' || x=='\n' || x=='\r'; x = peek()){
            consume();
        }
    }

    std::vector<Token> Parser::tokenizeSource()
    {
        std::vector<Token> result;
        for(;;)
        {
            Token token = nextToken();
            result.push_back(token);
            if(token.type == TokenType::Eof) break;
        }
        return result;
    }
}
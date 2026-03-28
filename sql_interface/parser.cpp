#include "parser.h"
namespace DBCPP::SqlInterface{

    

    Token Parser::nextToken()
    {
        consumeWhitespaces();
        char current = consume();
        switch(current){
            case '\0':
                return createToken(TokenType::Eof);
            case '.':
                return createToken(TokenType::Dot);
            case ',':
                return createToken(TokenType::Comma);
            case '=':
                return createToken(TokenType::Eq);
            case '<':
                if(peek() == '>')
                {
                    consume();
                    return createToken(TokenType::Neq);
                }
                else 
                {
                    return createToken(TokenType::Lt);
                }
            case '>':
                return createToken(TokenType::Gt);
            default:
                return createToken(TokenType::Invalid);

        }
    }

    char Parser::consume()
    {
        char current = (*m_source)[m_offset];
        m_offset++;
        m_length++;
        m_position++;
        return current;
    }

    char Parser::peek()
    {
        return (*m_source)[m_offset];
    }

    void Parser::consumeWhitespaces()
    {
        for(char x = peek(); x == ' ' || x== '\t' || x=='\n' || x=='\r'; x = peek()){
            consume();
            if(x=='\n'){
                m_line++;
                m_position=0;
            }
        }
        //reset length, because we start with new token after whitespace
        m_length = 0;
    }

    Token Parser::createToken(TokenType type)
    {
        Token token {type, m_source, m_offset-m_length, m_length, m_line, m_position};
        m_length = 0;
        return token;
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
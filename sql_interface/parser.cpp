#include "parser.h"
namespace DBCPP::SqlInterface{

    static bool isAlpha(char current)
    {
        return current >= 'a' && current <='z' 
            || current >='A' && current <='Z' 
            || current == '_';
    }

    static bool isNumber(char current)
    {
        return current >= '0' && current <='9';
    }

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
            case '*':
                return createToken(TokenType::Star);
            default:
                if(isAlpha(current))
                {
                    return consumeIdentifier();
                }
                else if(isNumber(current))
                {
                    return consumeNumber();
                }
                else 
                {
                    return createToken(TokenType::Invalid);
                }

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
        Token token {type, m_source, m_offset-m_length, m_length, m_line, m_position-m_length+1};
        m_length = 0;
        return token;
    }

    Token Parser::consumeIdentifier()
    {
        while(isAlpha(peek())) consume();
        auto value = m_source->substr(m_offset-m_length, m_length);
        auto type = TokenType::Identifier;
        if(value == "select"){
            type = TokenType::Select;
        } else if (value == "from"){
            type = TokenType::From;
        } else if (value == "where") {
            type = TokenType::Where;
        }
        return createToken(type);
    }

    Token Parser::consumeNumber()
    {
        while(isNumber(peek())) consume();
        return createToken(TokenType::Number);
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

    std::string Token::GetTokenValue()
    {
        return source->substr(beginOffset, length);
    }
}
#include "scanner.h"
#include <stdexcept>

namespace DBCPP::SqlInterface
{

static bool isAlpha(char current)
{
    return current >= 'a' && current <= 'z' || current >= 'A' && current <= 'Z' || current == '_';
}

static bool isNumber(char current)
{
    return current >= '0' && current <= '9';
}

Token Scanner::nextToken()
{
    consumeWhitespaces();
    char current = consume();
    switch (current)
    {
    case '\0':
        return createToken(TokenType::Eof);
    case '.':
        return createToken(TokenType::Dot);
    case ',':
        return createToken(TokenType::Comma);
    case '=':
        return createToken(TokenType::Eq);
    case '<':
        if (peek() == '>')
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
    case '+':
        return createToken(TokenType::Plus);
    case '-':
        return createToken(TokenType::Minus);
    case '/':
        return createToken(TokenType::Slash);
    case '\'':
        return consumeString();
    default:
        if (isAlpha(current))
        {
            return consumeIdentifier();
        }
        else if (isNumber(current))
        {
            return consumeNumber();
        }
        else
        {
            return createToken(TokenType::Invalid);
        }
    }
}

char Scanner::consume()
{
    char current = (*m_source)[m_offset];
    m_offset++;
    m_length++;
    m_position++;
    return current;
}

char Scanner::peek()
{
    return (*m_source)[m_offset];
}

void Scanner::consumeWhitespaces()
{
    for (char x = peek(); x == ' ' || x == '\t' || x == '\n' || x == '\r'; x = peek())
    {
        consume();
        if (x == '\n')
        {
            m_line++;
            m_position = 0;
        }
    }
    // reset length, because we start with new token after whitespace
    m_length = 0;
}

Token Scanner::createToken(TokenType type)
{
    Token token{type, m_source, m_offset - m_length, m_length, m_line, m_position - m_length + 1};
    m_length = 0;
    return token;
}

Token Scanner::consumeIdentifier()
{
    while (isAlpha(peek()) || isNumber(peek()) || peek() == '_')
        consume(); // missing numbers in identifiers
    auto value = m_source->substr(m_offset - m_length, m_length);
    auto type = TokenType::Identifier;
    if (value == "select")
    {
        type = TokenType::Select;
    }
    else if (value == "from")
    {
        type = TokenType::From;
    }
    else if (value == "where")
    {
        type = TokenType::Where;
    }
    else if (value == "or")
    {
        type = TokenType::Or;
    }
    else if (value == "and")
    {
        type = TokenType::And;
    }
    return createToken(type);
}

Token Scanner::consumeNumber()
{
    while (isNumber(peek()))
        consume();
    return createToken(TokenType::Number);
}

Token Scanner::consumeString()
{
    while (peek() != '\'')
    {
        char next = consume();
        if (next == '\0')
        {
            Error("Unterminated string!");
        }
    }
    consume(); // consume ending '
    return createToken(TokenType::String);
}

void Scanner::Error(std::string message)
{
    throw std::domain_error("Scanning error! " + message);
}

std::vector<Token> Scanner::tokenizeSource()
{
    std::vector<Token> result;
    for (;;)
    {
        Token token = nextToken();
        result.push_back(token);
        if (token.type == TokenType::Eof)
            break;
    }
    return result;
}

std::string Token::GetTokenValue() const
{
    return source->substr(beginOffset, length);
}
} // namespace DBCPP::SqlInterface
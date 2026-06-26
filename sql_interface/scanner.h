#pragma once
#include <string>
#include <vector>
namespace DBCPP::SqlInterface
{
enum class TokenType
{
    Identifier,
    Comma,
    Dot,
    Eq,
    Neq,
    Gt,
    Lt,
    Eof,
    Select,
    From,
    Where,
    Number,
    Star,
    Invalid,
    String,
    And,
    Or,
    Plus,
    Minus,
    Slash
};

struct Token
{
    TokenType type;
    std::string *source;
    int beginOffset;
    int length;
    int line;
    int position;

    std::string GetTokenValue() const;
};

class Scanner
{
  private:
    std::string *m_source;
    int m_offset = 0;
    int m_length = 1;
    int m_line = 1;
    int m_position = 0;
    Token nextToken();
    char consume();
    char peek();
    void consumeWhitespaces();
    Token createToken(TokenType type);
    Token consumeIdentifier();
    Token consumeNumber();
    Token consumeString();
    void Error(std::string message);

  public:
    Scanner(std::string *source) : m_source(source)
    {
    }
    std::vector<Token> tokenizeSource();
};
} // namespace DBCPP::SqlInterface
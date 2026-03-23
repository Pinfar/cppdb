#include <string>
#include <vector>
namespace DBCPP::SqlInterface{
    enum class TokenType {
        Identifier,
        Comma,
        Dot,
        Eq,
        Neq,
        Gt,
        Lt,
        Eof,
        Invalid
    };

    struct Token {
        TokenType type;
        std::string* source;
        int beginPosition;
        int length;
    };

    class Parser {
        private:
            std::string* m_source;
            int m_position = 0;
            Token nextToken();
            char consume();
            char peek();
            void consumeWhitespaces();
        public:
            Parser(std::string* source):m_source(source){}
            std::vector<Token> tokenizeSource();
    };
}
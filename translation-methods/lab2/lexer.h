#ifndef LAB2_LEXER_H
#define LAB2_LEXER_H

#include <istream>

enum class Token {
    ASTER,
    COMMA,
    END,
    FUN,
    LPAREN,
    MINUS,
    NUMBER,
    PLUS,
    RPAREN,
    SLASH,
};

std::string token_to_string(Token);

class lexer_t {
public:
    explicit lexer_t(std::istream&);
    ~lexer_t() = default;

    [[nodiscard]] Token cur_token() const;
    void next_token();
    Token get();
    Token assert_token(Token);
    void invoke_error(std::string const& msg = "") const;

private:
    std::istream& is;
    int pos;
    Token token;

private:
    void skip_while(int (*)(int));
};

#endif //LAB2_LEXER_H

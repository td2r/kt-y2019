#include "lexer.h"

std::string token_to_string(Token const token) {
    switch (token) {
        case Token::FUN:
            return "f";
        case Token::PLUS:
            return "+";
        case Token::MINUS:
            return "-";
        case Token::ASTER:
            return "*";
        case Token::SLASH:
            return "/";
        case Token::LPAREN:
            return "(";
        case Token::RPAREN:
            return ")";
        case Token::NUMBER:
            return "n";
        case Token::END:
            return "$";
        case Token::COMMA:
            return ",";
    }
}

lexer_t::lexer_t(std::istream& is)
        : is(is)
        , pos(0)
        , token()
{}

Token lexer_t::cur_token() const {
    return token;
}

void lexer_t::next_token() {
    skip_while(std::isspace);
    int c = is.get();
    pos++;
    switch (c) {
        case '+':
            token = Token::PLUS;
            break;
        case '-':
            token = Token::MINUS;
            break;
        case '*':
            token = Token::ASTER;
            break;
        case '/':
            token = Token::SLASH;
            break;
        case '(':
            token = Token::LPAREN;
            break;
        case ')':
            token = Token::RPAREN;
            break;
        case ',':
            token = Token::COMMA;
            break;
        case EOF:
            token = Token::END;
            break;
        default:
            if (std::isdigit(c)) {
                skip_while(std::isdigit);
                token = Token::NUMBER;
            } else if (std::isalpha(c)) {
                skip_while(std::isalpha);
                token = Token::FUN;
            } else {
                invoke_error();
            }
    }
}

Token lexer_t::get() {
    Token ret = cur_token();
    next_token();
    return ret;
}

Token lexer_t::assert_token(Token expected) {
    if (token != expected) {
        invoke_error(":  Expected token \"" +
                     token_to_string(expected) +
                     "\", but found \"" +
                     token_to_string(token) +
                     "\"");
    }
    next_token();
    return expected;
}

void lexer_t::invoke_error(std::string const& msg) const {
    throw std::runtime_error("Parse error at pos " +
                             std::to_string(pos) +
                             ", with token \"" +
                             token_to_string(token) +
                             "\"" +
                             msg);
}

void lexer_t::skip_while(int (*filter)(int)) {
    while (filter(is.peek())) {
        is.get();
        pos++;
    }
}

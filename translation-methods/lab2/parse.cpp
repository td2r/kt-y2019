#include "parse.h"

#include <memory>
#include <sstream>

tree_t* E(lexer_t&);

tree_t* AS(lexer_t& lex) {
    std::unique_ptr<tree_t> res(new tree_t("A'"));
    switch (lex.cur_token()) {
        case Token::COMMA:
            res->add_child(new tree_t(lex.get()));
            res->add_child(E(lex));
            res->add_child(AS(lex));
            break;
        case Token::RPAREN:
            res->add_child(new tree_t(" "));
            break;
        default:
            lex.invoke_error();
    }
    return res.release();
}

tree_t* A(lexer_t& lex) {
    std::unique_ptr<tree_t> res(new tree_t("A"));
    switch (lex.cur_token()) {
        case Token::LPAREN:
        case Token::MINUS:
        case Token::FUN:
        case Token::NUMBER:
            res->add_child(E(lex));
            res->add_child(AS(lex));
            break;
        case Token::RPAREN:
            res->add_child(new tree_t(" "));
            break;
        default:
            lex.invoke_error();
    }
    return res.release();
}

tree_t* F(lexer_t& lex) {
    std::unique_ptr<tree_t> res(new tree_t("F"));
    switch (lex.cur_token()) {
        case Token::FUN:
            res->add_child(new tree_t(lex.get()));
            res->add_child(new tree_t(lex.assert_token(Token::LPAREN)));
            res->add_child(A(lex));
            res->add_child(new tree_t(lex.assert_token(Token::RPAREN)));
            break;
        case Token::LPAREN:
            res->add_child(new tree_t(lex.get()));
            res->add_child(E(lex));
            res->add_child(new tree_t(lex.assert_token(Token::RPAREN)));
            break;
        case Token::MINUS:
            res->add_child(new tree_t(lex.get()));
            res->add_child(F(lex));
            break;
        case Token::NUMBER:
            res->add_child(new tree_t(lex.get()));
            break;
        default:
            lex.invoke_error();
    }
    return res.release();
}

tree_t* TS(lexer_t& lex) {
    std::unique_ptr<tree_t> res(new tree_t("T'"));
    switch (lex.cur_token()) {
        case Token::ASTER:
        case Token::SLASH:
            res->add_child(new tree_t(lex.get()));
            res->add_child(F(lex));
            res->add_child(TS(lex));
            break;
        case Token::PLUS:
        case Token::MINUS:
        case Token::RPAREN:
        case Token::END:
        case Token::COMMA:
            res->add_child(new tree_t(" "));
            break;
        default:
            lex.invoke_error();
    }
    return res.release();
}

tree_t* T(lexer_t& lex) {
    std::unique_ptr<tree_t> res(new tree_t("T"));
    res->add_child(F(lex));
    res->add_child(TS(lex));
    return res.release();
}

tree_t* ES(lexer_t& lex) {
    std::unique_ptr<tree_t> res(new tree_t("E'"));
    switch (lex.cur_token()) {
        case Token::PLUS:
        case Token::MINUS:
            res->add_child(new tree_t(lex.get()));
            res->add_child(T(lex));
            res->add_child(ES(lex));
            break;
        case Token::RPAREN:
        case Token::END:
        case Token::COMMA:
            res->add_child(new tree_t(" "));
            break;
        default:
            lex.invoke_error();
    }
    return res.release();
}

tree_t* E(lexer_t& lex) {
    std::unique_ptr<tree_t> res(new tree_t("E"));
    res->add_child(T(lex));
    res->add_child(ES(lex));
    return res.release();
}

tree_t* parse(std::istream& is) {
    lexer_t lex(is);
    lex.next_token();
    std::unique_ptr<tree_t> res(E(lex));
    lex.assert_token(Token::END);
    return res.release();
}

tree_t* parse(std::string const& s) {
    std::istringstream iss(s);
    return parse(iss);
}

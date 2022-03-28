#pragma once

#include <map>
#include "lexer.h"

struct parser_t {
    explicit parser_t(std::string const& s);
    explicit parser_t(std::istream& is);
    $$entity* parse();
    lexer_t lex;
    $$entity* root;
    std::vector<int> nodes;
    std::vector<$$entity*> entities;
    std::map<std::pair<int, int> const, void(*)(parser_t&)> const jmp;
    ~parser_t();
};

#ifndef LAB_4_GENERATOR_H
#define LAB_4_GENERATOR_H

#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <fstream>

struct generator_t {
    struct ntdata {
        std::string attrs;
        std::set<std::pair<std::vector<std::string>, std::string>> rules;

        ntdata();
        ntdata(std::string attrs, std::vector<std::string> rule, std::string constr);
    };

    std::ifstream ifs;
    std::string header;
    __attribute__((unused)) std::string members;
    std::string main_rule;
    std::vector<std::string> terminals_order;
    std::vector<std::pair<std::string, std::string>> terminals;
    std::map<std::string, ntdata> nonterminals;
    std::vector<std::pair<std::string, std::vector<std::string>>> rules;
    std::map<std::string, std::set<std::string>> first;
    std::vector<std::set<std::pair<int, int>>> node_by_idx;
    std::map<std::set<std::pair<int, int>>, int> idx_by_node;
    std::map<std::pair<int, int>, std::set<std::string>> lookahead;
    std::vector<std::map<std::string, std::string>> edges;

    void generate(std::string const& path_to_grammar, std::string const& path_output);

private:
    void skip_ws();
    void assert_string(std::string const& s);
    std::string collect_until(char c);
    std::string next_name();
    void parse_header();
    void parse_members();
    void parse_main_rule();
    std::string parse_nt_attrs();
    std::string parse_nt_constr();
    void parse_entities();
    void parse_grammar(std::string const& path_to_grammar);
    void implement_lexer(const std::string& path_output);
    void implement_parser(const std::string& path_output);
    void implement_lexer_h_header(std::ofstream& ofs) const;
    void implement_lexer_h_enum(std::ofstream& ofs);
    void implement_lexer_h_entities(std::ofstream& ofs);
    static void implement_lexer_h_struct(std::ofstream& ofs);
    static std::vector<std::string> build_nt_args(const std::vector<std::string>& types);
    static void implement_lexer_cpp_header(std::ofstream& ofs) ;
    void implement_lexer_cpp_enum(std::ofstream& ofs);
    void implement_lexer_cpp_entities(std::ofstream& ofs);
    void implement_lexer_cpp_struct(std::ofstream& ofs);
    std::set<std::string> first_of(std::vector<std::string> a);
    std::set<std::string> first_of(std::vector<std::string> a, const std::set<std::string>& la);
    void init_utils();
    void epsilon_closure(std::set<std::pair<int, int>>& node);
    void print_LR0_item(std::pair<int, int> const& item);
    std::set<std::pair<int, int>> absorb_symbol(std::set<std::pair<int, int>> const& node, std::string const& sym);
    static void implement_parser_h(std::ofstream& ofs);
    void implement_parser_cpp(std::ofstream& ofs);
};

#endif //LAB_4_GENERATOR_H

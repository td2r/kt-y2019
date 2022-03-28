#include <iostream>
#include <algorithm>
#include <regex>
#include <utility>
#include "generator.h"

using namespace std;

template <typename T>
static vector<T> concat(vector<T> const& a, vector<T> const& b) {
    vector<T> result(a.begin(), a.end());
    result.insert(result.end(), b.begin(), b.end());
    return result;
}

generator_t::ntdata::ntdata()
: attrs()
, rules({})
{}

generator_t::ntdata::ntdata(string attrs, vector<string> rule, string constr)
: attrs(std::move(attrs))
, rules({{ rule, constr }})
{}

void generator_t::generate(string const& path_to_grammar, string const& path_output) {
    parse_grammar(path_to_grammar);
    init_utils();

    cout << "terminals:\n";
    for (auto const &p: terminals) {
        cout << p.first << " -> " << p.second << "\n";
    }
    cout << "\n" << "nonterminals:\n";
    for (auto const &p: nonterminals) {
        cout << "  attrs {" << p.second.attrs << "}\n"
             << "  rules {\n";
        for (auto const &it1: p.second.rules) {
            cout << "    " << p.first << " -> ";
            for (auto const &it2: it1.first) {
                cout << it2 << ",";
            }
            cout << "; constructor {" << it1.second << "}\n";
        }
    }
    cout << "\n" << "rules\n";
    for (size_t i = 0; i < rules.size(); ++i) {
        cout << "[" << i << "] = " << rules[i].first << " ->";
        for (string const& s : rules[i].second) {
            cout << " " << (s.empty() ? "\\eps" : s);
        }
        cout << "\n";
    }
    cout << "\n" << "FIRST\n";
    for (auto const& p : first) {
        cout << p.first << ":";
        string sep = " {";
        for (string const& s : p.second) {
            cout << sep << (s.empty() ? "\\eps" : s);
            sep = ", ";
        }
        cout << "}\n";
    }
    cout << "\n" << "nodes\n";
    for (size_t i = 0; i < node_by_idx.size(); ++i) {
        cout << "[" << i << "]: {\n";
        for (auto const& p : node_by_idx[i]) {
            vector<string> v(rules[p.first].second.begin(), rules[p.first].second.end());
            cout << "    ";
            print_LR0_item(p);
            cout << "\n";
        }
        cout << "}\n";
    }
    cout << "\n" << "lookaheads\n";
    for (auto const& p : lookahead) {
        cout << "[";
        print_LR0_item(p.first);
        cout << "] = {";
        for (string const& s : p.second) {
            cout << " \"" << s << "\"";
        }
        cout << " }\n";
    }
    cout << "\n" << "edges\n";
    for (size_t i = 0; i < node_by_idx.size(); ++i) {
        cout << "node[" << i << "]:\n";
        for (auto const& p : edges[i]) {
            cout << "  " << p.first << " -> " << p.second << "\n";
        }
    }

    implement_lexer(path_output);
    implement_parser(path_output);
}

void generator_t::skip_ws() {
    while (isspace(ifs.peek()))
        ifs.get();
}

void generator_t::assert_string(string const& s) {
    for (size_t i = 0; i < s.length(); ++i) {
        if (ifs.peek() != s[i])
            throw runtime_error("string assertion fail: " + s);
        ifs.get();
    }
}

string generator_t::collect_until(char c) {
    string result;
    while (true) {
        if (ifs.peek() == EOF)
            throw runtime_error("expected char " + to_string(c) + ", but found EOF");
        if (ifs.peek() == c) {
            ifs.get();
            return result;
        }
        if (ifs.peek() == '\\') {
            ifs.get();
            if (ifs.peek() == c) {
                result.push_back(c);
            } else if (ifs.peek() != EOF) {
                result.push_back('\\');
                result.push_back((char) ifs.peek());
            } else {
                throw runtime_error("expected char " + to_string(c) + ", but found EOF");
            }
            ifs.get();
        } else {
            result.push_back((char) ifs.get());
        }
    }
}

string generator_t::next_name() {
    string name;
    skip_ws();
    while (isalpha(ifs.peek()) || isdigit(ifs.peek()) || ifs.peek() == '_') {
        name.push_back((char) ifs.get());
    }
    return name;
}

void generator_t::parse_header() {
    skip_ws();
    if (ifs.peek() == '@') {
        assert_string("@header");
        skip_ws();
        assert_string("{");
        header = collect_until('}');
        return;
    }
}

void generator_t::parse_members() {
    skip_ws();
    if (ifs.peek() == '@') {
        assert_string("@members");
        skip_ws();
        assert_string("{");
        members = collect_until('}');
        return;
    }
}

void generator_t::parse_main_rule() {
    skip_ws();
    assert_string("main");
    skip_ws();
    assert_string("=");
    skip_ws();
    main_rule = next_name();
    assert_string(";");
}

string generator_t::parse_nt_attrs() {
    skip_ws();
    if (ifs.peek() == '<') {
        ifs.get();
        return collect_until('>');
    }
    return "";
}

string generator_t::parse_nt_constr() {
    skip_ws();
    if (ifs.peek() == '{') {
        ifs.get();
        return collect_until('}');
    }
    return "";
}

void generator_t::parse_entities() {
    while (true) {
        skip_ws();
        if (ifs.peek() == EOF)
            return;
        if (isalpha(ifs.peek())) {
            string name = next_name();
            if (islower(name[0])) {
                skip_ws();
                assert_string("->");
                skip_ws();
                assert_string("/");
                string rgx = collect_until('/');
                skip_ws();
                assert_string(";");
                if (find_if(terminals.begin(), terminals.end(),
                         [name](pair<string, string> const& p) { return p.first == name; }) != terminals.end())
                    throw runtime_error("second rule for terminal entity");
                terminals.emplace_back( name, rgx );
            } else { // isupper
                skip_ws();
                string attrs = parse_nt_attrs();
                skip_ws();
                assert_string("->");
                skip_ws();
                vector<string> rule;
                string entity;
                while (!(entity = next_name()).empty()) {
                    rule.push_back(entity);
                }
                string constr = parse_nt_constr();
                skip_ws();
                assert_string(";");
                if (nonterminals.find(name) != nonterminals.end()) {
                    if (!attrs.empty())
                        cout << "Warning: re-specified nonterminal's '" << name <<  "' attributes <" << attrs << "> ignored\n";
                    nonterminals[name].rules.insert({ rule, constr });
                } else {
                    nonterminals.insert({ name, ntdata(attrs, rule, constr) });
                }
            }
        } else {
            throw runtime_error("expected entity name, starting with letter but found '" + to_string(ifs.peek()) + "'");
        }
    }
}

void generator_t::parse_grammar(string const& path_to_grammar) {
    ifs.open(path_to_grammar);

    parse_header();
    parse_members();
    parse_main_rule();
    parse_entities();
    nonterminals.insert({main_rule + "$", ntdata("", {main_rule}, "")});

    ifs.close();
}

void generator_t::implement_lexer(const std::string& path_output) {
    ofstream ofs;
    ofs.open(path_output + "/lexer.h");
    implement_lexer_h_header(ofs);
    implement_lexer_h_enum(ofs);
    implement_lexer_h_entities(ofs);
    implement_lexer_h_struct(ofs);
    ofs.close();
    ofs.open(path_output + "/lexer.cpp");
    implement_lexer_cpp_header(ofs);
    implement_lexer_cpp_enum(ofs);
    implement_lexer_cpp_entities(ofs);
    implement_lexer_cpp_struct(ofs);
    ofs.close();
}

void generator_t::implement_lexer_h_header(ofstream& ofs) const {
    ofs << "#pragma once\n"
           "#include <istream>\n"
           "#include <deque>\n"
           "#include <vector>\n" <<
           header <<
           "\n";
}

void generator_t::implement_lexer_h_enum(ofstream &ofs) {
    ofs << "enum : int {\n";
    for (auto const& p : concat(terminals, {{"$", ""}}))
        ofs << "    " << p.first << ",\n";
    for (auto const& p : nonterminals) {
        ofs << "    " << p.first << ",\n";
    }
    ofs << "};\n"
           "\n"
           "// debug\n"
           "std::string token_to_str(int token);\n"
           "\n";
}

void generator_t::implement_lexer_h_entities(ofstream &ofs) {
    ofs << "struct $$entity {\n"
           "    int type;\n"
           "    explicit $$entity(int type);\n"
           "    bool is_terminal() const;\n"
           "    bool is_nonterminal() const;\n"
           "    virtual ~$$entity() = default;\n"
           "};\n"
           "\n"
           "struct $$terminal : public $$entity {\n"
           "    std::string text;\n"
           "    $$terminal(int type, std::string text);\n"
           "    ~$$terminal() override = default;\n"
           "};\n"
           "\n"
           "struct $$nonterminal : public $$entity {\n"
           "    std::vector<$$entity*> children;\n"
           "    $$nonterminal(int type, std::vector<$$entity*>  children);\n"
           "    ~$$nonterminal() override;"
           "};\n"
           "\n";
    for (auto const& p : concat(terminals, {{ "$", "" }})) {
        ofs << "struct $$" << p.first << " : public $$terminal { explicit $$" << p.first << "(std::string text);};\n";
    }
    for (auto const& p : nonterminals) {
        ofs << "struct $$" << p.first << ";\n";
    }
    for (auto const& p1 : nonterminals) {
        ofs << "struct $$" << p1.first << " : public $$nonterminal {\n";
        string attrs_code = p1.second.attrs;
        replace(attrs_code.begin(), attrs_code.end(), ',', ';');
        ofs << "    " << attrs_code << ";\n";
        for (auto const& p2 : p1.second.rules) {
            const vector<std::string> &nt_args = build_nt_args(p2.first);
            ofs << "    " << (nt_args.size() == 1 ? "explicit " : "") << "$$" << p1.first << "(";
            for (size_t i = 0; i < nt_args.size(); ++i) {
                if (i) ofs << ", ";
                ofs << "$$" << p2.first[i] << "* " << nt_args[i];
            }
            ofs << ");\n";
        }
        ofs << "};\n";
    }
}

void generator_t::implement_lexer_h_struct(ofstream& ofs) {
    ofs << "struct lexer_t {\n"
           "    explicit lexer_t(std::string const& s);\n"
           "    explicit lexer_t(std::istream& is);\n"
           "    $$terminal* current_terminal();\n"
           "    $$terminal* next_terminal();\n"
           "private:\n"
           "    std::deque<$$terminal*> dq;\n"
           "};\n";
}

vector<string> generator_t::build_nt_args(const std::vector<std::string>& types) {
    map<string, int> cnt;
    for (auto const& t : types) {
        if (cnt.find(t) == cnt.end()) {
            cnt.insert({t, 1});
        } else {
            cnt[t]++;
        }
    }
    map<string, bool> need_idx;
    for (auto const& t : types) {
        need_idx[t] = (cnt[t] != 1);
    }
    for (auto& p : cnt) {
        p.second = 1;
    }
    vector<string> res;
    res.reserve(types.size());
    for (const auto & type : types) {
        res.push_back("$" + type + (need_idx[type] ? "_" + to_string(cnt[type]++) : ""));
    }
    return res;
}

void generator_t::implement_lexer_cpp_header(ofstream &ofs) {
    ofs << "#include \"lexer.h\"\n"
           "#include <regex>\n"
           "\n";
}

void generator_t::implement_lexer_cpp_enum(ofstream &ofs) {
    ofs << "std::string token_to_str(int token) {\n"
           "    switch (token) {\n";
    for (auto const& p : concat(terminals, {{ "$", "" }})) {
        ofs << "        case " << p.first << ":\n"
               "            return \"" << p.first << "\";\n";
    }
    for (auto const& p : nonterminals) {
        ofs << "        case " << p.first << ":\n"
               "            return \"" << p.first << "\";\n";
    }
    ofs <<"        default:\n"
           "            return \"unknown token\";\n"
           "    }\n"
           "}\n"
           "\n";
}

void generator_t::implement_lexer_cpp_entities(ofstream &ofs) {
    ofs << "$$entity::$$entity(int type) : type(type) {}\n"
           "\n"
           "bool $$entity::is_terminal() const {\n"
           "    return type <= $;\n"
           "}\n"
           "\n"
           "bool $$entity::is_nonterminal() const {\n"
           "    return type > $;\n"
           "}\n"
           "\n"
           "$$terminal::$$terminal(int type, std::string text) : $$entity(type), text(std::move(text)) {}\n"
           "\n"
           "$$nonterminal::$$nonterminal(int type, std::vector<$$entity*> children) : $$entity(type), children(std::move(children)) {}\n"
           "\n"
           "$$nonterminal::~$$nonterminal() {\n"
           "    size_t sz = children.size();\n"
           "    for (size_t i = 0; i < sz; ++i)\n"
           "        delete children[sz - i - 1];\n"
           "}\n"
           "\n";
    for (auto const& p : concat(terminals, {{ "$", "" }})) {
        ofs << "$$" << p.first << "::$$" << p.first << "(std::string text) "
               ": $$terminal(" << p.first << ", std::move(text)) {}\n";
    }
    for (auto const& p1 : nonterminals) {
        for (auto const& p2 : p1.second.rules) {
            const vector<std::string> &nt_args = build_nt_args(p2.first);
            string args_list;
            string typed_args_list;
            for (size_t i = 0; i < nt_args.size(); ++i) {
                if (i) {
                    args_list += ", ";
                    typed_args_list += ", ";
                }
                typed_args_list += "$$" + p2.first[i] + "* " + nt_args[i];
                args_list += nt_args[i];
            }
            ofs << "$$" << p1.first << "::$$" << p1.first << "(" << typed_args_list << ")\n"
                   ": $$nonterminal(" << p1.first << ", {" << args_list << "})\n"
                   "{\n" <<
                   p2.second << "\n"
                   "}\n";
        }
    }
}

static void check_regex(string const& s) {
    try {
        const basic_regex<char> &basicRegex = regex(s);
    } catch (...) {
        throw runtime_error("incorrect regex \"" + s + "\"");
    }
}

void generator_t::implement_lexer_cpp_struct(ofstream &ofs) {
    string rgx_for_all;
    for (auto const& p : terminals) {
        check_regex(p.second);
        ofs << "static const std::regex rgx_" << p.first << "(R\"(" << p.second << ")\");\n";
        if (!rgx_for_all.empty())
            rgx_for_all.push_back('|');
        rgx_for_all += p.second;
    }
    check_regex(rgx_for_all);
    ofs << "static const std::regex rgx(R\"(" << rgx_for_all << ")\");\n"
           "\n"
           "lexer_t::lexer_t(const std::string &s) {\n"
           "    std::sregex_token_iterator iter(s.begin(), s.end(), rgx);\n"
           "    std::sregex_token_iterator end;\n"
           "    for (; iter != end; ++iter) {\n"
           "        std::string fragment = *iter;\n";

    for (auto const& p : terminals) {
        ofs << "        if (std::regex_match(fragment, rgx_" << p.first << ")) {\n"
               "            dq.push_back(new $$" << p.first << "(fragment));\n"
               "            continue;\n"
               "        }\n";
    }

    ofs << "        throw std::runtime_error(\"fragment \\\"\" + fragment + \"\\\" doesn't match any regex\");\n"
           "    }\n"
           "}\n"
           "\n"
           "std::string slurp(std::istream& is) {\n"
           "    std::string s;\n"
           "    while (is.peek() != EOF) {\n"
           "        s.push_back((char) is.get());\n"
           "    }\n"
           "    return s;\n"
           "}\n"
           "\n"
           "lexer_t::lexer_t(std::istream &is) : lexer_t(slurp(is)) {}\n"
           "\n"
           "$$terminal* end_terminal = new $$$(\"\");\n"
           "\n"
           "\n"
           "$$terminal* lexer_t::current_terminal() {\n"
           "    return dq.empty() ? end_terminal : dq.front();\n"
           "}\n"
           "\n"
           "$$terminal *lexer_t::next_terminal() {\n"
           "    if (dq.empty())\n"
           "        return end_terminal;\n"
           "    dq.pop_front();\n"
           "    return current_terminal();\n"
           "}\n";
}

void generator_t::implement_parser(const std::string& path_output) {
    ofstream ofs;
    ofs.open(path_output + "/parser.h");
    implement_parser_h(ofs);
    ofs.close();
    ofs.open(path_output + "/parser.cpp");
    implement_parser_cpp(ofs);
    ofs.close();
}

std::set<std::string> generator_t::first_of(std::vector<std::string> a) {
    return first_of(std::move(a), { "" });
}

std::set<std::string> generator_t::first_of(std::vector<std::string> a, const std::set<std::string>& la) {
    if (a.empty())
        return {la.begin(), la.end()};
    if (a[0].empty())
        throw runtime_error("epsilon in a");
    else if (!isupper(a[0][0]))
        return { a[0] };
    set<string> result(first[a[0]]);
    if (result.find("") != result.end()) {
        result.erase("");
        set<string> rh_first = first_of(vector<string>(a.begin() + 1, a.end()), la);
        result.insert(rh_first.begin(), rh_first.end());
    }
    return result;
}

void generator_t::init_utils() {
    // rules
    rules.emplace_back(main_rule + "$", nonterminals[main_rule + "$"].rules.begin()->first); // main rule has 0 index
    for (auto const& p1 : nonterminals) {
        if (p1.first.back() == '$') // already in rules
            continue;
        for (auto const& p2 : p1.second.rules) {
            rules.emplace_back(p1.first, p2.first);
        }
    }

    // FIRST
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto const& p : rules) {
            set<string> a_first = first_of(p.second);
            size_t const size_before = first[p.first].size();
            first[p.first].insert(a_first.begin(), a_first.end());
            changed |= (size_before != first[p.first].size());
        }
    }

    // edges
    node_by_idx.push_back({{0, 0}});
    lookahead[{0, 0}].insert("$");
    epsilon_closure(node_by_idx[0]);
    idx_by_node[node_by_idx[0]] = 0;
    edges.emplace_back();
    for (size_t i = 0; i < node_by_idx.size(); ++i) {
        // absorbs
        for (auto const& p : terminals) {
            if (edges[i].find(p.first) == edges[i].end()) {
                const set<std::pair<int, int>> &next_node = absorb_symbol(node_by_idx[i], p.first);
                if (!next_node.empty()) {
                    if (idx_by_node.find(next_node) == idx_by_node.end()) {
                        node_by_idx.push_back(next_node);
                        idx_by_node.insert({ node_by_idx.back(), node_by_idx.size() - 1 });
                        edges.emplace_back();
                    }
                    edges[i].insert({ p.first, "shift<" + to_string(idx_by_node[next_node]) + ">" });
                }
            }
        }
        for (auto const& p : nonterminals) {
            if (edges[i].find(p.first) == edges[i].end()) {
                const set<std::pair<int, int>> &next_node = absorb_symbol(node_by_idx[i], p.first);
                if (!next_node.empty()) {
                    if (idx_by_node.find(next_node) == idx_by_node.end()) {
                        node_by_idx.push_back(next_node);
                        idx_by_node.insert({ node_by_idx.back(), node_by_idx.size() - 1 });
                        edges.emplace_back();
                    }
                    edges[i].insert({ p.first, "jump<" + to_string(idx_by_node[next_node]) + ">" });
                }
            }
        }
    }
    for (size_t i = 0; i < rules.size(); ++i) {
        for (size_t j = 0; j < rules[i].second.size(); ++j) {
            lookahead[{i, j + 1}].insert(lookahead[{i, j}].begin(), lookahead[{i, j}].end());
        }
    }
    for (size_t i = 0; i < node_by_idx.size(); ++i) {
        // folds
        for (auto const& p : node_by_idx[i]) {
            if (p.second == rules[p.first].second.size()) {
                for (string const& term : lookahead[p]) {
                    if (edges[i].find(term) != edges[i].end() && edges[i][term].rfind("fold", 0) == 0) {
                        throw runtime_error("fold-fold conflict: node[" + to_string(i) + "], term " + term);
                    }
                    edges[i][term] = "fold" + to_string(p.first);
//                    edges[i].insert({ term, "fold" + to_string(p.first) });
                }
            }
        }
    }
}

void generator_t::epsilon_closure(set<std::pair<int, int>>& node) {
    vector<pair<int, int>> res(node.begin(), node.end());
    set<pair<int, int>> used;
    for (size_t i = 0; i < res.size(); ++i) {
        pair<int, int> const& p = res[i];
        vector<string> const& a = rules[p.first].second;
        int const dot_idx = p.second;
        if (dot_idx < a.size() && isupper(a[dot_idx][0]) && used.find(p) == used.end()) {
            string const& B = a[dot_idx];
            set<string> const& la_add = first_of({a.begin() + dot_idx + 1, a.end()}, lookahead[p]);
            for (size_t j = 0; j < rules.size(); ++j) {
                if (rules[j].first != B)
                    continue;
                res.emplace_back(j, 0);
                lookahead[{j, 0}].insert(la_add.begin(), la_add.end());
            }
            used.insert(p);
        }
    }
    node = set<pair<int, int>>(res.begin(), res.end());
}

void generator_t::print_LR0_item(const pair<int, int>& item) {
//    vector<string> v(rules[item.first].second.begin(), rules[item.first].second.end());
//    v.insert(v.begin() + item.second, ".");
//    cout << rules[item.first].first << " ->";
//    for (string const& s : v) {
//        cout << " " << s;
//    }
    cout << rules[item.first].first << " ->";
    for (int i = 0; i < item.second; ++i) {
        cout << " " << rules[item.first].second[i];
    }
    wcout << " " << L"·";
    for (int i = item.second + 1; i < rules[item.first].second.size(); ++i) {
        cout << " " << rules[item.first].second[i];
    }
}

std::set<std::pair<int, int>> generator_t::absorb_symbol(const set<std::pair<int, int>> &node, const string &sym) {
    set<pair<int, int>> res;
    for (auto const& p : node) {
        string const& A = rules[p.first].first;
        vector<string> const& a = rules[p.first].second;
        if (p.second < a.size() && a[p.second] == sym) {
            res.insert({ p.first, p.second + 1 });
            lookahead[{ p.first, p.second + 1 }].insert(lookahead[p].begin(), lookahead[p].end());
        }
    }
    epsilon_closure(res);
    return res;
}

void generator_t::implement_parser_h(ofstream& ofs) {
    ofs << "#pragma once\n"
           "\n"
           "#include <map>\n"
           "#include \"lexer.h\"\n"
           "\n"
           "struct parser_t {\n"
           "    explicit parser_t(std::string const& s);\n"
           "    explicit parser_t(std::istream& is);\n"
           "    $$entity* parse();\n"
           "    lexer_t lex;\n"
           "    $$entity* root;\n"
           "    std::vector<int> nodes;\n"
           "    std::vector<$$entity*> entities;\n"
           "    std::map<std::pair<int, int> const, void(*)(parser_t&)> const jmp;\n"
           "    ~parser_t();\n"
           "};\n";
}

void generator_t::implement_parser_cpp(ofstream &ofs) {
    ofs << "#include <sstream>\n"
           "#include <cassert>\n"
           "#include \"parser.h\"\n"
           "\n"
           "template<int S>\n"
           "void shift(parser_t& parser) {\n"
           "    parser.entities.emplace_back(parser.lex.current_terminal());\n"
           "    parser.lex.next_terminal();\n"
           "    parser.nodes.emplace_back(S);\n"
           "}\n"
           "\n"
           "template<int S>\n"
           "void jump(parser_t& parser) {\n"
           "    assert(parser.root);\n"
           "    parser.root = nullptr;\n"
           "    parser.nodes.emplace_back(S);\n"
           "}\n"
           "\n";
    for (size_t i = 0; i < rules.size(); ++i) {
        ofs << "void fold" << i << "(parser_t& parser) {\n";
        size_t sz = rules[i].second.size();
        string call_args;
        for (size_t j = 0; j < sz; ++j) {
            ofs << "    auto* child" << sz - j << " = dynamic_cast<$$" << rules[i].second[sz - j - 1] << "*>(parser.entities.back());\n"
                   "    parser.nodes.pop_back();\n"
                   "    parser.entities.pop_back();\n";
            if (j)
                call_args += ", ";
            call_args += "child" + to_string(j + 1);
        }
        ofs << "    parser.entities.push_back(new $$" << rules[i].first << "(" << call_args << "));\n"
               "    parser.root = parser.entities.back();\n"
               "}\n"
               "\n";
    }
    ofs << "std::initializer_list<std::pair<std::pair<int, int> const, void(*)(parser_t&)>> jmp_init = {\n";
    for (size_t i = 0; i < edges.size(); ++i) {
        for (auto const& p : edges[i]) {
            ofs << "{{" << i << "," << p.first << "}, " << p.second << "},\n";
        }
    }
    ofs << "};\n"
           "\n"
           "parser_t::parser_t(const std::string &s)\n"
           ": lex(s)\n"
           ", root()\n"
           ", nodes()\n"
           ", entities()\n"
           ", jmp(jmp_init)\n"
           "{}\n"
           "\n"
           "parser_t::parser_t(std::istream &is)\n"
           ": lex(is)\n"
           ", root()\n"
           ", nodes()\n"
           ", entities()\n"
           ", jmp(jmp_init)\n"
           "{}\n"
           "\n"
           "$$entity *parser_t::parse() {\n"
           "    nodes.emplace_back(0);\n"
           "    while (entities.empty() || entities.back()->type != " << main_rule << "$) {\n"
           "        $$entity* sym = root ? root : lex.current_terminal();\n"
           "        auto const it = jmp.find({nodes.back(), sym->type});\n"
           "        if (it == jmp.end()) {\n"
           "            throw std::runtime_error(\"no jump from node \" + std::to_string(nodes.back()) +\n"
           "                                    \" by term \" + token_to_str(sym->type));\n"
           "        } else {\n"
           "            it->second(*this);\n"
           "        }\n"
           "    }\n"
           "    auto* result = dynamic_cast<$$nonterminal*>(root);\n"
           "    root = nullptr;\n"
           "    nodes.pop_back();\n"
           "    entities.pop_back();\n"
           "    return result->children[0];\n"
           "}\n"
           "\n"
           "parser_t::~parser_t() {\n"
           "    while (!entities.empty()) {\n"
           "        delete entities.back();\n"
           "        entities.pop_back();\n"
           "    }\n"
           "}\n";
}


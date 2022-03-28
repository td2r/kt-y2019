#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <iostream>
#include <regex>
#include "generator.h"
#include "lexer.h"
#include "parser.h"
#include "draw.h"

using namespace std;

#define CASE_CALCULATOR
//#define CASE_LAB2

std::string const project_path = "/home/td2r/Desktop/mt/lab-4-cpp";

#if defined(CASE_CALCULATOR)
std::string const grammar_path = "/home/td2r/Desktop/mt/lab-4-cpp/tmp/expressions_grammar.txt";
#elif defined(CASE_LAB2)
std::string const grammar_path = "/home/td2r/Desktop/mt/lab-4-cpp/tmp/lab2_grammar.txt";
#endif

std::string const input_text_path = "/home/td2r/Desktop/mt/lab-4-cpp/tmp/input.txt";
std::string const generated_data_path = "/home/td2r/Desktop/mt/lab-4-cpp/tmp/generated_data.txt";

void show_terminals_list() {
    std::ifstream ifs;
    ifs.open(input_text_path);
    lexer_t lex(ifs);
    $$terminal* token;
    while (true) {
        token = lex.current_terminal();
        std::cout << "type: " << token_to_str(token->type) << ", text: \"" << token->text << "\"" << std::endl;
        if (token->type == $) {
            break;
        }
        lex.next_terminal();
    }
    ifs.close();
}

void generate() {
    generator_t generator;
    freopen(generated_data_path.c_str(), "w", stdout);
    generator.generate(grammar_path, project_path);
    fclose(stdout);
}

void parse() {
    std::ifstream ifs;
    ifs.open(input_text_path);
    parser_t parser(ifs);
    unique_ptr<$$E> root(dynamic_cast<$$E *>(parser.parse()));
#if defined(CASE_CALCULATOR)
    cout << root->value << endl;
    draw("graph.svg", root.get());
#elif defined(CASE_LAB2)
    draw("graph.svg", root);
#endif
    ifs.close();
}

int main() {
//    show_terminals_list();
//    generate();
    parse();
    return 0;
}

#pragma clang diagnostic pop
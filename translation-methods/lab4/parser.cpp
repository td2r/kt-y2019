#include <sstream>
#include <cassert>
#include "parser.h"

template<int S>
void shift(parser_t& parser) {
    parser.entities.emplace_back(parser.lex.current_terminal());
    parser.lex.next_terminal();
    parser.nodes.emplace_back(S);
}

template<int S>
void jump(parser_t& parser) {
    assert(parser.root);
    parser.root = nullptr;
    parser.nodes.emplace_back(S);
}

void fold0(parser_t& parser) {
    auto* child1 = dynamic_cast<$$E*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    parser.entities.push_back(new $$E$(child1));
    parser.root = parser.entities.back();
}

void fold1(parser_t& parser) {
    auto* child3 = dynamic_cast<$$T*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    auto* child2 = dynamic_cast<$$minus*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    auto* child1 = dynamic_cast<$$E*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    parser.entities.push_back(new $$E(child1, child2, child3));
    parser.root = parser.entities.back();
}

void fold2(parser_t& parser) {
    auto* child3 = dynamic_cast<$$T*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    auto* child2 = dynamic_cast<$$plus*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    auto* child1 = dynamic_cast<$$E*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    parser.entities.push_back(new $$E(child1, child2, child3));
    parser.root = parser.entities.back();
}

void fold3(parser_t& parser) {
    auto* child1 = dynamic_cast<$$T*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    parser.entities.push_back(new $$E(child1));
    parser.root = parser.entities.back();
}

void fold4(parser_t& parser) {
    auto* child3 = dynamic_cast<$$rp*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    auto* child2 = dynamic_cast<$$E*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    auto* child1 = dynamic_cast<$$lp*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    parser.entities.push_back(new $$F(child1, child2, child3));
    parser.root = parser.entities.back();
}

void fold5(parser_t& parser) {
    auto* child1 = dynamic_cast<$$n*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    parser.entities.push_back(new $$F(child1));
    parser.root = parser.entities.back();
}

void fold6(parser_t& parser) {
    auto* child1 = dynamic_cast<$$F*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    parser.entities.push_back(new $$P(child1));
    parser.root = parser.entities.back();
}

void fold7(parser_t& parser) {
    auto* child3 = dynamic_cast<$$P*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    auto* child2 = dynamic_cast<$$power*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    auto* child1 = dynamic_cast<$$F*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    parser.entities.push_back(new $$P(child1, child2, child3));
    parser.root = parser.entities.back();
}

void fold8(parser_t& parser) {
    auto* child1 = dynamic_cast<$$P*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    parser.entities.push_back(new $$T(child1));
    parser.root = parser.entities.back();
}

void fold9(parser_t& parser) {
    auto* child3 = dynamic_cast<$$P*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    auto* child2 = dynamic_cast<$$aster*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    auto* child1 = dynamic_cast<$$T*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    parser.entities.push_back(new $$T(child1, child2, child3));
    parser.root = parser.entities.back();
}

void fold10(parser_t& parser) {
    auto* child3 = dynamic_cast<$$P*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    auto* child2 = dynamic_cast<$$slash*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    auto* child1 = dynamic_cast<$$T*>(parser.entities.back());
    parser.nodes.pop_back();
    parser.entities.pop_back();
    parser.entities.push_back(new $$T(child1, child2, child3));
    parser.root = parser.entities.back();
}

std::initializer_list<std::pair<std::pair<int, int> const, void(*)(parser_t&)>> jmp_init = {
{{0,E}, jump<3>},
{{0,F}, jump<4>},
{{0,P}, jump<5>},
{{0,T}, jump<6>},
{{0,lp}, shift<2>},
{{0,n}, shift<1>},
{{1,$}, fold5},
{{1,aster}, fold5},
{{1,minus}, fold5},
{{1,plus}, fold5},
{{1,power}, fold5},
{{1,rp}, fold5},
{{1,slash}, fold5},
{{2,E}, jump<7>},
{{2,F}, jump<4>},
{{2,P}, jump<5>},
{{2,T}, jump<6>},
{{2,lp}, shift<2>},
{{2,n}, shift<1>},
{{3,$}, fold0},
{{3,minus}, shift<9>},
{{3,plus}, shift<8>},
{{4,$}, fold6},
{{4,aster}, fold6},
{{4,minus}, fold6},
{{4,plus}, fold6},
{{4,power}, shift<10>},
{{4,rp}, fold6},
{{4,slash}, fold6},
{{5,$}, fold8},
{{5,aster}, fold8},
{{5,minus}, fold8},
{{5,plus}, fold8},
{{5,rp}, fold8},
{{5,slash}, fold8},
{{6,$}, fold3},
{{6,aster}, shift<11>},
{{6,minus}, fold3},
{{6,plus}, fold3},
{{6,rp}, fold3},
{{6,slash}, shift<12>},
{{7,minus}, shift<9>},
{{7,plus}, shift<8>},
{{7,rp}, shift<13>},
{{8,F}, jump<4>},
{{8,P}, jump<5>},
{{8,T}, jump<14>},
{{8,lp}, shift<2>},
{{8,n}, shift<1>},
{{9,F}, jump<4>},
{{9,P}, jump<5>},
{{9,T}, jump<15>},
{{9,lp}, shift<2>},
{{9,n}, shift<1>},
{{10,F}, jump<4>},
{{10,P}, jump<16>},
{{10,lp}, shift<2>},
{{10,n}, shift<1>},
{{11,F}, jump<4>},
{{11,P}, jump<17>},
{{11,lp}, shift<2>},
{{11,n}, shift<1>},
{{12,F}, jump<4>},
{{12,P}, jump<18>},
{{12,lp}, shift<2>},
{{12,n}, shift<1>},
{{13,$}, fold4},
{{13,aster}, fold4},
{{13,minus}, fold4},
{{13,plus}, fold4},
{{13,power}, fold4},
{{13,rp}, fold4},
{{13,slash}, fold4},
{{14,$}, fold2},
{{14,aster}, shift<11>},
{{14,minus}, fold2},
{{14,plus}, fold2},
{{14,rp}, fold2},
{{14,slash}, shift<12>},
{{15,$}, fold1},
{{15,aster}, shift<11>},
{{15,minus}, fold1},
{{15,plus}, fold1},
{{15,rp}, fold1},
{{15,slash}, shift<12>},
{{16,$}, fold7},
{{16,aster}, fold7},
{{16,minus}, fold7},
{{16,plus}, fold7},
{{16,rp}, fold7},
{{16,slash}, fold7},
{{17,$}, fold9},
{{17,aster}, fold9},
{{17,minus}, fold9},
{{17,plus}, fold9},
{{17,rp}, fold9},
{{17,slash}, fold9},
{{18,$}, fold10},
{{18,aster}, fold10},
{{18,minus}, fold10},
{{18,plus}, fold10},
{{18,rp}, fold10},
{{18,slash}, fold10},
};

parser_t::parser_t(const std::string &s)
: lex(s)
, root()
, nodes()
, entities()
, jmp(jmp_init)
{}

parser_t::parser_t(std::istream &is)
: lex(is)
, root()
, nodes()
, entities()
, jmp(jmp_init)
{}

$$entity *parser_t::parse() {
    nodes.emplace_back(0);
    while (entities.empty() || entities.back()->type != E$) {
        $$entity* sym = root ? root : lex.current_terminal();
        auto const it = jmp.find({nodes.back(), sym->type});
        if (it == jmp.end()) {
            throw std::runtime_error("no jump from node " + std::to_string(nodes.back()) +
                                    " by term " + token_to_str(sym->type));
        } else {
            it->second(*this);
        }
    }
    auto* result = dynamic_cast<$$nonterminal*>(root);
    root = nullptr;
    nodes.pop_back();
    entities.pop_back();
    return result->children[0];
}

parser_t::~parser_t() {
    while (!entities.empty()) {
        delete entities.back();
        entities.pop_back();
    }
}

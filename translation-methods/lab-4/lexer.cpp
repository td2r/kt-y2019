#include "lexer.h"
#include <regex>

std::string token_to_str(int token) {
    switch (token) {
        case power:
            return "power";
        case f:
            return "f";
        case n:
            return "n";
        case plus:
            return "plus";
        case minus:
            return "minus";
        case lp:
            return "lp";
        case rp:
            return "rp";
        case aster:
            return "aster";
        case slash:
            return "slash";
        case $:
            return "$";
        case E:
            return "E";
        case E$:
            return "E$";
        case F:
            return "F";
        case P:
            return "P";
        case T:
            return "T";
        default:
            return "unknown token";
    }
}

$$entity::$$entity(int type) : type(type) {}

bool $$entity::is_terminal() const {
    return type <= $;
}

bool $$entity::is_nonterminal() const {
    return type > $;
}

$$terminal::$$terminal(int type, std::string text) : $$entity(type), text(std::move(text)) {}

$$nonterminal::$$nonterminal(int type, std::vector<$$entity*> children) : $$entity(type), children(std::move(children)) {}

$$nonterminal::~$$nonterminal() {
    size_t sz = children.size();
    for (size_t i = 0; i < sz; ++i)
        delete children[sz - i - 1];
}

$$power::$$power(std::string text) : $$terminal(power, std::move(text)) {}
$$f::$$f(std::string text) : $$terminal(f, std::move(text)) {}
$$n::$$n(std::string text) : $$terminal(n, std::move(text)) {}
$$plus::$$plus(std::string text) : $$terminal(plus, std::move(text)) {}
$$minus::$$minus(std::string text) : $$terminal(minus, std::move(text)) {}
$$lp::$$lp(std::string text) : $$terminal(lp, std::move(text)) {}
$$rp::$$rp(std::string text) : $$terminal(rp, std::move(text)) {}
$$aster::$$aster(std::string text) : $$terminal(aster, std::move(text)) {}
$$slash::$$slash(std::string text) : $$terminal(slash, std::move(text)) {}
$$$::$$$(std::string text) : $$terminal($, std::move(text)) {}
$$E::$$E($$E* $E, $$minus* $minus, $$T* $T)
: $$nonterminal(E, {$E, $minus, $T})
{
value = $E->value - $T->value;
}
$$E::$$E($$E* $E, $$plus* $plus, $$T* $T)
: $$nonterminal(E, {$E, $plus, $T})
{
value = $E->value + $T->value;
}
$$E::$$E($$T* $T)
: $$nonterminal(E, {$T})
{
value = $T->value;
}
$$E$::$$E$($$E* $E)
: $$nonterminal(E$, {$E})
{

}
$$F::$$F($$lp* $lp, $$E* $E, $$rp* $rp)
: $$nonterminal(F, {$lp, $E, $rp})
{
value = $E->value;
}
$$F::$$F($$n* $n)
: $$nonterminal(F, {$n})
{
value = std::stoi($n->text);
}
$$P::$$P($$F* $F)
: $$nonterminal(P, {$F})
{
value = $F->value;
}
$$P::$$P($$F* $F, $$power* $power, $$P* $P)
: $$nonterminal(P, {$F, $power, $P})
{
value = (int) (std::pow((double) $F->value, (double) $P->value) + 0.5); 
}
$$T::$$T($$P* $P)
: $$nonterminal(T, {$P})
{
value = $P->value;
}
$$T::$$T($$T* $T, $$aster* $aster, $$P* $P)
: $$nonterminal(T, {$T, $aster, $P})
{
value = $T->value * $P->value;
}
$$T::$$T($$T* $T, $$slash* $slash, $$P* $P)
: $$nonterminal(T, {$T, $slash, $P})
{
value = $T->value / $P->value;
}
static const std::regex rgx_power(R"([ \t\n\r]*\*\*)");
static const std::regex rgx_f(R"([ \t\n\r]*[a-zA-Z]+)");
static const std::regex rgx_n(R"([ \t\n\r]*[0-9]+)");
static const std::regex rgx_plus(R"([ \t\n\r]*\+)");
static const std::regex rgx_minus(R"([ \t\n\r]*-)");
static const std::regex rgx_lp(R"([ \t\n\r]*\()");
static const std::regex rgx_rp(R"([ \t\n\r]*\))");
static const std::regex rgx_aster(R"([ \t\n\r]*\*)");
static const std::regex rgx_slash(R"([ \t\n\r]*/)");
static const std::regex rgx(R"([ \t\n\r]*\*\*|[ \t\n\r]*[a-zA-Z]+|[ \t\n\r]*[0-9]+|[ \t\n\r]*\+|[ \t\n\r]*-|[ \t\n\r]*\(|[ \t\n\r]*\)|[ \t\n\r]*\*|[ \t\n\r]*/)");

lexer_t::lexer_t(const std::string &s) {
    std::sregex_token_iterator iter(s.begin(), s.end(), rgx);
    std::sregex_token_iterator end;
    for (; iter != end; ++iter) {
        std::string fragment = *iter;
        if (std::regex_match(fragment, rgx_power)) {
            dq.push_back(new $$power(fragment));
            continue;
        }
        if (std::regex_match(fragment, rgx_f)) {
            dq.push_back(new $$f(fragment));
            continue;
        }
        if (std::regex_match(fragment, rgx_n)) {
            dq.push_back(new $$n(fragment));
            continue;
        }
        if (std::regex_match(fragment, rgx_plus)) {
            dq.push_back(new $$plus(fragment));
            continue;
        }
        if (std::regex_match(fragment, rgx_minus)) {
            dq.push_back(new $$minus(fragment));
            continue;
        }
        if (std::regex_match(fragment, rgx_lp)) {
            dq.push_back(new $$lp(fragment));
            continue;
        }
        if (std::regex_match(fragment, rgx_rp)) {
            dq.push_back(new $$rp(fragment));
            continue;
        }
        if (std::regex_match(fragment, rgx_aster)) {
            dq.push_back(new $$aster(fragment));
            continue;
        }
        if (std::regex_match(fragment, rgx_slash)) {
            dq.push_back(new $$slash(fragment));
            continue;
        }
        throw std::runtime_error("fragment \"" + fragment + "\" doesn't match any regex");
    }
}

std::string slurp(std::istream& is) {
    std::string s;
    while (is.peek() != EOF) {
        s.push_back((char) is.get());
    }
    return s;
}

lexer_t::lexer_t(std::istream &is) : lexer_t(slurp(is)) {}

$$terminal* end_terminal = new $$$("");


$$terminal* lexer_t::current_terminal() {
    return dq.empty() ? end_terminal : dq.front();
}

$$terminal *lexer_t::next_terminal() {
    if (dq.empty())
        return end_terminal;
    dq.pop_front();
    return current_terminal();
}

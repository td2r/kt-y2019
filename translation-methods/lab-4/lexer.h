#pragma once
#include <istream>
#include <deque>
#include <vector>

#include <string>
#include <cmath>

enum : int {
    power,
    f,
    n,
    plus,
    minus,
    lp,
    rp,
    aster,
    slash,
    $,
    E,
    E$,
    F,
    P,
    T,
};

// debug
std::string token_to_str(int token);

struct $$entity {
    int type;
    explicit $$entity(int type);
    bool is_terminal() const;
    bool is_nonterminal() const;
    virtual ~$$entity() = default;
};

struct $$terminal : public $$entity {
    std::string text;
    $$terminal(int type, std::string text);
    ~$$terminal() override = default;
};

struct $$nonterminal : public $$entity {
    std::vector<$$entity*> children;
    $$nonterminal(int type, std::vector<$$entity*>  children);
    ~$$nonterminal() override;};

struct $$power : public $$terminal { explicit $$power(std::string text);};
struct $$f : public $$terminal { explicit $$f(std::string text);};
struct $$n : public $$terminal { explicit $$n(std::string text);};
struct $$plus : public $$terminal { explicit $$plus(std::string text);};
struct $$minus : public $$terminal { explicit $$minus(std::string text);};
struct $$lp : public $$terminal { explicit $$lp(std::string text);};
struct $$rp : public $$terminal { explicit $$rp(std::string text);};
struct $$aster : public $$terminal { explicit $$aster(std::string text);};
struct $$slash : public $$terminal { explicit $$slash(std::string text);};
struct $$$ : public $$terminal { explicit $$$(std::string text);};
struct $$E;
struct $$E$;
struct $$F;
struct $$P;
struct $$T;
struct $$E : public $$nonterminal {
    int value;
    $$E($$E* $E, $$minus* $minus, $$T* $T);
    $$E($$E* $E, $$plus* $plus, $$T* $T);
    explicit $$E($$T* $T);
};
struct $$E$ : public $$nonterminal {
    ;
    explicit $$E$($$E* $E);
};
struct $$F : public $$nonterminal {
    int value;
    $$F($$lp* $lp, $$E* $E, $$rp* $rp);
    explicit $$F($$n* $n);
};
struct $$P : public $$nonterminal {
    int value;
    explicit $$P($$F* $F);
    $$P($$F* $F, $$power* $power, $$P* $P);
};
struct $$T : public $$nonterminal {
    int value;
    explicit $$T($$P* $P);
    $$T($$T* $T, $$aster* $aster, $$P* $P);
    $$T($$T* $T, $$slash* $slash, $$P* $P);
};
struct lexer_t {
    explicit lexer_t(std::string const& s);
    explicit lexer_t(std::istream& is);
    $$terminal* current_terminal();
    $$terminal* next_terminal();
private:
    std::deque<$$terminal*> dq;
};

#ifndef LAB2_TREE_H
#define LAB2_TREE_H

#include "lexer.h"

#include <vector>

class tree_t {
public:
    explicit tree_t(Token);
    explicit tree_t(std::string);
    ~tree_t();

    void add_child(tree_t*);

    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] std::vector<tree_t*> const& get_children() const;

private:
    std::string const name;
    std::vector<tree_t*> children;
};

#endif //LAB2_TREE_H

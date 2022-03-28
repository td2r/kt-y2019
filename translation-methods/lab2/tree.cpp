#include "tree.h"

#include <queue>
#include <unordered_map>

tree_t::tree_t(Token token)
        : tree_t(token_to_string(token))
{}

tree_t::tree_t(std::string node)
        : name(std::move(node))
        , children()
{}

tree_t::~tree_t() {
    for (tree_t* child : children) {
        delete child;
    }
}

void tree_t::add_child(tree_t* child) {
    children.emplace_back(child);
}

std::string tree_t::get_name() const {
    return name;
}

const std::vector<tree_t*> &tree_t::get_children() const {
    return children;
}

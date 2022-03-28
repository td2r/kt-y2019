#include "draw.h"

#include <queue>

#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>

Agnode_t* create_node(Agraph_t* g, tree_t const* root, int const idx) {
    return agnode(g, const_cast<char*>((root->get_name() + "\n#" + std::to_string(idx)).c_str()), TRUE);
}

void draw(std::string const& dest, tree_t const* tree, std::string const& ext) {
    GVC_t* gvc;
    Agraph_t* g;
    gvc = gvContext();
    g = agopen(nullptr, Agdirected, nullptr);

    int idx = 1;
    std::queue<std::pair<tree_t const*, Agnode_t*>> q;
    q.push(std::make_pair(tree, create_node(g, tree, idx++)));
    while (!q.empty()) {
        tree_t const* from = q.front().first;
        Agnode_t* from_node = q.front().second;
        q.pop();
        for (tree_t const* to : from->get_children()) {
            Agnode_t* to_node = create_node(g, to, idx++);
            agedge(g, from_node, to_node, nullptr, TRUE);
            q.push(std::make_pair(to, to_node));
        }
    }

    gvLayout(gvc, g, "dot");
    gvRender(gvc, g, ext.c_str(), fopen(dest.c_str(), "w+"));
    gvFreeLayout(gvc, g);
    agclose(g);
}

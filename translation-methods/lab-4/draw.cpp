#include "draw.h"
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>
#include <queue>

Agnode_t* create_node(Agraph_t* g, $$entity const* root, int const idx) {
    std::string name = (root->is_terminal() ? dynamic_cast<$$terminal const*>(root)->text : token_to_str(root->type)) +
            "\n#" + std::to_string(idx);
    return agnode(g, const_cast<char*>(name.c_str()), TRUE);
}

void draw(std::string const& dest, $$entity* tree, std::string const& ext) {
    GVC_t* gvc;
    Agraph_t* g;
    gvc = gvContext();
    g = agopen(nullptr, Agdirected, nullptr);

    int idx = 1;
    std::queue<std::pair<$$entity*, Agnode_t*>> q;
    q.push(std::make_pair(tree, create_node(g, tree, idx++)));
    while (!q.empty()) {
        $$entity* from = q.front().first;
        Agnode_t* from_node = q.front().second;
        q.pop();
        auto* nt_from = dynamic_cast<$$nonterminal*>(from);
        if (!nt_from)
            continue;
        for ($$entity* to : nt_from->children) {
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


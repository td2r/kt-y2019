#include "tree.h"
#include "parse.h"
#include "draw.h"

#include <memory>
#include <iostream>

int main() {
    try {
        std::unique_ptr<tree_t> root(parse("5 * max(1,2, min(2, min()))"));
        draw("graph.svg", root.get());
    } catch (std::runtime_error const& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}

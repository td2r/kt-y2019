#ifndef LAB2_PARSE_H
#define LAB2_PARSE_H

#include "tree.h"

#include <string>
#include <vector>
#include <istream>

tree_t* parse(std::istream&);
tree_t* parse(std::string const&);

#endif //LAB2_PARSE_H

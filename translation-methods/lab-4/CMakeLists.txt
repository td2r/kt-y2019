cmake_minimum_required(VERSION 3.20)
project(lab_4)

set(CMAKE_CXX_STANDARD 14)

# include antlr4 to cpp project links:
# https://stackoverflow.com/questions/69867081/building-and-linking-the-antlr4-c-runtime-with-cmake
# https://www.cs.upc.edu/~padro/CL/practica/install.html
# PS: fuck this shit

add_executable(lab_4
        main.cpp
        parser.cpp parser.h
        lexer.cpp lexer.h
        draw.cpp draw.h
        generator.cpp generator.h)

target_link_libraries(lab_4 PUBLIC gvc cgraph)

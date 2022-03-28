cmake_minimum_required(VERSION 3.20)
project(lab4)

set(CMAKE_CXX_STANDARD 14)

# include antlr4 to cpp project links:
# https://stackoverflow.com/questions/69867081/building-and-linking-the-antlr4-c-runtime-with-cmake
# https://www.cs.upc.edu/~padro/CL/practica/install.html
# PS: fuck this shit

add_executable(lab4
        main.cpp
        parser.cpp parser.h
        lexer.cpp lexer.h
        draw.cpp draw.h
        generator.cpp generator.h)

target_link_libraries(lab4 PUBLIC gvc cgraph)

target_compile_definitions(lab4 PRIVATE
    PROJECT_PATH="${CMAKE_CURRENT_SOURCE_DIR}"


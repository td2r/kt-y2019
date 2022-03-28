#include "../tree.h"
#include "../parse.h"

#include <queue>
#include <sstream>
#include <unordered_map>

#include <gtest/gtest.h>

void strlex(std::string const& s) {
    std::istringstream iss(s);
    lexer_t lex(iss);
    do {
        lex.next_token();
    } while (lex.cur_token() != Token::END);
}

std::string tree_to_string(tree_t* root) {
    std::ostringstream sb;
    std::queue<tree_t*> q;
    std::unordered_map<tree_t*, int> idx_node;

    q.push(root);
    std::string sep;
    for (int i = 1; !q.empty(); sep = ",") {
        tree_t* const u = q.front();
        q.pop();

        idx_node.insert({u, i++});
        sb << sep << u->get_name();

        for (tree_t* const& v : u->get_children()) {
            q.push(v);
        }
    }
    sb << std::endl;

    q.push(root);
    while (!q.empty()) {
        tree_t* const u = q.front();
        q.pop();
        int const u_idx = idx_node[u];

        sb << u_idx;
        sep = ">";
        for (tree_t* const& v : u->get_children()) {
            sb << sep << idx_node[v];
            q.push(v);
            sep = ",";
        }
        sb << std::endl;
    }

    return sb.str();
}

std::string e2ts(const std::string& expr) {
    std::unique_ptr<tree_t> tree(parse(expr));
    return tree_to_string(tree.get());
}

TEST(lexer_tests, lexems) {
    std::istringstream iss(") \n+*  \t sin19823/ (-");
    lexer_t lex(iss);
    lex.next_token();
    EXPECT_EQ(lex.get(), Token::RPAREN);
    EXPECT_EQ(lex.get(), Token::PLUS);
    EXPECT_EQ(lex.get(), Token::ASTER);
    EXPECT_EQ(lex.get(), Token::FUN);
    EXPECT_EQ(lex.get(), Token::NUMBER);
    EXPECT_EQ(lex.get(), Token::SLASH);
    EXPECT_EQ(lex.get(), Token::LPAREN);
    EXPECT_EQ(lex.get(), Token::MINUS);
    EXPECT_EQ(lex.get(), Token::END);
    EXPECT_EQ(lex.get(), Token::END);
}

TEST(lexer_tests, failure) {
    EXPECT_NO_THROW(strlex("-------+++++++++++****/(////))////"));
    EXPECT_NO_THROW(strlex("\n\n\n\t\t**/()))-25n\n\t\t89+5-4blablabla"));
    EXPECT_NO_THROW(strlex("aboba1aboba2aboba3aboba4aboba"));
    EXPECT_NO_THROW(strlex(""));
    EXPECT_NO_THROW(strlex("11111111111111111111111111111111111111"));

    EXPECT_THROW(strlex("5 % 3"), std::runtime_error);
    EXPECT_THROW(strlex("6 = 7"), std::runtime_error);
    EXPECT_THROW(strlex("300$bucks"), std::runtime_error);
    EXPECT_THROW(strlex("5 > 3?"), std::runtime_error);
}

// test for F nonterminal
TEST(parser_tests, factor) {
    EXPECT_STREQ(e2ts("1").c_str(), "E,T,E',F,T', ,n, \n"
                                    "1>2,3\n"
                                    "2>4,5\n"
                                    "3>6\n"
                                    "4>7\n"
                                    "5>8\n"
                                    "6\n"
                                    "7\n"
                                    "8\n");
    EXPECT_STREQ(e2ts(" -\n - \t5\t\n ").c_str(), "E,T,E',F,T', ,-,F, ,-,F,n\n"
                                                  "1>2,3\n"
                                                  "2>4,5\n"
                                                  "3>6\n"
                                                  "4>7,8\n"
                                                  "5>9\n"
                                                  "6\n"
                                                  "7\n"
                                                  "8>10,11\n"
                                                  "9\n"
                                                  "10\n"
                                                  "11>12\n"
                                                  "12\n");
    EXPECT_STREQ(e2ts("f ( (5 )) ").c_str(), "E,T,E',F,T', ,f,(,E,), ,T,E',F,T', ,(,E,), ,T,E',F,T', ,n, \n"
                                             "1>2,3\n"
                                             "2>4,5\n"
                                             "3>6\n"
                                             "4>7,8,9,10\n"
                                             "5>11\n"
                                             "6\n"
                                             "7\n"
                                             "8\n"
                                             "9>12,13\n"
                                             "10\n"
                                             "11\n"
                                             "12>14,15\n"
                                             "13>16\n"
                                             "14>17,18,19\n"
                                             "15>20\n"
                                             "16\n"
                                             "17\n"
                                             "18>21,22\n"
                                             "19\n"
                                             "20\n"
                                             "21>23,24\n"
                                             "22>25\n"
                                             "23>26\n"
                                             "24>27\n"
                                             "25\n"
                                             "26\n"
                                             "27\n");

    EXPECT_THROW(parse("(5 + 2"), std::runtime_error);
    EXPECT_THROW(parse("((5)"), std::runtime_error);
    EXPECT_THROW(parse("f(g())"), std::runtime_error);
    EXPECT_THROW(parse("2 3"), std::runtime_error);

    EXPECT_NO_THROW(parse("((f(((--g(--((-(5)))))))))"));
}

// rules for T and T' nonterms
TEST(parser_tests, term) {
    EXPECT_STREQ(e2ts("1 / ln(- 3 )").c_str(), "E,T,E',F,T', ,n,/,F,T',f,(,E,), ,T,E',F,T', ,-,F, ,n\n"
                                               "1>2,3\n"
                                               "2>4,5\n"
                                               "3>6\n"
                                               "4>7\n"
                                               "5>8,9,10\n"
                                               "6\n"
                                               "7\n"
                                               "8\n"
                                               "9>11,12,13,14\n"
                                               "10>15\n"
                                               "11\n"
                                               "12\n"
                                               "13>16,17\n"
                                               "14\n"
                                               "15\n"
                                               "16>18,19\n"
                                               "17>20\n"
                                               "18>21,22\n"
                                               "19>23\n"
                                               "20\n"
                                               "21\n"
                                               "22>24\n"
                                               "23\n"
                                               "24\n");

    EXPECT_THROW(parse("3 / / 5"), std::runtime_error);
    EXPECT_THROW(parse("1 * 3 2 / 5"), std::runtime_error);
    EXPECT_THROW(parse(" / 5"), std::runtime_error);
    EXPECT_THROW(parse("3 *"), std::runtime_error);

    EXPECT_NO_THROW(parse("1 / 2 / 3"));
    EXPECT_NO_THROW(parse("4 * 8 * 12"));
    EXPECT_NO_THROW(parse("7 / 3 * 8"));
    EXPECT_NO_THROW(parse("9 * 2 / 11"));
}

// rules for E and E' nonterms
TEST(parser_tests, expression) {
    EXPECT_STREQ(e2ts("2 - 3 * 5").c_str(), "E,T,E',F,T',-,T,E',n, ,F,T', ,n,*,F,T',n, \n"
                                            "1>2,3\n"
                                            "2>4,5\n"
                                            "3>6,7,8\n"
                                            "4>9\n"
                                            "5>10\n"
                                            "6\n"
                                            "7>11,12\n"
                                            "8>13\n"
                                            "9\n"
                                            "10\n"
                                            "11>14\n"
                                            "12>15,16,17\n"
                                            "13\n"
                                            "14\n"
                                            "15\n"
                                            "16>18\n"
                                            "17>19\n"
                                            "18\n"
                                            "19\n");
    EXPECT_STREQ(e2ts("r(17+-2)/-0").c_str(),
                 "E,T,E',F,T', ,f,(,E,),/,F,T',T,E',-,F, ,F,T',+,T,E',n,n, ,F,T', ,-,F, ,n\n"
                 "1>2,3\n"
                 "2>4,5\n"
                 "3>6\n"
                 "4>7,8,9,10\n"
                 "5>11,12,13\n"
                 "6\n"
                 "7\n"
                 "8\n"
                 "9>14,15\n"
                 "10\n"
                 "11\n"
                 "12>16,17\n"
                 "13>18\n"
                 "14>19,20\n"
                 "15>21,22,23\n"
                 "16\n"
                 "17>24\n"
                 "18\n"
                 "19>25\n"
                 "20>26\n"
                 "21\n"
                 "22>27,28\n"
                 "23>29\n"
                 "24\n"
                 "25\n"
                 "26\n"
                 "27>30,31\n"
                 "28>32\n"
                 "29\n"
                 "30\n"
                 "31>33\n"
                 "32\n"
                 "33\n");

    EXPECT_THROW(parse(""), std::runtime_error);
    EXPECT_THROW(parse("+3"), std::runtime_error);
    EXPECT_THROW(parse("((5 + 8) 9 (-5 / -8))"), std::runtime_error);
    EXPECT_THROW(parse("1(2)"), std::runtime_error);
    EXPECT_THROW(parse("(2)3"), std::runtime_error);

    EXPECT_NO_THROW(parse("((f(g(137)) *- t (7)) +- (17 / 0 + 1))"));
}

// test for operators prioritizes
TEST(parser_tests, priority) {
    EXPECT_STREQ(e2ts("2 -3 + 5").c_str(), "E,T,E',F,T',-,T,E',n, ,F,T',+,T,E',n, ,F,T', ,n, \n"
                                           "1>2,3\n"
                                           "2>4,5\n"
                                           "3>6,7,8\n"
                                           "4>9\n"
                                           "5>10\n"
                                           "6\n"
                                           "7>11,12\n"
                                           "8>13,14,15\n"
                                           "9\n"
                                           "10\n"
                                           "11>16\n"
                                           "12>17\n"
                                           "13\n"
                                           "14>18,19\n"
                                           "15>20\n"
                                           "16\n"
                                           "17\n"
                                           "18>21\n"
                                           "19>22\n"
                                           "20\n"
                                           "21\n"
                                           "22\n");
    EXPECT_STREQ(e2ts("(3 +- 16) / 11 - 2").c_str(),
                 "E,T,E',F,T',-,T,E',(,E,),/,F,T',F,T', ,T,E',n, ,n, ,F,T',+,T,E',n, ,F,T', ,-,F, ,n\n"
                 "1>2,3\n"
                 "2>4,5\n"
                 "3>6,7,8\n"
                 "4>9,10,11\n"
                 "5>12,13,14\n"
                 "6\n"
                 "7>15,16\n"
                 "8>17\n"
                 "9\n"
                 "10>18,19\n"
                 "11\n"
                 "12\n"
                 "13>20\n"
                 "14>21\n"
                 "15>22\n"
                 "16>23\n"
                 "17\n"
                 "18>24,25\n"
                 "19>26,27,28\n"
                 "20\n"
                 "21\n"
                 "22\n"
                 "23\n"
                 "24>29\n"
                 "25>30\n"
                 "26\n"
                 "27>31,32\n"
                 "28>33\n"
                 "29\n"
                 "30\n"
                 "31>34,35\n"
                 "32>36\n"
                 "33\n"
                 "34\n"
                 "35>37\n"
                 "36\n"
                 "37\n");
}

TEST(parser_tests, modification) {
    EXPECT_NO_THROW(parse("3 + max(1 / 8, sin(5 + 7))"));
    EXPECT_NO_THROW(parse("f(g(), g(), 23)"));

    EXPECT_THROW(parse("2 * 3, 5 + 1"), std::runtime_error);
    EXPECT_THROW(parse("2, 1"), std::runtime_error);
    EXPECT_THROW(parse("max(1,,3)"), std::runtime_error);
    EXPECT_THROW(parse("min(1,)"), std::runtime_error);
}

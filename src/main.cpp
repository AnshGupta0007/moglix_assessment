// main.cpp
// Command-line driver for the Longest Valid Parentheses solution.
//
// Usage:
//   1) Argument mode : ./lvp "(()"          -> prints 2
//   2) Stdin mode    : echo ")()())" | ./lvp -> prints 4
//                      (reads one string per line, prints the answer per line)
//
// The heavy lifting lives in include/solution.hpp so it can be unit-tested and
// reused independently of this I/O shell.

#include <iostream>
#include <string>

#include "solution.hpp"

int main(int argc, char** argv) {
    // Argument mode: treat each CLI argument as an input string.
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            std::cout << lvp::longestValidParentheses(argv[i]) << '\n';
        }
        return 0;
    }

    // Stdin mode: one string per line.
    std::string line;
    while (std::getline(std::cin, line)) {
        std::cout << lvp::longestValidParentheses(line) << '\n';
    }
    return 0;
}

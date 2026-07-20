// test_solution.cpp
// A tiny, dependency-free unit-test harness. No external framework required —
// just compile and run. Exit code 0 means every assertion passed.
//
// It validates all THREE implementations against the same expected answers,
// then cross-checks them against a brute-force O(n^3) oracle on exhaustive
// short inputs and on randomized inputs. If any implementation ever disagrees,
// the test fails loudly with the offending input.

#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "solution.hpp"

namespace {

int g_checks = 0;
int g_failures = 0;

void check(bool cond, const std::string& what) {
    ++g_checks;
    if (!cond) {
        ++g_failures;
        std::cerr << "  [FAIL] " << what << '\n';
    }
}

// Reference oracle: brute force over every substring. O(n^3) but obviously
// correct — used only to validate the fast solutions on small inputs.
int brute(const std::string& s) {
    const int n = static_cast<int>(s.size());
    int best = 0;
    for (int i = 0; i < n; ++i) {
        int bal = 0;
        for (int j = i; j < n; ++j) {
            bal += (s[j] == '(') ? 1 : -1;
            if (bal < 0) break;               // can never recover -> stop
            if (bal == 0) best = std::max(best, j - i + 1);
        }
    }
    return best;
}

// Assert that all three production implementations agree with `expected`.
void expect(const std::string& s, int expected) {
    const int a = lvp::longestValidParentheses(s);
    const int b = lvp::longestValidParenthesesStack(s);
    const int c = lvp::longestValidParenthesesDP(s);
    check(a == expected, "counter(\"" + s + "\") == " + std::to_string(expected) +
                             " got " + std::to_string(a));
    check(b == expected, "stack(\"" + s + "\") == " + std::to_string(expected) +
                             " got " + std::to_string(b));
    check(c == expected, "dp(\"" + s + "\") == " + std::to_string(expected) +
                             " got " + std::to_string(c));
}

}  // namespace

int main() {
    std::cout << "Running Longest Valid Parentheses test suite...\n";

    // --- The three problem statement examples ------------------------------
    expect("(()", 2);
    expect(")()())", 4);
    expect("", 0);

    // --- Hand-picked edge cases --------------------------------------------
    expect("(", 0);
    expect(")", 0);
    expect("()", 2);
    expect(")(", 0);
    expect("()()", 4);
    expect("(())", 4);
    expect("()(()", 2);
    expect("()(())", 6);
    expect("((((((", 0);
    expect("))))))", 0);
    expect("()(((((", 2);
    expect(")()()(", 4);
    expect("(()())", 6);
    expect("()((())", 4);
    expect("(()))())(", 4);

    // --- Exhaustive: every string over {'(',')'} up to length 14 -----------
    for (int len = 0; len <= 14; ++len) {
        for (int mask = 0; mask < (1 << len); ++mask) {
            std::string s(len, '(');
            for (int b = 0; b < len; ++b)
                if (mask & (1 << b)) s[b] = ')';
            const int oracle = brute(s);
            check(lvp::longestValidParentheses(s) == oracle,
                  "exhaustive counter mismatch on \"" + s + "\"");
            check(lvp::longestValidParenthesesStack(s) == oracle,
                  "exhaustive stack mismatch on \"" + s + "\"");
            check(lvp::longestValidParenthesesDP(s) == oracle,
                  "exhaustive dp mismatch on \"" + s + "\"");
        }
    }

    // --- Randomized fuzz on longer inputs ----------------------------------
    std::mt19937 rng(12345);
    for (int trial = 0; trial < 20000; ++trial) {
        const int len = static_cast<int>(rng() % 200);
        std::string s(len, '(');
        for (int i = 0; i < len; ++i)
            if (rng() & 1) s[i] = ')';
        const int oracle = brute(s);
        check(lvp::longestValidParentheses(s) == oracle,
              "fuzz counter mismatch on \"" + s + "\"");
        check(lvp::longestValidParenthesesStack(s) == oracle,
              "fuzz stack mismatch on \"" + s + "\"");
        check(lvp::longestValidParenthesesDP(s) == oracle,
              "fuzz dp mismatch on \"" + s + "\"");
    }

    // --- Large valid string near the constraint upper bound ----------------
    {
        const int pairs = 30000;  // 60000 chars, above the 3*10^4 constraint
        std::string s(2 * pairs, '(');
        for (int i = pairs; i < 2 * pairs; ++i) s[i] = ')';  // "((...))"
        expect(s, 2 * pairs);
    }

    std::cout << g_checks << " checks run, " << g_failures << " failures.\n";
    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED \xE2\x9C\x94\n";
        return 0;
    }
    std::cout << "TESTS FAILED\n";
    return 1;
}

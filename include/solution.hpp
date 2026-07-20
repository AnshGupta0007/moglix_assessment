// solution.hpp
// Longest Valid Parentheses
//
// Given a string containing just the characters '(' and ')', return the
// length of the longest valid (well-formed) parentheses substring.
//
// This header exposes three independent, self-contained implementations so the
// same problem can be studied from three angles. All three return identical
// results; they differ only in time/space trade-offs and readability.
//
//   longestValidParentheses          -> two-pass counter  : O(n) time, O(1) space  (PRIMARY)
//   longestValidParenthesesStack     -> stack of indices  : O(n) time, O(n) space
//   longestValidParenthesesDP        -> dynamic programming: O(n) time, O(n) space
//
// Author: Ansh Gupta
// Language: C++17

#ifndef LONGEST_VALID_PARENTHESES_SOLUTION_HPP
#define LONGEST_VALID_PARENTHESES_SOLUTION_HPP

#include <algorithm>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

namespace lvp {

// ---------------------------------------------------------------------------
// PRIMARY SOLUTION — Two-pass counter.  O(n) time, O(1) extra space.
// ---------------------------------------------------------------------------
//
// Idea: A substring is valid iff at every prefix the number of ')' never
// exceeds the number of '(' AND the totals are equal.
//
// Scan left -> right keeping counts of '(' (open) and ')' (close):
//   * when open == close  -> we have a balanced run of length 2*close.
//   * when close > open    -> the run can never be repaired going right;
//                             reset both counters.
// A single left-to-right pass misses cases where '(' outnumber ')' forever,
// e.g. "(()". So we scan a second time right -> left with the roles of the
// counters swapped, resetting when open > close. The answer is the max seen
// across both passes.
//
// This uses only a handful of integer counters — truly O(1) auxiliary space.
inline int longestValidParentheses(std::string_view s) {
    int best = 0;
    int open = 0;   // count of '('
    int close = 0;  // count of ')'

    // Left -> right: catches every valid substring whose limiting factor is a
    // surplus of ')'.
    for (char c : s) {
        if (c == '(') {
            ++open;
        } else {
            ++close;
        }
        if (open == close) {
            best = std::max(best, 2 * close);
        } else if (close > open) {
            open = close = 0;
        }
    }

    // Right -> left: catches valid substrings that a forward scan can never
    // close out because '(' stays ahead (e.g. "(()").
    open = close = 0;
    for (auto it = s.rbegin(); it != s.rend(); ++it) {
        if (*it == '(') {
            ++open;
        } else {
            ++close;
        }
        if (open == close) {
            best = std::max(best, 2 * open);
        } else if (open > close) {
            open = close = 0;
        }
    }

    return best;
}

// ---------------------------------------------------------------------------
// ALTERNATIVE 1 — Stack of indices.  O(n) time, O(n) space.
// ---------------------------------------------------------------------------
//
// Keep a stack of indices. Seed it with -1 as a "base" boundary marking the
// position just before the current valid run.
//   * '(' -> push its index.
//   * ')' -> pop. If the stack becomes empty, this ')' is unmatched and
//            becomes the new base boundary (push its index). Otherwise the
//            valid substring runs from the index now on top of the stack to
//            the current index, i.e. length = i - stack.top().
inline int longestValidParenthesesStack(std::string_view s) {
    int best = 0;
    std::stack<int> st;
    st.push(-1);  // boundary before any valid substring

    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        if (s[i] == '(') {
            st.push(i);
        } else {
            st.pop();
            if (st.empty()) {
                st.push(i);  // unmatched ')': reset the boundary
            } else {
                best = std::max(best, i - st.top());
            }
        }
    }
    return best;
}

// ---------------------------------------------------------------------------
// ALTERNATIVE 2 — Dynamic programming.  O(n) time, O(n) space.
// ---------------------------------------------------------------------------
//
// dp[i] = length of the longest valid substring *ending exactly at* index i.
// A valid substring must end in ')', so dp[i] is 0 whenever s[i] == '('.
// For s[i] == ')':
//   * if s[i-1] == '('            -> "...()"  : dp[i] = dp[i-2] + 2
//   * else if the char before the run that ends at i-1 is '(' :
//           j = i - dp[i-1] - 1
//           if s[j] == '('        -> "...((...))" : dp[i] = dp[i-1] + 2 + dp[j-1]
inline int longestValidParenthesesDP(std::string_view s) {
    const int n = static_cast<int>(s.size());
    if (n < 2) return 0;

    std::vector<int> dp(n, 0);
    int best = 0;

    for (int i = 1; i < n; ++i) {
        if (s[i] != ')') continue;

        if (s[i - 1] == '(') {
            dp[i] = (i >= 2 ? dp[i - 2] : 0) + 2;
        } else {
            const int j = i - dp[i - 1] - 1;
            if (j >= 0 && s[j] == '(') {
                dp[i] = dp[i - 1] + 2 + (j >= 1 ? dp[j - 1] : 0);
            }
        }
        best = std::max(best, dp[i]);
    }
    return best;
}

}  // namespace lvp

#endif  // LONGEST_VALID_PARENTHESES_SOLUTION_HPP

# Longest Valid Parentheses

> Given a string containing just the characters `'('` and `')'`, return the
> length of the **longest valid (well-formed) parentheses substring**.

A clean, production-quality C++17 solution to the classic *Longest Valid
Parentheses* problem — solved three different ways, verified exhaustively, and
wired up with a CLI, a zero-dependency test suite, and continuous integration.

[![CI](https://github.com/OWNER/REPO/actions/workflows/ci.yml/badge.svg)](https://github.com/OWNER/REPO/actions/workflows/ci.yml)
![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)
![License: MIT](https://img.shields.io/badge/License-MIT-green)

---

## Problem

| Input | Output | Explanation |
|-------|:------:|-------------|
| `"(()"`     | `2` | The longest valid substring is `"()"`. |
| `")()())"`  | `4` | The longest valid substring is `"()()"`. |
| `""`        | `0` | Empty string, nothing to match. |

**Constraints**

- `0 <= s.length <= 3 * 10^4`
- `s[i]` is `'('` or `')'`.

---

## TL;DR — the optimal solution

The primary implementation runs in **O(n) time** using only **O(1) extra
space** — a handful of integer counters, no stack, no DP array.

```cpp
int longestValidParentheses(std::string_view s) {
    int best = 0, open = 0, close = 0;
    for (char c : s) {                       // left -> right
        c == '(' ? ++open : ++close;
        if (open == close)      best = std::max(best, 2 * close);
        else if (close > open)  open = close = 0;
    }
    open = close = 0;
    for (auto it = s.rbegin(); it != s.rend(); ++it) {  // right -> left
        *it == '(' ? ++open : ++close;
        if (open == close)      best = std::max(best, 2 * open);
        else if (open > close)  open = close = 0;
    }
    return best;
}
```

Full, commented source: [`include/solution.hpp`](include/solution.hpp).

---

## Three approaches, one answer

All three live in [`include/solution.hpp`](include/solution.hpp) and are checked
against each other (and against a brute-force oracle) by the test suite.

| Approach | Function | Time | Space | Notes |
|----------|----------|:----:|:-----:|-------|
| **Two-pass counter** ⭐ | `longestValidParentheses` | O(n) | **O(1)** | Optimal space. Two linear scans with swapped reset conditions. |
| Stack of indices | `longestValidParenthesesStack` | O(n) | O(n) | The most intuitive; a `-1` sentinel tracks the boundary before the current run. |
| Dynamic programming | `longestValidParenthesesDP` | O(n) | O(n) | `dp[i]` = length of the longest valid substring *ending at* `i`. |

### Why the two-pass counter needs two passes

A single left-to-right scan resets whenever `')'` outnumbers `'('`, which
correctly handles a surplus of closing brackets. But it can never "close out" a
run where `'('` stays permanently ahead — e.g. `"(()"`, where `open` is always
`>=` `close` so `open == close` never triggers on the final `()`. Scanning a
second time from the right, with the reset condition mirrored (`open > close`),
catches exactly those cases. The maximum over both passes is the answer.

---

## Build & run

Requires only a **C++17 compiler** (GCC, Clang, or MSVC). No third-party
libraries.

### Using `make`

```bash
make            # builds the CLI at bin/lvp
make test       # builds and runs the full test suite
make run        # runs the three example inputs
make clean      # removes build artifacts
```

### Using CMake

```bash
cmake -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

### Run the CLI

```bash
# As command-line arguments (one answer per argument):
./bin/lvp "(()" ")()())" ""
# 2
# 4
# 0

# Or piped via stdin (one string per line):
printf '(()\n)()())\n' | ./bin/lvp
# 2
# 4
```

---

## Testing

The suite in [`tests/test_solution.cpp`](tests/test_solution.cpp) is fully
self-contained (no GoogleTest / Catch dependency) and validates **all three
implementations** simultaneously:

1. **The three official examples** from the problem statement.
2. **Hand-picked edge cases** — empty string, single chars, `")("`, deeply
   nested, all-open, all-close, etc.
3. **Exhaustive verification** — *every* string over `{'(', ')'}` up to length
   14 (that's `2^15 - 1` strings) checked against an independent O(n³)
   brute-force oracle.
4. **Randomized fuzzing** — 20,000 random strings up to length 200, again
   cross-checked against the oracle.
5. **Scale test** — a 60,000-character string (above the constraint's upper
   bound) to confirm correctness and performance at the limit.

```bash
$ make test
Running Longest Valid Parentheses test suite...
... checks run, 0 failures.
ALL TESTS PASSED ✔
```

Every push runs this suite on **Ubuntu and macOS** via GitHub Actions
([`.github/workflows/ci.yml`](.github/workflows/ci.yml)).

---

## Project layout

```
.
├── include/solution.hpp        # The three implementations (header-only)
├── src/main.cpp                # CLI driver (args + stdin)
├── tests/test_solution.cpp     # Zero-dependency test + fuzz suite
├── CMakeLists.txt              # CMake build
├── Makefile                    # Plain make build
├── .github/workflows/ci.yml    # CI: build + test on Linux & macOS
├── LICENSE                     # MIT
└── README.md
```

---

## License

Released under the [MIT License](LICENSE).

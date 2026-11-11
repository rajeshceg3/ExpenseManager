---
name: verify-expenses
description: Build the ExpenseManager C++ CLI with conan+cmake and verify add, view, monthly summarize, save and load round-trip against data/expenses.csv.
---
# ExpenseManager CLI verification

## Launch
Toolchain: C++17 compiler, cmake 3.15+, conan 2 (`pip install --user conan`). Setup: `conan profile detect --force`, `conan install . --build=missing -of build`, `cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release`, `cmake --build build -j4`. Binary: `build/expense_tracker` (interactive menu on stdin/stdout).

## Doctor
`python3 .cursor/skills/verify-expenses/verify.py` (stdlib only). Drives the real binary: adds two expenses, views all, summarizes 2026-11 by month, saves, then reloads in a second process and re-views. Asserts every marker and the CSV round-trip in data/expenses.csv. Prints PASS/FAIL lines and FAILURES=0, exit 0 on green. Removes its test CSV afterwards.

## Evidence
Run output + the transient data/expenses.csv (3 rows: header + two expenses).

## Cleanup
verify.py deletes data/expenses.csv it created. build/ and CMakeUserPresets.json are local artifacts, not committed.

## Known boundaries
First build compiles conan deps from source when binaries are missing (fmt/date/fast-cpp-csv-parser plus openssl/libcurl for date's tz download); allow several minutes. No unit-test suite, no date-validation beyond sscanf, no hosted/CI claim. Delete flow and day/year/range summaries are not covered by this runner.

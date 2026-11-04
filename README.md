# ExpenseManager
## Verification

See `.cursor/skills/verify-expenses/SKILL.md` for isolated actual PTY journeys and evidence. `cmake -S . -B <owned-build> -DEXPENSE_HEADER_ROOT=<prefix>` optionally builds with real headers under `<prefix>/include` (fmt/date/csv), using header-only fmt and Threads. Default Conan package discovery is unchanged. This alternative was verified with distro fmt8.1.1/date3.0.1/libfccp2016; exact Conan-pinned versions remain unverified. Run only in a fresh working directory with its own `data/`, never against existing expense files.

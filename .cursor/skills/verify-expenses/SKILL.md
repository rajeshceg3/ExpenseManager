---
name: verify-expenses
description: Drive ExpenseManager's real C++ terminal menus in a PTY, verify quoted CSV persistence and failure-state preservation, summaries, deletion and EOF cleanup.
---
# Verify Expense Manager
## Launch
Interactive C++17CLI src/main.cpp plusExpense/ExpenseManager; data/expenses.csv relative to currentdirectory, noexternalAPI/auth. Neverrunagainstuser'sdata. Usefreshcwdwithdata/ eachtime. NativeCMake defaultsConanpinsfmt10.2.1/date3.0.1/fast-cpp-csv-parser0.1.3. Optional EXPENSE_HEADER_ROOT enablesactualdependencyheader mode FMT_HEADER_ONLY/Threads withfind_path, notstubs. ExecutedproofusesprivateunpackedUbuntu fmt8.1.1/date3.0.1/libfccp2016headers; exactConanpins/Conanpackage discoveryremainunverified. Do notcallthatdefaultbuildPASS.
Python3/pexpect,cmake,g++required. Example isolateddependencies: unpack libfmt-dev/libhowardhinnant-date-dev/libfccp-dev into ownprefix; no globalsysteminstall. HelperbuildsrealCMake/sourceinownedtempdir and launchesrealbinaryinuniquePTY/data directory. No existing expenses overwritten.
## Doctor
`python3 .cursor/skills/verify-expenses/helpers/verify.py doctor --headers /tmp/expense-runtime/usr --out .verification-evidence/expenses`
Checks actualheaders/compiler/CMake. Pythonpexpectmustbeinstalled inyourruntime. Substituteactualprefixcontaininginclude/fmt,include/date andinclude/csv.h orinclude/libfccp/csv.h.
## Drive
`python3 .cursor/skills/verify-expenses/helpers/verify.py drive --headers /tmp/expense-runtime/usr --out .verification-evidence/expenses`
RealPTY: emptyview,2adds,quotedcomma/doublequoteCSVsave/load/view, malformedandmissingloadpreservescurrentrecords,day/month/year/inclusiverange/reversedrange,invalid+validdelete,CSVrows,exit0. SeparateactualEOFinputmustexit1notspin. Filefixturechangesoccuronlyinownedcwd atordinarypersistenceboundary; nointernalsetters.
## Evidence
out/build.log,pty.log,saved.csv,doctor.json,drive.json survivecleanup. Inspectactualtranscript/CSV, notjustexit. SavedfixtureisfabricatedLunch/Train,notuserfinancialdata. No financialadvice. Thisdoesnotcertifyinvalidcalendardates/trailingdatejunk/nonfiniteamounts/multilineCSV/precision/security/concurrentwrites/crashatomicity. Thoseareremainingproductlimits.
## Cleanup
Helpercloses/reapsownedPTYevenfailure; uniquebuild/data removed,proofoutside retained. Gitstatusmustremainunchangedduringdrive. Neverdeleteexistingdata/runproductfromrepo data/ or usekillbyname.
## Helpers
Executablehelpers/verify.py usesPythonstdlib+pexpectandactualCMakeCLI. No externalnetwork. Featuremapfeatures/README.md. Use /maintain-verification-skill whenchangingmenus/persistence.

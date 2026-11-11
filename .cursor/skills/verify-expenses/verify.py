#!/usr/bin/env python3
"""Drive the built expense_tracker binary and verify add/view/summarize/save/load."""
import json, os, subprocess, sys, csv
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', '..'))
BIN = os.path.join(ROOT, 'build', 'expense_tracker')
CSV = os.path.join(ROOT, 'data', 'expenses.csv')
failures = []
def check(name, cond, detail=''):
    print(('PASS' if cond else 'FAIL'), name, detail)
    if not cond: failures.append(name)
if not os.path.exists(BIN):
    print('FAIL binary-missing build/expense_tracker not found - run conan install + cmake build first')
    sys.exit(1)
if os.path.exists(CSV): os.remove(CSV)
script = ('1\nTeam lunch\n450.50\n2026-11-10\nFood\nCash\n'
          '1\nAWS bill\n1200\n2026-11-11\nInfra\nCredit\n'
          '2\n6\n2\n2026\n11\n5\n4\n7\n')
r = subprocess.run([BIN], input=script, capture_output=True, text=True, cwd=ROOT, timeout=60)
out = r.stdout
check('run-exits-clean', r.returncode == 0, f'rc={r.returncode}')
check('add-expense', out.count('Expense added.') == 2)
check('view-all', 'Team lunch' in out and 'AWS bill' in out)
check('summarize-month', 'Expenses for 2026-11' in out and '450.50' in out and '1200.00' in out)
check('save-confirmed', 'Expenses saved to expenses.csv' in out)
check('csv-written', os.path.exists(CSV))
if os.path.exists(CSV):
    rows = list(csv.reader(open(CSV)))
    check('csv-roundtrip', len(rows) == 3 and rows[1][1] == 'Team lunch' and rows[2][1] == 'AWS bill', f'{len(rows)} rows')
r2 = subprocess.run([BIN], input='5\n2\n7\n', capture_output=True, text=True, cwd=ROOT, timeout=60)
check('load-roundtrip', 'Expenses loaded from expenses.csv' in r2.stdout and 'Team lunch' in r2.stdout)
if os.path.exists(CSV): os.remove(CSV)
print('FAILURES=%d' % len(failures))
sys.exit(0 if not failures else 1)

#!/usr/bin/env python3
import argparse,csv,json,pathlib,shutil,subprocess,tempfile
import pexpect
root=pathlib.Path(__file__).resolve().parents[4];p=argparse.ArgumentParser();p.add_argument('mode',choices=['doctor','drive']);p.add_argument('--headers',required=True);p.add_argument('--out',default='.verification-evidence/expenses');a=p.parse_args();out=pathlib.Path(a.out).resolve();out.mkdir(parents=True,exist_ok=True);r={'mode':a.mode,'ok':False,'checks':[]};child=None
try:
 headers=pathlib.Path(a.headers).resolve();assert all(shutil.which(x) for x in ['g++','cmake']),'Build tools missing';assert (headers/'include/fmt/core.h').is_file() and (headers/'include/date/date.h').is_file(),'Actual headers missing';assert any((headers/x).is_file() for x in ['include/csv.h','include/libfccp/csv.h']),'Actual CSVheaders missing';r['checks'].append('doctor actual source/dependencies available')
 if a.mode=='drive':
  before=subprocess.run(['git','status','--porcelain'],cwd=root,capture_output=True,text=True,check=True).stdout
  with tempfile.TemporaryDirectory(prefix='expenses-proof-') as td:
   td=pathlib.Path(td);build=td/'build';logs=[]
   for cmd in [['cmake','-S',str(root),'-B',str(build),'-DEXPENSE_HEADER_ROOT='+str(headers)],['cmake','--build',str(build)]]:
    q=subprocess.run(cmd,capture_output=True,text=True,timeout=45);logs.append(q.stdout+q.stderr);assert q.returncode==0,'Native CMake failed'
   (out/'build.log').write_text('\n'.join(logs));cwd=td/'session';(cwd/'data').mkdir(parents=True);binary=build/'expense_tracker'
   transcript=open(out/'pty.log','w');child=pexpect.spawn(str(binary),cwd=str(cwd),encoding='utf-8',timeout=5);child.logfile=transcript
   def menu(n):child.expect_exact('Enter your choice: ');child.sendline(str(n))
   def add(desc,amount,date,category,kind):
    menu(1)
    for prompt,value in [('Enter description: ',desc),('Enter amount: ',amount),('Enter date (YYYY-MM-DD): ',date),('Enter category: ',category),('Enter transaction type (Cash/Credit): ',kind)]:child.expect_exact(prompt);child.sendline(value)
    child.expect_exact('Expense added.')
   menu(2);child.expect_exact('No expenses recorded.')
   add('Lunch, "large"','12.5','2026-11-04','Food, "home"','Cash');add('Train','20','2026-10-31','Travel','Credit');r['checks'].append('actualPTY add2/emptyview')
   menu(4);child.expect_exact('Expenses saved to expenses.csv');saved=(cwd/'data/expenses.csv').read_text();rows=list(csv.reader(saved.splitlines()));assert rows[1][1]=='Lunch, "large"' and rows[1][3]=='Food, "home"' and len(rows)==3,'CSVescaping';(out/'saved.csv').write_text(saved)
   menu(5);child.expect_exact('Expenses loaded from expenses.csv');menu(2);child.expect_exact('Lunch, "large" | $12.5');child.expect_exact('Train | $20');r['checks'].append('quotedCSVactualsave/load/view')
   # Break only our own saved fixture while the real app remains live.
   (cwd/'data/expenses.csv').write_text('Date,Description,Amount,Category,Type\n2026-11-04,Bad,not-a-number,Food,Cash\n');menu(5);child.expect_exact('Failed to load expenses.');menu(2);child.expect_exact('Lunch, "large" | $12.5');child.expect_exact('Train | $20');(cwd/'data/expenses.csv').unlink();menu(5);child.expect_exact('Failed to load expenses.');menu(2);child.expect_exact('Lunch, "large" | $12.5');r['checks'].append('malformed+missingload preserve actual in-memory records')
   # Summary menu visits each real path, tests inclusive range and invalid range.
   menu(6)
   for choice,inputs,heading,expected in [(1,[('Enter date (YYYY-MM-DD): ','2026-11-04')],'--- Expenses for 2026-11-04 ---','Lunch, "large"'),(2,[('Enter year (YYYY): ','2026'),('Enter month (MM): ','10')],'--- Expenses for 2026-10 ---','Train'),(3,[('Enter year (YYYY): ','2026')],'--- Expenses for 2026 ---','Train'),(4,[('Enter start date (YYYY-MM-DD): ','2026-10-31'),('Enter end date (YYYY-MM-DD): ','2026-11-04')],'--- Expenses from 2026-10-31 to 2026-11-04 ---','Train')]:
    menu(choice)
    for prompt,value in inputs:child.expect_exact(prompt);child.sendline(value)
    child.expect_exact(heading);child.expect_exact(expected)
   menu(4);child.expect_exact('Enter start date (YYYY-MM-DD): ');child.sendline('2026-11-04');child.expect_exact('Enter end date (YYYY-MM-DD): ');child.sendline('2026-10-31');child.expect_exact('Start date cannot be after end date. Aborting.');menu(5);child.expect_exact('Returning to main menu...');r['checks'].append('day/month/year/inclusive-range and reversed-range actualmenus')
   menu(3);child.expect_exact('Enter index of expense to delete: ');child.sendline('99');child.expect_exact('Invalid index or expense not found.');menu(3);child.expect_exact('Enter index of expense to delete: ');child.sendline('0');child.expect_exact('Expense deleted.');menu(4);child.expect_exact('Expenses saved to expenses.csv');rows=list(csv.reader((cwd/'data/expenses.csv').read_text().splitlines()));assert len(rows)==2 and rows[1][1]=='Train','Deletepersistence';menu(7);child.expect_exact('Exiting application.');child.expect(pexpect.EOF);child.close();assert child.exitstatus==0,'CLIexit';child=None;transcript.close();r['checks'].append('invalid/validdelete and savedrows/exit0')
   precise='1\nPrecision\n12.3456789012345\n2026-11-04\nTest\nCash\n4\n7\n'
   q=subprocess.run([str(binary)],input=precise,capture_output=True,text=True,cwd=cwd,timeout=2);assert q.returncode==0;precise_rows=list(csv.reader((cwd/'data/expenses.csv').read_text().splitlines()));assert float(precise_rows[1][2])==12.3456789012345,'CSVprecision regression';r['checks'].append('saved amount double precision retained')
   q=subprocess.run([str(binary)],input='',capture_output=True,text=True,cwd=cwd,timeout=2);assert q.returncode==1 and q.stderr=='Input ended\n','EOFbusyloop';r['checks'].append('EOF exit1 no busyloop')
  after=subprocess.run(['git','status','--porcelain'],cwd=root,capture_output=True,text=True,check=True).stdout;assert before==after,'Sourcechanged';r['checks'].append('ownedPTY/build/data removed, proof retained, source unchanged')
 r['ok']=True
except Exception as e:r['failure']=str(e)
finally:
 if child:child.close(force=True)
(out/(a.mode+'.json')).write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r));raise SystemExit(0 if r['ok'] else 1)

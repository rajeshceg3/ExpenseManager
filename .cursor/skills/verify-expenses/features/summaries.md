# Date summaries
## Sub-features
Day/month/year/inclusivedaterange/reversedrangeguard.
## How to get to it (user POV)
Main6openssummarymenu,options1-4selectfilter,5returns. Enterdate/year/monthprompts.
## Driving it with verify.py
ActualtwofixturesOct31/Nov4:dayNov4Lunch,monthOctTrain,year2026Train,inclusiveOct31-Nov4Train;reversedrangeaborts. Observableheadingsandexpectedrowstested,notinternalmethodcalls.
## Gotchas
Testsdo notassertabsenceofeveryunmatchedroworallcalendarboundaries. Datesarebasicintegers,notdate::year_month_dayvalidated. No timezone/currencyanalytics/totalsclaim. Invalidcalendar/hugeyearinputremainfollowup.

# Final project for Embedded systems, 2021

## Compiling and running

To compile for the architecture of your computer, run `make embedded_final_hw` and then `./embedded_final_hw`

To compile for Raspberry pi zero, run `make raspberry`. The resulting executable is called `raspberry`

If the program is killed by SIGINT or SIGTERM, it will write its CPU time in the log file.

Calling `make` will create both executables. Calling `make clean` will delete them and also any file 
upload*.bin.

## Structure of project

* The header file `common.h` defines many constants, such as the speedup factor, and contains 
function definitions. If PRODUCTION is not defined, the program will write each log entry to STDOUT 
in human-readable form.

* The folders `run-pc/` and `run-ras/` contain the outputs of the program, run in a laptop or a Pi zero, 
respectively. The program was running for 432 minutes of real time, or 30 days of simulated time.

* Folder `converter` contains a C file which implements a decoder, from the binary raw log to a CSV. 
Then the log can be analysed with other tools such as Matlab or Excel.

* File `covid_utils.c` contains a predefiled list of pseudo-random MACs, and functions testCOVID, 
uploadContacts, and BTnearMe.

* File `log_time_utils.c` contains functions related to logging. In particular, setupTimeAndLog, 
printFuncCall, and printCpuTimeExit.

* File `data_structures.c` contains the structures which store the recent and close contacts. Also, 
related functions cleanOldCloseCont, overwriteRecentContact, isACloseContact, and newCloseContact.

* Finally, `embedded_final_hw.c` contains the top-level functions main and every10sec.

## Copyright

All rights reserved, 2021


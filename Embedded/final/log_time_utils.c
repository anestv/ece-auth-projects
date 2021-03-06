
#include <sys/time.h>
#include <sys/resource.h>
#include "common.h"


struct timeval programStartTime;

FILE * logFile;

void setupTimeAndLog(){
	
	gettimeofday(&programStartTime, NULL);
	
	logFile = fopen(LOG_FILENAME, "wb");
	
	if (logFile == NULL) {
		fprintf(stderr, "Error creating log file\n");
	}
	setvbuf(logFile, NULL, _IOFBF, 800); // Set buffer length to 800 bytes = 50 prints * 16 bytes/print
}

/* List of funcIds:
A : main				B : BTnearMe
C : newCloseContact		T : testCOVID
U : uploadContacts		Z : printCpuTimeExit
*/
void printFuncCall(const char funcId, const uint64_t data){

	struct timeval currTime;
	gettimeofday(&currTime, NULL);
	
	int32_t seconds  = currTime.tv_sec - programStartTime.tv_sec;
	int32_t useconds = currTime.tv_usec- programStartTime.tv_usec;
	
	if (useconds < 0){
		useconds += 1000000;
		seconds -= 1;
	}
	
	if (data >> 56)
		fprintf(stderr, "Warning: data has more than 56 bits. Those will be discarded\n");
	
	fwrite(&seconds, sizeof(int32_t), 1, logFile);
	fwrite(&useconds, sizeof(int32_t), 1, logFile);
	fwrite(&funcId, sizeof(char), 1, logFile);
	
	fwrite(&data, sizeof(uint64_t)-1, 1, logFile);
	
#ifndef PRODUCTION
	printf("%3ld.%06ld %c %llx\n", seconds, useconds, funcId, data);
#endif
}


void printCpuTimeExit(){

	struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	long usg_seconds = usage.ru_utime.tv_sec;
	long usg_useconds = usage.ru_utime.tv_usec;
	
	//printf("User CPU time: %ld.%06ld\nReal time passed:", usg_seconds, usg_useconds);
	printFuncCall('X', usg_seconds);
	printFuncCall('X', usg_useconds);
	
	printFuncCall('X', usage.ru_stime.tv_sec);
	printFuncCall('X', usage.ru_stime.tv_usec);
	
	fclose(logFile);
	
	//printf("i am %d\n", getpid());
	//char psCommand[40];
	//sprintf(psCommand, "ps --ppid=%d --format=time --no-headers", getpid());
	//system(psCommand);
	
	//char pidArg[14];
	//sprintf(pidArg, "--pid=%d", getpid());
	//execlp("ps", pidArg, "--format=time", "--no-headers", NULL);
	exit(0);
}

/*
για να βρουμε το CPU time μπρουμε να χρησιμοποιησουμε το προγραμμα ps --pid=1234...
Ακόμα καλύτερα, για να δείξει μόνο χρόνο και όχι headers:
ps --pid=1234 --format=time --no-headers
εξοδος: HH:MM:SS
*/


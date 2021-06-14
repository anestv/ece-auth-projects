
#include <sys/time.h>
#include <sys/resource.h>
#include "common.h"


struct timeval programStartTime;

void printTimeSinceStart(){
	
	struct timeval currTime;
	gettimeofday(&currTime, NULL);
	
	long seconds  = currTime.tv_sec - programStartTime.tv_sec;
	long useconds = currTime.tv_usec- programStartTime.tv_usec;
	
	if (useconds < 0){
		useconds += 1000000;
		seconds -= 1;
	}
	
	//TODO store in file as binary
	printf("%ld.%06ld\n", seconds, useconds); //TODO remove
	// printf(" %ld \n", currTime.tv_sec);
}


//#include <string.h>
void printCpuTimeExit(){

	struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	long usg_seconds = usage.ru_utime.tv_sec;
	long usg_useconds = usage.ru_utime.tv_usec;
	
	//TODO store in file as binary
	printf("User CPU time: %ld.%06ld\nReal time passed:", usg_seconds, usg_useconds);
	
	printTimeSinceStart();
	
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


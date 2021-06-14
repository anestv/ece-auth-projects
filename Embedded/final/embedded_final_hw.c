
#include <time.h>
#include <limits.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

#include "common.h"


void every10sec(int sig , siginfo_t *si, void *uc){
	
	printTimeSinceStart();
	
	
	static int secondsSinceLastTest = 0; //TODO maybe declare as global
	secondsSinceLastTest += 10;
	if (secondsSinceLastTest >= TESTING_INTERVAL_SECONDS){
		
		secondsSinceLastTest = 0;
		
		if (testCOVID()){
			// TODO notify close contacts
			printf("Covid test positive!\n");
		}
	}
	
	
	macaddress nearestMac = BTnearMe();
	
	printf("Scanned %lx\n", nearestMac);
	
	if (isACloseContact(nearestMac))
		newCloseContact(nearestMac);
	
	overwriteRecentContact(nearestMac);
	
	
	//printf("End of every10sec\n");
	//printTimeSinceStart();
	
	/*
	double dd;
	long long i = 1, pr = 1;
	for (; i < 5e7; i++){
		pr *= i;
		pr %= 10111000111;
		dd = (double)pr / rand();
	}
	printf("   %lld %lld %lf \n", i, pr, dd);*/
}

/*
https://linux.die.net/man/2/timer_settime
https://linux.die.net/man/2/timer_create

link with -lrt
*/

static inline void errExit(char* str){
	puts(str);
	exit(1);
}


extern struct timeval programStartTime;

int main(){
	printf("%d %x\n", RAND_MAX - INT_MAX, RAND_MAX);
	gettimeofday(&programStartTime, NULL);
	// store time, so that we can later find the time since start
	
	
	/* Establish handler for timer signal */
	/*printf("Establishing handler for signal %d\n", SIG);
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = every10sec; // the function to be called
	//sigemptyset(&sa.sa_mask);
	if (sigaction(SIG, &sa, NULL) != 0)
		errExit("sigaction");
	*/
	signal(SIG, every10sec);
	
	
	
	/* Create the timer */
	timer_t timerid;
	struct sigevent sev;
	const int CLOCKID = CLOCK_REALTIME; //TODO what to choose? #define?
	
	sev.sigev_notify = SIGEV_SIGNAL;//SIGEV_THREAD;//SIGEV_SIGNAL; // raise signal when timer expires
	sev.sigev_signo = SIG;
	//sev.sigev_value.sival_ptr = &timerid; // no need?
	if (timer_create(CLOCKID, &sev, &timerid) != 0)
		errExit("timer_create");
	
	//TODO we may need a different thread or process to handle the signals, because in main() sleeps..
	// maybe use SIGEV_THREAD
	
	/* Start the timer */
	struct itimerspec its;
	its.it_interval.tv_sec  = its.it_value.tv_sec  = BT_SCAN_PERIOD_SEC;
	its.it_interval.tv_nsec = its.it_value.tv_nsec = BT_SCAN_PERIOD_NSEC;

	if (timer_settime(timerid, 0, &its, NULL) != 0)
		errExit("timer_settime");
	
	
	// Signal handler: When Ctrl-C is pressed, display CPU time before exiting
	signal(SIGINT, printCpuTimeExit);
	signal(SIGTERM, printCpuTimeExit);
	
	printf("main is done\n");
	
	for (unsigned int t;;){
	
		//TODO what to do here?
		t = sleep(UINT_MAX);
		//printf("%u\n", t);
	}
	
	return 0;
}




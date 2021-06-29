
#include <limits.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

#include "common.h"


//TODO remove?
extern closecontact closeContacts[CLOSE_CONTACTS_LENGTH];
extern unsigned int closeContactsSize;

void every10sec(int sig /*, siginfo_t *si, void *uc*/){
	
	//printTimeSinceStart();
	
	
	static unsigned int periodsSinceLastTest = 0;
	periodsSinceLastTest ++;
	if (periodsSinceLastTest >= TESTING_INTERVAL_PERIODS){
		
		periodsSinceLastTest = 0;
		
		cleanOldCloseCont();
		
		// do a covid test
		if (testCOVID()){
		
			//printf("+ Covid test positive!\n");
			uploadContacts(closeContacts, closeContactsSize);
			
		//} else {
		//	printf("- Covid test negative\n");
		}
	}
	
	
	const macaddress nearestMac = BTnearMe();
	
	// printf("Scanned %llx\n", nearestMac);
	
	if (isACloseContact(nearestMac))
		newCloseContact(nearestMac);
	
	overwriteRecentContact(nearestMac);
	
	
	//printf("End of every10sec\n");
	//printTimeSinceStart();
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


//extern struct timeval programStartTime;

int main(){
	// store time, so that we can later find the time since start
	setupTimeAndLog();
	
	printFuncCall('A', 0); // program start
	
	/*for (int i = 0; i < 10; i++){
		
		printFuncCall('B', BTnearMe());
		sleep(1);
	}
	
	// TODO return these to normal
	printFuncCall('A', (uint64_t)0xFFFFFFFFFFFFF<<(uint64_t)1);
	//printf("%d  %d\n", sizeof(long long), sizeof(uint64_t));
	closeLogFile();
	return 0;*/
	
	/* Establish handler for timer signal */
	/* struct sigaction sa;
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
	
	sev.sigev_notify = SIGEV_SIGNAL;//SIGEV_THREAD;//SIGEV_SIGNAL; // raise signal when timer expires
	sev.sigev_signo = SIG;
	//sev.sigev_value.sival_ptr = &timerid; // no need?
	if (timer_create(CLOCK_REALTIME, &sev, &timerid) != 0)
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
	
	
	printFuncCall('A', 1);
	//printf("main is done\n"); // TODO remove
	
	for (unsigned int t;;){
	
		// do nothing and wait for interrupts
		t = sleep(UINT_MAX);
		//printf("%u\n", t);
	}
	
	return 0;
}




#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define SPEEDUP 40 // TODO revert to 100
// run this much faster

#define SEC_PER_PERIOD 10
#define BT_SCAN_PERIOD_SEC (SEC_PER_PERIOD / SPEEDUP)
#define BT_SCAN_PERIOD_NSEC ((SEC_PER_PERIOD * (1000000000 / SPEEDUP)) % 1000000000)

#define PROB_COVID_PERCENT 80 // TODO revert to 5 or 10
// the (supposed) probability that the user tests positive for covid

#define MAC_PRESET_LIST_LENGTH 100

#define CLOSE_CONTACTS_LENGTH 100

#define MAX_SECONDS_FOR_CLOSECONT 1200
#define RECENT_CONTACTS_LENGTH (MAX_SECONDS_FOR_CLOSECONT / SEC_PER_PERIOD)
// 20*60 seconds / 10 seconds = 120

#define MIN_SECONDS_FOR_CLOSECONT 240
#define MIN_PERIODS_FOR_CLOSECONT (MIN_SECONDS_FOR_CLOSECONT / SEC_PER_PERIOD)
// 4 minutes / 10 seconds = 24 periods

#define MAX_SECONDS_REMEMBER_CLOSECONT 1209600
#define MAX_REALTIME_REMEMBER_CLOSECONT (MAX_SECONDS_REMEMBER_CLOSECONT / SPEEDUP)
// 14 days * 24*60*60 = 1209600 seconds

#define TESTING_INTERVAL_SECONDS 300// TODO revert to 14400
#define TESTING_INTERVAL_PERIODS (TESTING_INTERVAL_SECONDS / SEC_PER_PERIOD)
// 4 hours = 4*60*60 seconds. / 10 seconds = 1440 periods

#define SIG SIGRTMIN // could also be SIGUSR1
// which signal to use for 10 second timer


typedef uint64_t macaddress;
typedef uint8_t bool;

typedef struct {
	macaddress mac;
	time_t lastSeen;
} closecontact;

// covid_utils.c

bool testCOVID();
macaddress BTnearMe();
void uploadContacts(const closecontact * const, int);
// Notice: The uploadContacts() signature is different than the suggested

void cleanOldCloseCont();
bool isACloseContact(macaddress);
void newCloseContact(macaddress);
void overwriteRecentContact(macaddress);


// time_utils.c

void printTimeSinceStart();
void printCpuTimeExit();


#endif


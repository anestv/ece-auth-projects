
#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>


#define BT_SCAN_PERIOD_SEC 0 //TODO revert to 10
#define BT_SCAN_PERIOD_NSEC 5e8

#define PROB_COVID_PERCENT 5
// the (supposed) probability that the user tests positive for covid

#define MAC_PRESET_LIST_LENGTH 100

#define CLOSE_CONTACTS_LENGTH 100

#define RECENT_CONTACTS_LENGTH 120
// 20*60 seconds / 10 seconds = 120

#define MIN_PERIODS_FOR_CLOSECONT 24
// 4 minutes / 10 seconds = 24 periods

#define TESTING_INTERVAL_SECONDS 14400
// 4 hours = 4*60*60 seconds

#define SIG SIGRTMIN // could also be SIGUSR1
// which signal to use for 10 second timer


typedef uint64_t macaddress;
typedef uint8_t bool;

// covid_utils.c

bool testCOVID();
macaddress BTnearMe();
void uploadContacts(macaddress*, int);

bool isACloseContact(macaddress);
void newCloseContact(macaddress);
void overwriteRecentContact(macaddress);


// time_utils.c

void printTimeSinceStart();
void printCpuTimeExit();


#endif


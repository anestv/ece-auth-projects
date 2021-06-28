
#include "common.h"

closecontact closeContacts[CLOSE_CONTACTS_LENGTH];
unsigned int closeContactsSize = 0;

macaddress recentContacts[RECENT_CONTACTS_LENGTH];
unsigned int recentContactsIdx = 0;

/*
circular array recentContacts : (mac, ---seen at time--- δε χρειάζεται καν. Μπορούμε να
το βρούμε με βάση την απόσταση από το τέλος της λίστας * 10 sec)
Αν το ιδιο ερθει ξανά, το ξανααποθηκεύουμε.

closeContacts : (mac, last seen on)
Αν το ιδιο ερθει ξανά, ενημερώνουμε το last seen on

Κάθε 1 μέρα, μπορούμε να σβήνουμε όσα έχουν lastSeen < now - 14days
*/

void cleanOldCloseCont(){
	
	const time_t currTime = time(NULL); // current datetime in seconds since epoch
	// time() could cause an error because it gives the actual time, not sped-up
	// This is why we use MAX_REALTIME_REMEMBER_CLOSECONT , which is 100 times less
	
	unsigned int writePos = 0, readPos = 0;
	
	while (readPos < closeContactsSize){
	
		if (closeContacts[readPos].lastSeen < currTime - MAX_REALTIME_REMEMBER_CLOSECONT) {
			// no longer a close contact. Discard it.
			// Next readPos, same writePos
			readPos++;
			
		} else {
			// still a close contact.
			// move it forward, skipping discarded places
			
			// TODO is this correct?
			closeContacts[writePos] = closeContacts[readPos];
			writePos++;
			readPos++;
		}
	}
	
	closeContactsSize = writePos;
}


void overwriteRecentContact(const macaddress nearestMac){
	
	recentContactsIdx++;
	if (recentContactsIdx == RECENT_CONTACTS_LENGTH)
		recentContactsIdx = 0;
	
	recentContacts[recentContactsIdx] = nearestMac;
}

bool isACloseContact(const macaddress testMac){
// Performs a linear search in circular array recentContacts
// Time seen > 4min, that's why we skip interval [currIndex - 23 , currIndex]

// currIndex must be skipped, it contains the previous mac, not the current.

	const unsigned int currIndex = recentContactsIdx;
	
	int start = 0, end = RECENT_CONTACTS_LENGTH;
	
	// if the interval contains 0, adjust the start and end of loop
	if (currIndex < MIN_PERIODS_FOR_CLOSECONT){
		start = currIndex + 1;
		end = (currIndex - MIN_PERIODS_FOR_CLOSECONT + 1) + RECENT_CONTACTS_LENGTH;
	}
	
	
	for (int i = start; i < end; i++){
		
		if (i >= currIndex - MIN_PERIODS_FOR_CLOSECONT + 1 && i <= currIndex){
			i = currIndex + 1; // skip those. No need to call 'continue'
			
			if (i >= RECENT_CONTACTS_LENGTH) // end of list. would be an error
				break;
		}
		if (recentContacts[i] == testMac)
			return 1;
	}
	
	return 0;
}

void newCloseContact(const macaddress newMac){
	// add (mac, currTime) to closeContacts, sorted
	
	printf("Close contact! %llx\n", newMac);
	
	const time_t currTime = time(NULL); // current time in seconds since epoch
	
	if (closeContactsSize == 0){
	
		closeContacts[0].mac = newMac;
		closeContacts[0].lastSeen = currTime;
		
		closeContactsSize = 1;
		return;
	}
	
	// binary search to see if newMac exists
	int lo = 0, hi = closeContactsSize - 1;
	int pos = (lo + hi) / 2;
	while (lo <= hi && closeContacts[pos].mac != newMac){
		
		if (newMac < closeContacts[pos].mac)
			hi = pos - 1;
		else
			lo = pos + 1;
		
		pos = (lo + hi) / 2;
	}
	
	if (closeContacts[pos].mac == newMac){
		// it already exists!
		closeContacts[pos].lastSeen = currTime;
	
	} else {
		// are we in the right place to insert mac?
		// we have lo = hi + 1. Should insert between hi and lo, so at lo.
		
		for (pos = closeContactsSize; pos > lo; pos--)
			closeContacts[pos] = closeContacts[pos - 1];
			// move them one place right
		
		closeContacts[lo].mac = newMac;
		closeContacts[lo].lastSeen = currTime;
		printf(" at pos %d\n", lo);
		
		closeContactsSize++;
	}
}


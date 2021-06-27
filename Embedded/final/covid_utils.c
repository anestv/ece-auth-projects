
#include "common.h"

const macaddress macAddrList[] = {0x9235d08e69ab, 0x0ef0abe3f96a, 0x47cc2ff51178, 0xdec2be41f22f, 
0xb8400b1f648d, 0x03ad5455465d, 0xe20e46dad05c, 0x3bbd87abf147, 0x1676b8362223, 0x8dde7e032239, 
0x36089ae02fd5, 0xe2771882af92, 0xed2be18be178, 0xb7241086bfa5, 0x8861159a7979, 0x811187e214fc, 
0x71bedee506af, 0x5442663818d8, 0x8abb91640aaa, 0x67755d3d0ee7, 0x46daaf1f90f5, 0x4600dd01b71b, 
0xb670b5695578, 0xac8a62a87efe, 0x60c7cdfaeb6f, 0x60abe1aa06c5, 0x3b63252ffc32, 0x4c8edd75df3d, 
0x4f294272bbf5, 0x58133208920d, 0xe417c90eb164, 0x957098296b48, 0x25413ad3c62f, 0x792d7998cd12, 
0xa91e473cd248, 0x5788ca6e3463, 0x27511deed177, 0xdc4a2000f907, 0xecec89c5eb52, 0x6d61f1f93a76, 
0x7215c626be8a, 0x89a54dbd1699, 0x413f2a875c82, 0x089db4bd8218, 0xbc74aa1d590b, 0x967d0d4b220a, 
0x65eecb7b8e50, 0x1436b82ece7f, 0x70aaba742f65, 0x8edd0eb88184, 0x3c5b9ff21da0, 0xf05a26a51feb, 
0xf848d27e0c8d, 0x88714823129e, 0x638df6b2fca2, 0x760ae7ccdbbf, 0x69557291b296, 0xa333a5fab9e5, 
0x1e06a4025d97, 0x15639071ed00, 0x370a61fca2c6, 0xb041c368f028, 0x4f874de6936f, 0x18925d4fade6, 
0x3a31a275e396, 0x66cf592a747a, 0xa51fc23f13e3, 0x7bceccc4f8db, 0xd54c3b17e938, 0x1a71c7e25584, 
0xc17fa0f16ac3, 0x181601916e68, 0x9c3748965fb9, 0x1eec0b8b3af3, 0x127b0fd8ff72, 0x06232dc70034, 
0x835586d7ceb1, 0x7539546fcbc2, 0xcb6cfcf21254, 0xbb4ffcd95f72, 0x5f23795eeb06, 0x4131573ca338, 
0x106d264aa69d, 0x71c6a3fa11ae, 0x0bf3bc9eaaa7, 0xbd35d3df1020, 0x2f4ecaa2c0ef, 0x9c922908399b, 
0xc808c177a7a1, 0xbdae97fd8ec8, 0xc871fac42e65, 0x7e4f7e9f0123, 0x186c96f68023, 0x11686eaa9284, 
0xe89b4d410a4c, 0x4183196bda57, 0x6448e1d90fff, 0x1053d341bd54, 0x7134432ce429, 0xa13535573325 };


closecontact closeContacts[CLOSE_CONTACTS_LENGTH];
unsigned int closeContactsSize = 0;

macaddress recentContacts[RECENT_CONTACTS_LENGTH];
unsigned int recentContactsIdx = 0;

bool testCOVID(){
	
	return (rand() % 100) < PROB_COVID_PERCENT;
}

macaddress BTnearMe(){
	/* say we want 300 close contacts in 14 days. There will be 14*24*3600/10 = 120960 scans
	This is 0.25% chance. = 50% * 50% * 1/100 (chance to hit TODO find the maths..... */
	
	int randi = rand();
	
	if (randi & 1) // 50-50
		return macAddrList[(randi >> 1) % MAC_PRESET_LIST_LENGTH];
	else
		return (randi >> 1);
	
	
	//int index = rand() % MAC_PRESET_LIST_LENGTH;
	//return macAddrList[index];
}

// Notice: The funtion signature is different than the suggested
void uploadContacts(const closecontact* const closeContacts, const int CClength){ // TODO what is the int for? size perhaps?
	
	char filename[30]; // 10 digits + 10 chars + \0 + extra
	sprintf(filename, "upload%10ld.bin", time(NULL));
	
	FILE *file = fopen(filename, "wb");
	
	if (file == NULL) {
        fprintf(stderr,"Error writing upload to file\n");
        return;
    }
    
    fwrite(&CClength, sizeof(int), 1, file);
    fwrite(closeContacts, sizeof(closecontact), CClength, file);
    
    fclose(file);
}


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

/*
circular array recentContacts : (mac, ---seen at time--- δε χρειάζεται καν. Μπορούμε να 
το βρούμε με βάση την απόσταση από το τέλος της λίστας * 10 sec)
Αν το ιδιο ερθει ξανά, το ξανααποθηκεύουμε.

closeContacts : (mac, last seen on) 
Αν το ιδιο ερθει ξανά, ενημερώνουμε το last seen on

Κάθε 1 μέρα, μπορούμε να σβήνουμε όσα έχουν lastSeen < now - 14days

*/


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


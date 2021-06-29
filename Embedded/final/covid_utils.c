
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


bool testCOVID(){
	
	const bool res = (rand() % 100) < PROB_COVID_PERCENT;
	
	printFuncCall('T', res);
	
	return res;
}

macaddress BTnearMe(){
	/* say we want 300 close contacts in 14 days. There will be 14*24*3600/10 = 120960 scans
	This is 0.25% chance. = 50% * 50% * 1/100 (chance to hit TODO find the maths..... */
	
	int randi = rand();
	
	macaddress res;
	
	if (randi & 1) // 50-50
		res = macAddrList[(randi >> 1) % MAC_PRESET_LIST_LENGTH];
	else
		res = (randi >> 1);
	
	printFuncCall('B', res);
	
	return res;
}

// Notice: The funtion signature is different than the suggested
void uploadContacts(const closecontact* const closeContacts, const int CClength){
	
	printFuncCall('U', CClength);
	
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


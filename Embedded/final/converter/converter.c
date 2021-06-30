
#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char** argv){
	
	if (argc != 2){
		printf("Error. Argument required: filename to change\n");
		return 1;
	}
	
	FILE * fbin = fopen(argv[1], "rb");
	FILE * fcsv = fopen(strcat(argv[1], ".csv"), "w");
	
	if (fbin == NULL || fcsv == NULL){
		printf("Error opening the files\n");
		return 1;
	}
	
	fprintf(fcsv, "time,funcId,data\n");
	
	unsigned int time[2];
	char funcId;
	uint64_t data;
	
	while (1){
		int nread = fread(time, sizeof(int), 2, fbin);
		
		if (nread < 2) // did not read 2 ints
			break;
		
		fread(&funcId, sizeof(char), 1, fbin);
		fread(&data, sizeof(uint64_t)-1, 1, fbin);
		
		fprintf(fcsv, "%u.%06d,%c,0x%llx\n", time[0], time[1], funcId, data);
	}
	
	fclose(fbin);
	fclose(fcsv);
	
	printf("Done!\n");
	
	return 0;
}


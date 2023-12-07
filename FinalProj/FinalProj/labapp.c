#include "zoomrecs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if( argc != 3) {
        	puts("Usage ./labcapp <inputcsv> <outputcsv>");
        	return EXIT_FAILURE;
	}

	char* input_filename = argv[1];
	char* output_filename = argv[2];

	FILE *inputf = fopen(input_filename, "rt");
        FILE *outputf  = fopen(output_filename, "wt");

	if (inputf == NULL){
        	printf("Error! Unable to open the input file %s", input_filename);
        	fclose(inputf);
        	return EXIT_FAILURE;
    	}

    	if (outputf == NULL){
        	printf("Error! Unable to open the output file %s", output_filename);
        	fclose(outputf);
        	return EXIT_FAILURE;
    	}

    	struct ZoomRecord *head = csvToList(inputf);

    	if (head == NULL) {
        	printf("Error! Input CSV file %s is empty", input_filename);
        	return EXIT_FAILURE;
    	}

	generateAttendance(head, outputf);

    	freeList(head);
    	fclose(inputf); fclose(outputf);

	return EXIT_SUCCESS;
}

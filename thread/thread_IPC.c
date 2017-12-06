/*
2017 System Programming
Assignment #3
Thread Ver.
*/
#include "ku_tsort.h"

int main(int argc, char *argv[]){
	FILE *fp;
	int num_of_inputs;		//argv[1]; m
	int num_of_threads;		//argv[2]; n
	char buffer[SIZE];
	int i, cnt= 0;
	
	/* ./ku_tsort.c m n input.txt output.txt */
	if(argc < 5)
		exit(0);
	num_of_inputs = atoi(argv[1]);
	num_of_threads = atoi(argv[2]);

	/* read ouinput.txt */
	fp = fopen(argv[3], "r");
	if(fp == NULL){
		perror("!!! File Open Error !!!\n");
	    exit(0);
	}

	/* save all line of input.txt at input[m] */
	int input[num_of_inputs];
	while(fgets(buffer,SIZE,fp)){
		if(feof(fp))
			break;
		input[cnt] = atoi(buffer);
		cnt++;
	}
	fclose(fp);

	/* merge_sorted and create threads */
	merge_sort(input, num_of_inputs, num_of_threads);

	
	/* write output.txt */
	fp = fopen(argv[4], "w");
	for(i = 0; i<num_of_inputs; i++){
		/* sorted data convert into string */
		sprintf(buffer, "%d", input[i]);
		strcat(buffer, "\n");
		fputs(buffer, fp);
	}		
	fclose(fp);

	return 0;
}

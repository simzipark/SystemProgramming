/*
2017 System Programming
Assignment #3
Fork Ver.
*/
#include "ku_psort.h"

int main(int argc, char *argv[]){
    FILE *fp;
	int num_of_inputs;		//argv[1]; m
	int num_of_childs;		//argv[2]; n
	int state;				//for waitpid
	int result;				//for msgXXX perror
	int quota;				//number that each child have to sorting work
	
	char buffer[SIZE];		//for file i/o
    int i, cnt = 0;			//index
	
	/* ./ku_tsort.c m n input.txt output.txt */
	if(argc < 5)
		exit(0);
	num_of_inputs = atoi(argv[1]);
	num_of_childs = atoi(argv[2]);
	
	if(num_of_childs > num_of_inputs)
		num_of_childs = num_of_inputs;

	quota = num_of_inputs / num_of_childs;	
	
	/* for fork */
	key_t key = (key_t)1994;
	int id;					
	struct{
		int start;
		int end;
		int data[num_of_inputs];
	}msg;

    /* read ouinput.txt to */
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

    /* msg initializaion */
	id = msgget(key, IPC_CREAT|0666);	//open message queue
	msg.start = 0;
	msg.end = quota-1;

	/* copy input to msg.data */
	for(i = 0; i<num_of_inputs; i++)
		msg.data[i] = input[i];

	/* create childs */
	int childs[num_of_childs];
	childs[num_of_childs] = 201410081;
	for(i = 0; i<num_of_childs; i++){
		childs[i] = fork();
		/* parent keep creating child */
		if(childs[i] > 0)
			break;
	}

	/* parent wait all childs */
	waitpid(childs[i], &state, 0);

	/* if user input argv[2] == 1 */
	if(num_of_childs == 1 && childs[0]>0){
		quick_sort(msg.data, msg.start, msg.end);

		result = msgsnd(id, &msg, sizeof(msg)-sizeof(long),0);
		if(result == -1){
			perror("!!! msgsnd Fail !!!\n");
		   	exit(1);
		}
	}
	/* process for not final child nor ancestor */
	else if(childs[i]!=201410081 && childs[i-1]==0 && i!=0){

		result = msgrcv(id, &msg, sizeof(msg),0,0);
		if(result == -1){
			perror("!!! msgrcv Fail !!!\n");
		   	exit(1);
		}

		quick_sort(msg.data, msg.start, msg.end);
		quick_sort(msg.data, 0, msg.end);

		msg.start = msg.end+1;
		msg.end = msg.start+quota;

		if(msg.end > num_of_inputs)
			msg.end = num_of_inputs-1;

		result = msgsnd(id, &msg, sizeof(msg)-sizeof(long),0);
		if(result == -1){
			perror("!!! msgsnd Fail !!!\n");
		   	exit(1);
		}
		exit(0);
	}
	/* process for final child */
	else if(childs[i]==201410081){

		quick_sort(msg.data, msg.start, msg.end);

		msg.start = msg.end+1;
		msg.end = msg.start+quota;

		result = msgsnd(id, &msg, sizeof(msg),0);
		if(result == -1){
			perror("!!! msgsnd Fail !!!\n");
		   	exit(1);
		}
		exit(0);
	}
	/* process for ancestor */
	else{
		result = msgrcv(id, &msg, sizeof(msg)-sizeof(long),0,0);
		if(result == -1){
			perror("!!! msgrcv Fail !!!\n");
		   	exit(1);
		}
	}

	/* write output.txt */
	fp = fopen(argv[4], "w");
	for(i = 0; i<num_of_inputs; i++){
		/* sorted data convert into string */
		sprintf(buffer, "%d", msg.data[i]);

		strcat(buffer, "\n");
		fputs(buffer, fp);
	}
	fclose(fp);

	/* delete message queue */
	msgctl(id, IPC_RMID, 0);
	return 0;
}

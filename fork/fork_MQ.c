/*
2017 System Programming
Assignment #3
Fork Ver.
*/
#include "ku_psort.h"

#define NAME_POSIX "/msg_queue"

int main(int argc, char *argv[]){
    FILE *fp;
	int num_of_inputs;		//argv[1]; m
	int num_of_childs;		//argv[2]; n
	int state;			//for waitpid
	int quota;			//number that each child have to sorting work
	
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


    /* message queue initializaion */
	mqd_t mqdes;
	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(msg);
	mqdes = mq_open(NAME_POSIX, O_CREAT|O_RDWR, 0644, &attr);

	if((mqdes == (mqd_t)-1)){
		perror("Message open error!");
		exit(1);
	}


    /* msg initializaion */
	msg.start = 0;
	msg.end = quota-1;
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

		if(mq_send(mqdes,(const char *)&msg, sizeof(msg), 0) == -1){
			perror("send error\n");
			exit(1);
		}
	}
	/* process for not final child nor ancestor */
	else if(childs[i]!=201410081 && childs[i-1]==0 && i!=0){

		if(mq_receive(mqdes,(char *)&msg, sizeof(msg)+1, NULL) == -1){
			perror("receive error\n");
			exit(1);
		}

		quick_sort(msg.data, msg.start, msg.end);
		quick_sort(msg.data, 0, msg.end);

		msg.start = msg.end+1;
		msg.end = msg.start+quota;

		if(msg.end > num_of_inputs)
			msg.end = num_of_inputs-1;

		if(mq_send(mqdes,(const char *)&msg, sizeof(msg), 0) == -1){
			perror("send error\n");
			exit(1);
		}
		exit(0);
	}
	/* process for final child */
	else if(childs[i]==201410081){
		quick_sort(msg.data, msg.start, msg.end);

		msg.start = msg.end+1;
		msg.end = msg.start+quota;

		if(mq_send(mqdes,(const char *)&msg, sizeof(msg), 0) == -1){
			perror("send error\n");
			exit(1);
		}
		exit(0);
	}
	/* process for ancestor */
	else{
		if(mq_receive(mqdes,(char *)&msg, sizeof(msg)+1, NULL) == -1){
			perror("receive error\n");
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
	close(mqdes);
	mq_unlink(NAME_POSIX);

	return 0;
}

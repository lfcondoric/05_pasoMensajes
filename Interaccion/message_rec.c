#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MSGSZ 128

typedef struct msgbuf{
	long mtype;
	char mtext[MSGSZ];
} message_buf;

int main(){
	int msqid;
	key_t key;
	message_buf rbuf, sbuf;
	size_t buf_length;

	key = 1234;

	if((msqid=msgget(key, 0666))<0){
		perror("msgget");
		exit(1);
	}
	//Procesi 2: Recive
	if(msgrcv(msqid, &rbuf, MSGSZ, 1, 0)<0){
		perror("msgrcv");
		exit(1);
	}
	printf("%s\n", rbuf.mtext);
	//Proceso 2: envia
	sbuf.mtype = 2;
	(void)strcpy(sbuf.mtext, "Process 2: Loud and Clear");
	buf_length = strlen(sbuf.mtext) + 1;

	if(msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT)<0){
		perror("msgsnd");
		exit(1);
	}

	//Proceso 2: recive
	if(msgrcv(msqid, &rbuf, MSGSZ, 3, 0)<0){
		perror("msgrcv");
		exit(1);
	}
	printf("%s\n", rbuf.mtext);

	exit(0);
}


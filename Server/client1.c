#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main() {
    int msqid;
    key_t key = 1234;
    message_buf sbuf, rbuf;

    if((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    // cliente 1 envia mensaje al servidor ( mtype = 10 )
    sbuf.mtype = 10;
    strcpy(sbuf.mtext, "Peticion enviada por Cliente 1");
    if(msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {
        perror("msgsnd");
        exit(1);
    }

    // cliete 1 recive respuesta del servidor ( mtype = 11 )
    if(msgrcv(msqid, &rbuf, MSGSZ, 11, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }
    printf("%s\n",rbuf.mtext);
    exit(0);
}

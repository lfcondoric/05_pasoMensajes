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
    message_buf rbuf, sbuf;

    if((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    // Servidor atiende al cliente 1 (mtype = 10)
    if(msgrcv(msqid, &rbuf, MSGSZ, 10, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }

    printf("Servidor recibio del Cliente 1: %s\n", rbuf.mtext);

    sbuf.mtype = 11;
    strcpy(sbuf.mtext, "Hola Cliente 1, recibi tu mensaje privado.");
    if(msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {
        perror("msgsnd");
        exit(1);
    }

    // Servidor atiende al cleinte 22 (mtype = 20)
    if(msgrcv(msqid, &rbuf, MSGSZ, 20, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }
    printf("Servidor recibio del Cliente 2: %s\n", rbuf.mtext);

    sbuf.mtype = 21;
    strcpy(sbuf.mtext, "Hola Cliente 2, recibi tu mensaje privado.");
    if(msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {
        perror("msgsnd");
        exit(1);
    }

    exit(0);
}

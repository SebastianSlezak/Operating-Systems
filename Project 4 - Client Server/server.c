#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <ctype.h>
#include <string.h>

#include <unistd.h>
#include <string.h>

#define MAX 80
#define SERVER 1
char temp[15];
int takePID(char[]);
void sig_hand(int);
struct message
{
    long mtype;
    char mtext[MAX];
};
int main(int argc, char *argv[])
{
    char *text;
    int msize;
    int i, pid;
    key_t key;
    int queueID;
    struct message mes;

    key = ftok(".", 33);
    if ((queueID = msgget(key, IPC_CREAT | 0660)) == -1)
    {
        perror("msgget() error:...");
        exit(1);
    }
    signal(SIGCLD, SIG_IGN);
    signal(SIGINT, sig_hand);
    printf("^C terminates server\a\n");
    sleep(1);
    printf("\a");
    while (1)
    {
        printf("S: I look forward to the message...\n");
        mes.mtype = SERVER;
        msgrcv(queueID, (struct msgbuf *)&mes, MAX, mes.mtype, 0);
        printf("S: Received from: %s\n", mes.mtext);

        msize = strlen(mes.mtext);

        for (i = 0; i < msize; i++)
        {
            mes.mtext[i] = toupper(mes.mtext[i]);
        }
        pid = takePID(mes.mtext);

        mes.mtype = pid;
        printf("S: Sending... %s -> %ld\n", mes.mtext, mes.mtype);
        msgsnd(queueID, (struct msgbuf *)&mes, strlen(mes.mtext) + 1, 0);
    }
}
int takePID(char text[MAX])
{
    int i, pid, len, oldi;
    len = strlen(text);
    for (i = 0; i < 12; i++)
    {
        temp[i] = text[i];
        if (temp[i] == '~')
        {
            temp[i + 1] = '\n';
            break;
        }
    }
    oldi = i;
    for (i = 0; i < len - oldi; i++)
    {
        text[i] = text[i + 1 + oldi];
    }
    pid = atoi(temp);
    return pid;
}
void sig_hand(int sig_n)
{
    key_t key;
    int queueID;
    if ((sig_n == SIGTERM) || (sig_n == SIGINT))
    {
        printf("SIGTERM\n");
        key = ftok(".", 33);

        queueID = msgget(key, IPC_CREAT | 0660);

        msgctl(queueID, IPC_RMID, 0);
        exit(0);
    }
}

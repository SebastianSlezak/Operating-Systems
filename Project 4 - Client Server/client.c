#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

#define MAX 80
#define SERVER 1

struct message
{
    long mtype;
    char mtext[MAX];
};
int main(int argc, char *argv[])
{
    int customerNumber, tem, i;
    char tmptxt[10], message[80];
    key_t key;            
    int queueId;        
    struct message mes; 
    customerNumber = getpid();

    key = ftok(".", 33);

    if ((queueId = msgget(key, IPC_CREAT | 0660)) == -1)
    {
        perror("msgget() calling...");
        exit(1);
    }
    while (1)
    {

        mes.mtype = SERVER; 
        sprintf(tmptxt, "%d~", getpid());
        strcpy(mes.mtext, tmptxt);
        printf("C[%d]: Enter text to send:\n", getpid());
        i = 0;
        while (1)
        {
            message[i] = getchar();
            if ((message[i] == '\n') || (i >= 80))
            {
                message[i] = '\0';
                break;
            }
            i++;
        }
        strcat(mes.mtext, message); 
        printf("C[%d]: Sending... \"%s\" -> SERVER\n", customerNumber, &mes.mtext[strlen(mes.mtext) - strlen(message)]);
        msgsnd(queueId, (struct msgbuf *)&mes, strlen(mes.mtext) + 1, 0); 
        mes.mtype = getpid();                                               
        msgrcv(queueId, (struct msgbuf *)&mes, MAX, mes.mtype, 0);
        printf("C[%d]: Retrieved: \"%s\" addressed to %ld\n", customerNumber, mes.mtext, mes.mtype);
    }
}

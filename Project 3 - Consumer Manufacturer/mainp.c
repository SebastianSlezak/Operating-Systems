#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <signal.h>

#define P 12 
#define MAX 10
#define MAX2 12
#define EMPTY 1
#define FULL 2

struct buffer
{
    long mtype;
    int mvalue;
};
int shmID, semID, msgID;

void exit(int sig)
{
    msgctl(msgID, IPC_RMID, NULL);
    shmctl(shmID, IPC_RMID, NULL);
    semctl(semID, 0, IPC_RMID, NULL);
    printf("MAIN - exit function signal %d: End\n", sig);
    exit(1);
}
int main()
{
    key_t key, keym, keys; 
    int i;
    struct buffer message;
    struct sigaction act;
    act.sa_handler = exit;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);
    if ((key = ftok(".", 'A')) == -1)
    {
        printf("ftok error (main)\n");
        exit(1);
    }

    msgID = msgget(key, IPC_CREAT | IPC_EXCL | 0666);
    if (msgID == -1)
    {
        printf("message queue error\n");
        exit(1);
    }

    keym = ftok(".", 'B');
    shmID = shmget(keym, MAX2 * sizeof(int), IPC_CREAT | IPC_EXCL | 0666);
    if (shmID == -1)
    {
        printf("shared memory error\n");
        exit(1);
    }

    keys = ftok(".", 'C');
    semID = semget(keys, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (semID == -1)
    {
        printf("semaphore error \n");
        exit(1);
    }
    semctl(semID, 0, SETVAL, 1);

    message.mtype = EMPTY;
    for (i = 0; i < MAX; i++)
    {
        if (msgsnd(msgID, &message, sizeof(message.mvalue), 0) == -1)
        {
            printf("error sending empty comment\n");
            exit(1);
        };
        printf("blank message sent %d\n", i);
    }
    for (i = 0; i < P; i++)
        switch (fork())
        {
        case -1:
            perror("fork error(mainprog)");
            exit(2);
        case 0:
            execl("./prod", "prod", NULL);
        }

    for (i = 0; i < P; i++)
        switch (fork())
        {
        case -1:
            printf("fork error (mainprog)\n");
            exit(2);
        case 0:
            execl("./kons", "kons", NULL);
        }

    for (i = 0; i < 2 * P; i++)
        wait(NULL);

    msgctl(msgID, IPC_RMID, NULL);
    shmctl(shmID, IPC_RMID, NULL);
    semctl(semID, 0, IPC_RMID, NULL);
    printf("MAIN: Exit\n");
}

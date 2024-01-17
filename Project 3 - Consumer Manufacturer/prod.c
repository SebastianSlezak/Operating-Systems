#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include <sys/sem.h>

struct buffer
{
    int mtype;
    int mvalue;
};

int *mem;

#define MAX 10
#define MAX2 12
#define FULL 2
#define EMPTY 1
#define read mem[MAX]
#define record mem[MAX + 1]

int main()
{
    key_t key, keym, keys;
    int msgID, semID, shmID;
    struct buffer message;

    if ((key = ftok(".", 'A')) == -1)
    {
        printf("Error ftok (A)\n");
        exit(2);
    };

    msgID = msgget(key, IPC_CREAT | 0666);
    if (msgID == -1)
    {
        printf("message queue error\n");
        exit(1);
    }

    keym = ftok(".", 'B');
    keys = ftok(".", 'X');

    shmID = shmget(keym, MAX2 * sizeof(int), IPC_CREAT | 0666);

    mem = (int *)shmat(shmID, NULL, 0);

    if (msgrcv(msgID, &message, sizeof(message.mvalue), EMPTY, 0) == -1)
    {
    }

    semID = semget(keys, 1, IPC_CREAT | 0666);

    int value = semctl(semID, 0, GETVAL, 0);

    struct sembuf operations;
    operations.sem_num = 0;
    operations.sem_op = -1;
    operations.sem_flg = 0;

    if (semop(semID, &operations, 1) == -1)
        printf("producer wait blad\n");

    mem[record] = getpid();

    printf("PID=%d:saved under buffer %d : %d\n", getpid(), record, mem[record]);

    record = (record + 1) % MAX;

    operations.sem_op = 1;

    if (semop(semID, &operations, 1) == -1)
    {
        printf("producer signal error\n");
    };

    value = semctl(semID, 0, GETVAL, 0);

    message.mtype = FULL;

    if (msgsnd(msgID, &message, sizeof(message.mvalue), 0) == -1)
    {
    }
}

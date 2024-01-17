#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>

struct buffer
{
    long mtype;
    int mvalue;
};

int *mem;

#define MAX2 12
#define MAX 10
#define FULL 2
#define EMPTY 1
#define record mem[MAX + 1]
#define read mem[MAX]

int main()
{
    key_t key, keym, keys;
    int msgID, shmID, semID;
    int i;
    int value;
    struct buffer message;
    sleep(1);
    printf("consumer--------------------------------\n");

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
    };

    keym = ftok(".", 'B');
    keys = ftok(".", 'X');

    shmID = shmget(keym, MAX2 * sizeof(int), IPC_CREAT | 0666);

    mem = (int *)shmat(shmID, NULL, 0);

    if (*mem == -1)
        printf("memory problem\n");

    if (msgrcv(msgID, &message, sizeof(message.mvalue), FULL, 0) == -1)
    {
        printf("error receiving full message\n");
        exit(1);
    }
    printf("received a FULL message\n");
    sleep(1);

    if ((semID = semget(keys, 1, IPC_CREAT | 0666)) == -1)
    {
        perror("consumer - semaphore error: ");
        exit(1);
    };

    struct sembuf operations;
    operations.sem_num = 0;
    operations.sem_op = -1;
    operations.sem_flg = SEM_UNDO;

    if (semop(semID, &operations, 1) == -1)
    {
        printf("consumer - semaphore wait error\n");
        perror("error wait");
    }

    fprintf(stderr, "consumer %d - read from buffer %d: %d\n", getpid(), read, mem[read]);

    read = (read + 1) % MAX;

    operations.sem_op = 1;

    if (semop(semID, &operations, 1) == -1)
    {
        printf("consumer - semaphore signal error\n");
        perror("error signal");
    }

    message.mtype = EMPTY;
    if (msgsnd(msgID, &message, sizeof(message.mvalue), 0) == -1)
    {
        printf("error sending empty message\n");
        exit(1);
    }
}

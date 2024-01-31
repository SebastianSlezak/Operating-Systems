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

int *memory;

#define MAX 10
#define MAX2 12
#define FULL 2
#define EMPTY 1
#define record memory[MAX]
#define read memory[MAX + 1]
int main()
{
        key_t key, keym, keys;
        int msgID;
        int shmID, semID;
        int i;
        struct buffer message;

        if ((key = ftok(".", 'A')) == -1)
        {
                printf("[PROD %d] error ftok key\n", getpid());
                exit(2);
        }

        msgID = msgget(key, IPC_CREAT | 0666);
        if (msgID == -1)
        {
                printf("[PROD %d] error message queues\n", getpid());
                exit(1);
        }

        keym = ftok(".", 'B');
        keys = ftok(".", 'C');

        shmID = shmget(keym, MAX2 * sizeof(int), IPC_CREAT | 0666);
        if (shmID == -1)
        {
                printf("[PROD %d] error memories\n", getpid());
        }

        memory = (int *)shmat(shmID, NULL, 0);

        if (msgrcv(msgID, &message, sizeof(message.mvalue), EMPTY, 0) == -1)
        {
                printf("[PROD %d] error receipt of the message EMPTY\n", getpid());
        }
        else
        {
                printf("[PROD %d] received message EMPTY\n", getpid());
        }

        semID = semget(keys, 2, IPC_CREAT | 0666);
        if (semID == -1)
        {
                printf("[PROD %d] error semaphore\n", getpid());
        }

        sleep((getpid() % 10) * 2);

        struct sembuf operations;
        operations.sem_num = 0;
        operations.sem_op = -1;
        operations.sem_flg = 0;

        if (semop(semID, &operations, 1) == -1)
        {
                printf("[PROD %d] error semop -1\n", getpid());
        }

        memory[record] = getpid();

        printf("[PROD %d] -> saved under buffer %d : %d\n", getpid(), record, memory[record]);
        record = (record + 1) % MAX;

        operations.sem_op = 1;

        if (semop(semID, &operations, 1) == -1)
        {
                printf("[PROD %d] error semop 1", getpid());
        }

        message.mtype = FULL;
        if (msgsnd(msgID, &message, sizeof(message.mvalue), 0) == -1)
        {
                printf("[PROD %d] error sending a message FULL\n");
        }
}

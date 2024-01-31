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

int *memory;

#define MAX2 12
#define MAX 10
#define FULL 2
#define EMPTY 1
#define record memory[MAX + 1]
#define read memory[MAX]

int main()
{
        key_t key, keym, keys;
        int msgID, shmID, semID;
        int i;
        struct buffer message;
        sleep(1);

        if ((key = ftok(".", 'A')) == -1)
        {
                printf("[CONS %d] error ftok (A)\n", getpid());
                exit(2);
        };

        msgID = msgget(key, IPC_CREAT | 0666);
        if (msgID == -1)
        {
                printf("[CONS %d] error message queues\n", getpid());
                exit(1);
        };

        keym = ftok(".", 'B');
        keys = ftok(".", 'C');

        shmID = shmget(keym, MAX2 * sizeof(int), IPC_CREAT | 0666);
        if (shmID == -1)
        {
                printf("[CONS %d] error memories\n");
        }

        memory = (int *)shmat(shmID, NULL, 0);
        if (*memory == -1)
        {
                printf("[CONS %d] error memories\n", getpid());
        }

        if (msgrcv(msgID, &message, sizeof(message.mvalue), FULL, 0) == -1)
        {
                printf("[CONS %d] error message reception FULL\n", getpid());
                exit(1);
        }
        else
        {
                printf("[CONS %d] picked up message FULL\n", getpid());
        }

        sleep(1);

        if ((semID = semget(keys, 2, IPC_CREAT | 0666)) == -1)
        {
                printf("[KONS] error semaphore\n", getpid());
        }

        sleep(getpid() % 10);
        struct sembuf operations;
        operations.sem_num = 1;
        operations.sem_op = -1;
        operations.sem_flg = SEM_UNDO;

        if (semop(semID, &operations, 1) == -1)
        {
                printf("[CONS %d] error semop -1\n", getpid());
        }
        printf("[CONS %d] <- read from the buffer %d: %d\n", getpid(), read, memory[read]);

        read = (read + 1) % MAX;

        operations.sem_op = 1;

        if (semop(semID, &operations, 1) == -1)
                printf("[CONS %d] error semop 1\n", getpid());

        message.mtype = EMPTY;
        if (msgsnd(msgID, &message, sizeof(message.mvalue), 0) == -1)
        {
                printf("[CONS] error messaging EMPTY\n");
                exit(1);
        }
}

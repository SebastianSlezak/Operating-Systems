#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <errno.h>

static void leaveSemaphore(int, short);
static void raiseSemaphore(int, short);

int main()
{
    key_t key;
    if (!(key = ftok(".", 'F')))
    {
        printf("Key creation error [PRODUCER]");
        exit(-2);
    }

    int semaphoreId;
    if ((semaphoreId = semget(key, 4, 0600 | IPC_CREAT)) == -1)
    {
        printf("Error accessing semaphore set [PRODUCER]\n");
        exit(-3);
    }
    else
        printf("PID=%d has been written to the buffer [PRODUCER]\n", getpid());

    int tab[4] = {1, 0, 1, 0};

    for (int i = 0; i < 4; i++)
    {
        if (semctl(semaphoreId, i, SETVAL, tab[i]) == -1)
        {
            printf("Initialization error sem\n");
            exit(-1);
        }
    }

    FILE *input, *buffer;
    if ((input = fopen("input", "r")) == NULL)
    {
        exit(1);
    }

    char token;
    while (token != EOF)
    {
        leaveSemaphore(semaphoreId, 0);
        if ((buffer = fopen("buffer", "w")) == NULL)
        {
            printf("Can't open buffer file to overwrite [PRODUCER]\n");
            exit(1);
        }
        token = fgetc(input);
        if (ferror(input) == EOF)
        {
            printf("Error reading character [PRODUCER]\n");
            exit(-9);
        }
        printf("Read token: %c [PRODUCER]\n", token);
        fputc(token, buffer);
        if (ferror(buffer) == EOF)
        {
            printf("Error writing character to buffer [PRODUCER]\n");
            exit(-9);
        }
        if (fclose(buffer) == EOF)
        {
            printf("Error closing buffer [PRODUCER]\n");
            exit(-1);
        }

        if (token == EOF)
            leaveSemaphore(semaphoreId, 2);
        raiseSemaphore(semaphoreId, 1);
    }

    printf("Completion of copy to buffer [PRODUCER]\n");
    leaveSemaphore(semaphoreId, 3);

    if (fclose(input) == EOF)
    {
        printf("Input closing error [PRODUCER]\n");
        exit(-1);
    }

    if (semctl(semaphoreId, 4, IPC_RMID) == -1)
    {
        printf("Cannot delete semaphore set [PRODUCER]\n");
        exit(-6);
    }
    else
        printf("The semaphore collection has been deleted, id: %d [PRODUCER]\n", semaphoreId);

    return 0;
}

static void leaveSemaphore(int semaphoreId, short semaphoreNumber)
{
    int changeSemaphore;
    struct sembuf bufor_sem;
    bufor_sem.sem_num = semaphoreNumber;
    bufor_sem.sem_op = -1;
    bufor_sem.sem_flg = 0;

    while (1)
    {
        changeSemaphore = semop(semaphoreId, &bufor_sem, 1);
        if (changeSemaphore == 0 || errno != 4)
            break;
    }

    if (changeSemaphore == -1)
    {
        if (errno != 4)
        {
            perror("Semaphore lowering error [PRODUCER]\n");
            exit(-5);
        }
    }
    else
        printf("Semaphore %d has been lowered [PRODUCER]\n", semaphoreNumber);
}

static void raiseSemaphore(int semaphoreId, short semaphoreNumber)
{
    int changeSemaphore;
    struct sembuf bufor_sem;
    bufor_sem.sem_num = semaphoreNumber;
    bufor_sem.sem_op = 1;
    bufor_sem.sem_flg = 0;

    while (1)
    {
        changeSemaphore = semop(semaphoreId, &bufor_sem, 1);
        if (changeSemaphore == 0 || errno != 4)
            break;
    }

    if (changeSemaphore == -1)
    {
        if (errno != 4)
        {
            perror("Semaphore lift error [PRODUCER]\n");
            exit(-5);
        }
    }
    else
        printf("Semaphore %d raised [PRODUCER]\n", semaphoreNumber);
}

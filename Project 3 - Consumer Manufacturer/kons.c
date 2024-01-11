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
        printf("Key creation error [CONSUMER]");
        exit(-2);
    }

    int semaphoreId;
    if ((semaphoreId = semget(key, 4, 0600 | IPC_CREAT)) == -1)
    {
        printf("Error accessing semaphore set [CONSUMER]\n");
        exit(-3);
    }
    else
        printf("Process %d has gained access to the semaphore set [CONSUMER]\n", getpid());

    FILE *output, *buffer;
    if ((output = fopen("output", "w")) == NULL)
    {
        printf("Can't open file output to save! [CONSUMER]\n");
        exit(1);
    }

    if (semctl(semaphoreId, 2, SETVAL, 1) == -1)
    {
        printf("Sem initiation error [CONSUMER]\n");
        exit(-1);
    }

    char token;
    int flag;

    if ((flag = semctl(semaphoreId, 2, GETVAL, NULL)) < 0)
    {
        printf("Error retrieving semaphore value [CONSUMER]\n");
        exit(-7);
    }

    while (flag > 0)
    {
        leaveSemaphore(semaphoreId, 1);
        if ((buffer = fopen("buffer", "r")) == NULL)
        {
            printf("Cannot open read buffer file [CONSUMER]\n");
            exit(1);
        }
        if ((token = fgetc(buffer)) != EOF)
        {
            if (ferror(buffer) == EOF)
            {
                printf("Error reading character [CONSUMER]\n");
                exit(-9);
            }
            printf("Character read: %c [CONSUMER]\n", token);
            fputc(token, output);
        }

        if (ferror(output) == EOF)
        {
            printf("Error writing character to buffer [CONSUMER]\n");
            exit(-9);
        }
        if (fclose(buffer) == EOF)
        {
            printf("Error closing buffer [CONSUMER]\n");
            exit(-1);
        }

        raiseSemaphore(semaphoreId, 0);
        if ((flag = semctl(semaphoreId, 2, GETVAL, NULL)) < 0)
        {
            printf("Error retrieving semaphore value [CONSUMER]\n");
            exit(-7);
        }
    }

    if (fclose(output) == EOF)
    {
        printf("Exit Closure Error [CONSUMER]\n");
        exit(-1);
    }

    raiseSemaphore(semaphoreId, 3);

    return 0;
}

static void leaveSemaphore(int semaphoreId, short semaphoreNumber)
{
    int changeSemaphore;
    struct sembuf buffer_sem;
    buffer_sem.sem_num = semaphoreNumber;
    buffer_sem.sem_op = -1;
    buffer_sem.sem_flg = 0;

    while (1)
    {
        changeSemaphore = semop(semaphoreId, &buffer_sem, 1);
        if (changeSemaphore == 0 || errno != 4)
            break;
    }

    if (changeSemaphore == -1)
    {
        if (errno != 4)
        {
            perror("Error lowering semaphore [CONSUMER]\n");
            exit(-5);
        }
    }
    else
        printf("Semaphore %d has been lowered [CONSUMER]\n", semaphoreNumber);
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
            perror("Semaphore lift error [CONSUMER]\n");
            exit(-5);
        }
    }
    else
        printf("Semaphore %d raised [CONSUMER]\n", semaphoreNumber);
}

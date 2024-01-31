#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

key_t key;
int semaphore;

void end(const char *errMsg)
{
    perror(errMsg);
    exit(EXIT_FAILURE);
}

void createNewSemaphore(key_t key)
{
    if ((semaphore = semget(key, 5, 0666 | IPC_CREAT)) == -1)
    {
        end("[P1] Error when creating a new semaphore\n");
    }
    else
    {
        printf("[P1] The semaphore has been created: %d\n", semaphore);
    }
}

void semaphoreP(int x)
{
    struct sembuf bufor_sem;
    bufor_sem.sem_num = x;
    bufor_sem.sem_op = -1;
    bufor_sem.sem_flg = SEM_UNDO;

    if ((semop(semaphore, &bufor_sem, 1)) == -1)
    {
        end("[P1] Error when closing semaphore\n");
    }
    else
    {
        printf("[P1] Semaphore %d has been closed\n", x);
    }
}

void semaphoreV(int x)
{
    struct sembuf bufor_sem;
    bufor_sem.sem_num = x;
    bufor_sem.sem_op = 1;
    bufor_sem.sem_flg = SEM_UNDO;

    if ((semop(semaphore, &bufor_sem, 1)) == -1)
    {
        end("[P1] Error when opening semaphore\n");
    }
    printf("[P1] Semaphore %d has been opened\n", x);
}

int main()
{
    if ((key = ftok(".", 'T')) == -1)
    {
        end("[P1] Key creation error\n");
    }

    createNewSemaphore(key);

    FILE *file = fopen("wynik.txt", "a");
    // p(s2) t11 t12 v(s3)
    semaphoreP(2);

    printf("Section t11 with PID = %d\n", getpid());
    fprintf(file, "Section t11 with PID = %d\n", getpid());
    sleep(1);

    printf("Section t12 with PID = %d\n", getpid());
    fprintf(file, "Section t12 with PID = %d\n", getpid());
    sleep(1);

    semaphoreV(3);

    fclose(file);
    exit(0);
}
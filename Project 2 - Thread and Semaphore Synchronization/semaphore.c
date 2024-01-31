#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/errno.h>
#include <sys/wait.h>

key_t key;
int semaphore;
int x;
pid_t w;

void end(const char *errMsg)
{
    perror(errMsg);
    exit(EXIT_FAILURE);
}

void createNewSemaphore(key_t key)
{
    if ((semaphore = semget(key, 5, 0666 | IPC_CREAT)) == -1)
    {
        end("[MAIN] Error when creating a new semaphore\n");
    }
    else
    {
        printf("[MAIN] The semaphore has been created: %d\n", semaphore);
    }
}

void setSemaphore()
{
    int args[5] = {0, 0, 0, 0, 0};

    if ((semctl(semaphore, 0, SETALL, args)) == -1)
    {
        end("[MAIN] Semaphore cannot be set\n");
    }
    else
    {
        printf("[MAIN] The semaphore has been set\n");
    }
}

void createChilds()
{
    char path[20], program[20];

    for (int i = 2; i <= 4; ++i)
    {
        sprintf(path, "./program2_%d", i);
        sprintf(program, ".program2_%d", i);

        switch (fork())
        {
        case -1:
            end("[MAIN] Error when creating a descendant");
            break;
        case 0:
            if (execl(path, program, NULL) == -1)
            {
                perror("[MAIN] Error at execl");
            }
            exit(2);
            break;
        default:
            sleep(2);
            break;
        }
    }
}

void waitSemaphore()
{
    for (int i = 0; i < 3; ++i)
    {
        w = wait(&x);

        if (w == -1)
        {
            end("[MAIN] Wait error");
        }
        else
        {
            printf("The process %d ended with: %d \n", w, WIFEXITED(x));
        }
    }
}

void deleteSemaphore()
{
    if ((semctl(semaphore, 5, IPC_RMID)) == -1)
    {
        end("[MAIN] Unable to remove semaphore");
    }
    else
    {
        printf("[MAIN] The semaphore has been removed\n");
    }
}

int main()
{
    FILE *file = fopen("wynik.txt", "w");
    fclose(file);

    if ((key = ftok(".", 'T')) == -1)
    {
        end("[MAIN] Key creation error\n");
    }

    createNewSemaphore(key);
    setSemaphore();
    createChilds();
    waitSemaphore();
    deleteSemaphore();
    exit(0);
}

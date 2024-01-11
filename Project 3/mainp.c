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

#define P 12    // liczba  procesow producent▒w i konsument▒w
#define MAX 10  // rozmiar puli bufor▒w
#define MAX2 12 // rozmiar pami▒ci dzielonej
#define PUSTY 1 // typ komunikatu
#define PELNY 2 // typ komunikatu
// struktura komunikatu
struct bufor
{
    long mtype;
    int mvalue;
};
int shmID, semID, msgID; // ID semafora, kolejki kom. pamieci dzielonej

// funkcja koniec -- do obslugi przerwania
void koniec(int sig)
{
    msgctl(msgID, IPC_RMID, NULL);
    shmctl(shmID, IPC_RMID, NULL);
    semctl(semID, 0, IPC_RMID, NULL);
    printf("MAIN - funkcja koniec sygnal %d: Koniec.\n", sig);
    exit(1);
}
int main()
{
    key_t klucz, kluczm, kluczs; // klucze do IPC
    int i;
    struct bufor komunikat;
    struct sigaction act;
    act.sa_handler = koniec;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);
    if ((klucz = ftok(".", 'A')) == -1)
    {
        printf("Blad ftok (main)\n");
        exit(1);
    }

    msgID = msgget(klucz, IPC_CREAT | IPC_EXCL | 0666);
    if (msgID == -1)
    {
        printf("blad kolejki komunikatow\n");
        exit(1);
    }

    kluczm = ftok(".", 'B');
    shmID = shmget(kluczm, MAX2 * sizeof(int), IPC_CREAT | IPC_EXCL | 0666);
    if (shmID == -1)
    {
        printf("blad pamięci dzielonej\n");
        exit(1);
    }

    kluczs = ftok(".", 'C');
    semID = semget(kluczs, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (semID == -1)
    {
        printf("blad semaforów \n");
        exit(1);
    }
    semctl(semID, 0, SETVAL, 1);

    komunikat.mtype = PUSTY;
    //   komunikat.mvalue=0;
    for (i = 0; i < MAX; i++)
    {
        if (msgsnd(msgID, &komunikat, sizeof(komunikat.mvalue), 0) == -1)
        {
            printf("blad wyslania kom. pustego\n");
            exit(1);
        };
        printf("wyslany pusty komunikat %d\n", i);
    }
    for (i = 0; i < P; i++)
        switch (fork())
        {
        case -1:
            perror("Blad fork (mainprog)");
            exit(2);
        case 0:
            execl("./prod", "prod", NULL);
        }

    for (i = 0; i < P; i++)
        switch (fork())
        {
        case -1:
            printf("Blad fork (mainprog)\n");
            exit(2);
        case 0:
            execl("./kons", "kons", NULL);
        }

    for (i = 0; i < 2 * P; i++)
        wait(NULL);

    msgctl(msgID, IPC_RMID, NULL);
    shmctl(shmID, IPC_RMID, NULL);
    semctl(semID, 0, IPC_RMID, NULL);
    printf("MAIN: Koniec.\n");
}

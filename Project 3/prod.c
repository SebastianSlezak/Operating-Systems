#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include <sys/sem.h>

struct bufor
{
    int mtype;
    int mvalue;
};

int *pam;

#define MAX 10
#define MAX2 12
#define PELNY 2
#define PUSTY 1
#define odczyt pam[MAX]
#define zapis pam[MAX + 1]
int main()
{
    key_t klucz, kluczm, kluczs;
    int msgID;
    int shmID, semID;
    time_t czas;
    struct bufor komunikat;

    if ((klucz = ftok(".", 'A')) == -1)
    {
        printf("Blad ftok (A)\n");
        exit(2);
    };

    msgID = msgget(klucz, IPC_CREAT | 0666);
    if (msgID == -1)
    {
        printf("blad klejki komunikatow\n");
        exit(1);
    }
    kluczm = ftok(".", 'B');
    kluczs = ftok(".", 'C');
    shmID = shmget(kluczm, MAX2 * sizeof(int), IPC_CREAT | 0666);
    pam = (int *)shmat(shmID, NULL, 0);

    // odbiokomunikatu
    if (msgrcv(msgID, &komunikat, sizeof(komunikat.mvalue), PUSTY, 0) == -1)
    {
    }

    semID = semget(kluczs, 1, IPC_CREAT | 0666);

    sleep(getpid() % 10);
    struct sembuf operacje;
    // w sekcji krytycznej  zapis PID-u (getpid()) do puli buforow pod indeks zapis
    // wypisanie informacji na ekranie
    // modyfikacja indeksu do zapisu
    // wyslanie odpowiedniego komunikatu
}

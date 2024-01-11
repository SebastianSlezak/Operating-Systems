#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
struct bufor
{
    int mtype;
    int mvalue;
};
int *pam;
#define MAX2 12
#define MAX 10
#define PELNY 2
#define PUSTY 1
#define zapis pam[MAX + 1]
#define odczyt pam[MAX]

int main()
{
    key_t klucz, kluczm, kluczs;
    int msgID, shmID, semID;
    int i;
    struct bufor komunikat;
    sleep(1);
    printf("konsument--------------------------------\n");
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
    };

    kluczm = ftok(".", 'B');
    kluczs = ftok(".", 'C');
    if (semID = semget(kluczs, 1, IPC_CREAT | 0666) == -1)
    {
        perror("blad semafora ");
        exit(1);
    };
    shmID = shmget(kluczm, MAX2 * sizeof(int), IPC_CREAT | 0666);
    pam = (int *)shmat(shmID, NULL, 0);
    // odbiorpowiedniego komunikatu
    sleep(getpid() % 10);
    struct sembuf operacje;
    // w sekcji krytycznej odczyt z pamiedzielonej spod indeksu oczyt
    // uaktualnienie wskazdo odczytu (mod MAX
    // wyslanie odpowiedniego komunkat
}

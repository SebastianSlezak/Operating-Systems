#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ROWS 2
#define COLUMNS 10

int tab[ROWS][COLUMNS];

typedef struct {
    int row;
    int result;
} ThreadData;

void *calculateRowSum (void *data){
    ThreadData *threadData = (ThreadData *)data;
    int row = threadData->row;
    int sum = 0;

    for (int i = 0; i < COLUMNS; ++i) {
        sum += tab[row][i];
    }

    threadData->result = sum;
    pthread_exit(NULL);
};

int main() {
    time_t t;
    srand((unsigned) time(&t));

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLUMNS; ++j) {
            tab[i][j] = rand() % 10;
        }
    }

    pthread_t pthread1, pthread2;
    ThreadData data1 = {0, 0};
    ThreadData data2 = {1, 0};

    pthread_create(&pthread1, NULL, calculateRowSum, (void *)&data1);
    pthread_create(&pthread2, NULL, calculateRowSum, (void *)&data2);

    pthread_join(pthread1, NULL);
    pthread_join(pthread2, NULL);

    int totalSum = data1.result + data2.result;

    printf("Sum of the elements of the first row: %d\n", data1.result);
    printf("Sum of the elements of the second row: %d\n", data2.result);
    printf("Total sum: %d\n", totalSum);

    return 0;
}
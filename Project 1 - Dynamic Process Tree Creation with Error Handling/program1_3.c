#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/errno.h>

int main() {

    char cmd[20];
    int w, x;

    sprintf(cmd, "pstree -p %d", getpid());
    system(cmd);

    for (int i = 0; i < 3; i++) {
        switch (fork()) {
            case -1:
                perror("fork error");
                exit(1);
                break;
            case 0:
                if (execl("./program1_1", "program1_1", NULL) == -1) {
                    perror("execl error");
                }
                exit(2);
                break;
            default:
                printf("Action for the process parent.\n");
                break;
        }
    }

    system(cmd);

    for (int i = 0; i < 3; ++i) {
        w = wait(&x);

        if (w == -1) {
            perror("wait error");
            exit(3);
        }
        printf("The process %d ended with: %d \n", w, x);
    }

    return 0;
}

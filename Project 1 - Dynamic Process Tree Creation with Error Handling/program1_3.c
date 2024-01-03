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
                /*Proces potomny*/
                if (execl("./program1_1", "program1_1", NULL) == -1) {
                    perror("execl error");
                }
                exit(2);
                break;
            default:
                /*Proces macierzysty*/
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

/*
During the execution of the program, 7 processes are created.
The program (program1_3) is the master process that runs the other processes.
Then the sh processes are run, which is needed to run the pstree process.
Then, using the execl() function, three times the descendant processes of the program (program1_1) are created.
At the very end, the sh process, which is another child process of the program1_3 process that runs pstree.
*/
}

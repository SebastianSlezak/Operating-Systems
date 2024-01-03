#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {

    char cmd[20];

    sprintf(cmd, "pstree -p %d", getpid());
    system(cmd);

    for (int i = 0; i < 3; i++) {
        switch (fork()) {
            case -1:
                perror("fork error");
                exit(1);
                break;
            case 0:
                printf("PID: %d\n", getpid());
                printf("PPID: %d\n", getppid());
                printf("UID: %d\n", getuid());
                printf("GID: %d\n", getgid());
                break;
            default:
                printf("Action for the process parent.\n");
                break;
        }
    }

    system(cmd);

    return 0;
}

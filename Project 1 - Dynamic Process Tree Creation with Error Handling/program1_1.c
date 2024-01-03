#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    printf("PID: %d\n", getpid());
    printf("PPID: %d\n", getppid());
    printf("UID: %d\n", getuid());
    printf("GID: %d\n", getgid());

    return 0;
}
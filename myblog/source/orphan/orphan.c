/**
 * orphan.c
 * 孤儿进程
 **/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if (pid == 0) // child process
    {
        printf("child process %d, parent process %d\n", getpid(), getppid());
        sleep(5);
        printf("parent process died now\n");
        printf("child process %d, parent process %d\n", getpid(), getppid());
        exit(0);
    }
    else // parent process
    {
        printf("parent process %d\n", getpid());
        sleep(1);
        printf("parent process closed now.\n");
        exit(0);
    }
    return 0;
}

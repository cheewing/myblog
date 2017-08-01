/**
 * zombie.c
 * 显示僵尸进程的产生过程
 **/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv[])
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
        printf("after child process died....\n");
        exit(0);
    }
    else // parent process
    {
        sleep(1);
        printf("parent process %d\n", getpid());
        system("ps -o pid,ppid,state,tty,command");
        printf("parent process is exiting\n");
        exit(0);
    }
    exit(0);
}

/**
 * init.c
 * 僵尸进程解决方法：fork两次，让僵尸进程成为孤儿进程
 **/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char **argv[])
{
    pid_t pid;
    
    pid = fork();

    if (pid < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if (pid == 0)
    {
        printf("I am first child process %d\n", getpid());
        pid = fork();
        
        if (pid < 0)
        {
            perror("fork error");
            exit(2);
        }
        else if (pid > 0)
        {
            printf("first child %d is exited\n", getpid());
            exit(0);
        }

        sleep(3);
        printf("I am second child process %d, parent process %d\n", getpid(), getppid());
        exit(0);
    }
    else
    {
        if (waitpid(pid, NULL, 0) != pid)
        {
            perror("waitpid error");
            exit(1);
        }
    }
    exit(0);
}

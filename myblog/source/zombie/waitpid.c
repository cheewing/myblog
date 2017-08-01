/**
 * waitpid.c
 * 僵尸进程解决方法：父进程接收子进程信号
 **/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

static void sig_child(int signo);

int main(int argc, char **argv[])
{
    pid_t pid;

    signal(SIGCHLD, sig_child);   // catch child signal

    pid = fork();

    if (pid < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if (pid == 0) // child process
    {
        printf("child process %d is exiting\n", getpid());
        exit(0);
    }
    else // parent process
    {
        printf("parent process %d\n", getpid());
        sleep(2);
        system("ps -o pid,ppid,state,tty,command");
        printf("parent process is exiting\n");
        exit(0);
    }
    exit(0);
}

static void sig_child(int signo)
{
    pid_t pid;
    int stat;

    while ((pid = waitpid(pid, &stat, WNOHANG)) > 0)    
    {
        printf("child process %d terminated\n", pid);
    }
}

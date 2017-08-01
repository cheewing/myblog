## fork后父子进程共享资源

Unix环境高级编程中8.3节中说，“子进程是父进程的副本。例如，子进程获得父进程数据空间、堆和栈的副本。注意，这是子进程所拥有的副本。父进程和子进程并不共享这些存储空间部分。父进程和子进程共享正文段。”

书中还预留了例子说明子进程对变量所做的改变并不影响父进程中该变量的值。

```c
/**
 * fork.c
 * 探讨父子进程共享变量问题
 **/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int globvar = 6;        /* external variable in initialized data */
char buf[] = "a write to stdout\n";

int 
main(void)
{
    int var;    /* automatic variable on the stach */
    pid_t pid;

    var = 88;
    if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1)
    {
        perror("write error");
        exit(1);
    }
    printf("before fork\n");    /* we don't flush stdout */

    if ((pid = fork()) < 0)
    {
        perror("fork error");
        exit(1);
    } else if (pid == 0) {    /* child */
        globvar++;          /* modify variables */
        var++;
    } else {
        sleep(2);           /* parent */
    }

    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);
    exit(0);
}
```

编译和运行程序，如下：

```c
/myblog/source/fork# gcc fork.c -o fork
/myblog/source/fork# ./fork
a write to stdout
before fork
pid = 12071, glob = 7, var = 89
pid = 12070, glob = 6, var = 88
```

如上，可以看出，fork后子进程只是获得了父进程的副本，所有变量的变更都只在各自进程中有效。

实际上，fork后子进程和父进程共享的资源还包括：

- 打开的文件
- 实际用户ID、实际组ID、有效用户ID、有效组ID
- 添加组ID
- 进程组ID
- 会话期ID
- 控制终端
- 设置-用户-ID标志和设置-组-ID标志
- 当前工作目录
- 根目录
- 文件方式创建屏蔽字
- 信号屏蔽和排列
- 对任一打开文件描述符的在执行时关闭标志
- 环境
- 连接的共享存储段（共享内存）
- 资源限制

父子进程之间的区别是：

- fork的返回值
- 进程ID
- 不同的父进程ID
- 子进程的tms_utime，tms_stime，tms-cutime以及tms_ustime设置为0
- 父进程设置的锁，子进程不继承
- 子进程的未决告警被清除
- 子进程的未决信号集设置为空集



参考文献

[1] Unix环境高级编程（第3版）

[2] http://blog.csdn.net/u012138828/article/details/39031823
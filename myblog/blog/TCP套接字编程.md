### TCP套接字编程



#### socket函数

为了执行网络I/O，一个进程必须做的第一件事情就是调用socket函数，指定期望的通信协议类型。

```c
#include <sys/socket.h>

int socket(int family, int type, int protocol);

返回：若成功则为非负描述符，若出错则为-1
```

参数：

- family参数指明协议族

  | family   | 说明      |
  | -------- | ------- |
  | AF_INET  | IPv4协议  |
  | AF_INET6 | IPv6协议  |
  | AF_LOCAL | Unix域协议 |
  | AF_ROUTE | 路由套接字   |
  | AF_KEY   | 密钥套接字   |

- type参数指明套接字类型

  | type           | 说明      |
  | -------------- | ------- |
  | SOCK_STREAM    | 字节流套接字  |
  | SOCK_DGRAM     | 数据报套接字  |
  | SOCK_SEQPACKET | 有序分组套接字 |
  | SOCK_RAW       | 原始套接字   |

- protocol参数为下图所示的某个协议类型常值，或者设为0，以选择所给定family和type组合的系统默认值。

  | protocol     | 说明       |
  | ------------ | -------- |
  | IPPROTO_TCP  | TCP传输协议  |
  | IPPROTO_UDP  | UDP传输协议  |
  | IPPROTO_SCTP | SCTP传输协议 |

socket函数在成功时返回一个小的非负整数值，称为套接字描述符。



#### connect函数

TCP客户用connect函数来建立与TCP服务器的连接。

```c
#include <sys/socket.h>
int connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen);

返回：若成功则为0，若出错则为-1
```

参数：

- sockfd是由socket函数返回的套接字描述符。
- servaddr参数是指向套接字地址结构的指针。
- addrlen为sockaddr结构的大小。



#### bind函数

bind函数把一个本地协议地址赋予一个套接字。

```c
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr *myaddr, socklen_t addrlen);

返回：若成功则为0，若出错则为-1
```

- sockfd是由socket函数返回的套接字描述符。
- myaddr参数是指向特定于协议的地址结构的指针。
- addrlen是该地址结构的长度。



#### listen函数

listen函数仅由TCP服务器调用，它做两件事情：

- 当socket函数创建一个套接字时，它被假设为一个主动套接字，即它是一个将调用connect发起连接的客户套接字。listen函数把一个未连接的套接字转换成一个被动套接字，指示内核应接受指向该套接字的连接请求。
- 本函数的第二个参数规定了内核应该为相应套接字排队的最大连接个数。

```c
#include <sys/socket.h>
int listen(int sockfd, int backlog);

返回：若成功则为0，若出错则为-1
```

- sockfd是由socket函数返回的套接字描述符。
- backlog参数规定了内核应该为相应套接字排队的最大连接个数。



#### accept函数

accept函数由TCP服务器调用，用于从已完成连接队列队头返回下一个已完成连接。如果已完成连接队列唯恐，那么进程将被投入睡眠（假定套接字为默认的阻塞方式）。

```c
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);

返回：若成功则为非负描述符，若出错则为-1
```

- sockfd是由socket函数返回的套接字描述符。
- 参数cliaddr和addrlen用来返回已连接的对端进程（客户）的协议地址。
- addrlen是值-结果参数：调用前，我们将由*addrlen所引用的整数值置为由cliaddr所指向的套接字地址结构的长度，返回时，该整数值即为由内核存放在该套接字地址结构内的确切字节数。



#### TCP客户/服务器程序套接字流程





#### 服务器／客户端程序实例

tcpserv.c

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>

int main()
{
    int listenfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    char buf[1024];
    int n;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
        perror("socket error");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(9877);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    listen(listenfd, 5);

    socklen_t len = sizeof(cliaddr);
    for ( ; ; )
    {
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);

        while ((n = read(connfd, buf, 1024)) > 0)
        {
            fputs(buf, stdout);
            write(connfd, buf, n);
        }
        close(connfd);
    }

    close(listenfd);
    exit(0);
}

```



tcpcli.c

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>

int main(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in servaddr;
    char sendline[1024], recvline[1024];

    if (argc != 2)
    {
        printf("usage: tcpcli <IPaddress>\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        perror("socket error");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9877);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    while (fgets(sendline, 1024, stdin) != NULL)
    {
        printf(sendline);
        write(sockfd, sendline, strlen(sendline));

        if (read(sockfd, recvline, 1024) == 0)
        {
            printf("str_cli: server terminated prematurely\n");
            exit(1);
        }

        fputs(recvline, stdout);
    }

    exit(0);
}

```


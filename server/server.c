#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define PENDING_CONNECTION_BACKLOG 50

int err(char *s);
struct sockaddr_in create_serv_addr(int port);

int main(int argc, char const *argv[])
{
    int conn_fd, listen_fd;
    struct sockaddr_in cli_addr;
    socklen_t cli_len;

    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        err("socket");
    }

    struct sockaddr_in serv_addr = create_serv_addr(3001);
    if (bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        err("bind");
    }

    if (listen(listen_fd, PENDING_CONNECTION_BACKLOG) == -1)
    {
        err("listen");
    }
    while (1)
    {
        cli_len = sizeof(cli_addr);
        conn_fd = accept(listen_fd, (struct sockaddr *)&cli_addr, &cli_len);

        close(conn_fd);
    }

    return 0;
}

struct sockaddr_in create_serv_addr(int port)
{
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    return serv_addr;
}

int err(char *s)
{
    printf("%s\n", s);
    printf("Errno:%d\n", errno);
    fprintf(stderr, "%s\n", strerror(errno));
    exit(-1);
}

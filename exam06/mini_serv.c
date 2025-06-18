#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <sys/select.h>

int count = 0, ids[65536], max_fd;
char *msgs[65536], buf[1001];
fd_set active_fds, read_fds, write_fds;

#define ARG_ERR "Wrong number of arguments\n"
#define FATAL_ERR "Fatal error\n"

void fatal() {
    write(2, FATAL_ERR, strlen(FATAL_ERR));
    exit(1);
}

void notify_all(int sender, char *message) {
    size_t len = strlen(message);
    for (int fd = 0; fd <= max_fd; fd++)
        if (FD_ISSET(fd, &write_fds) && fd != sender)
            send(fd, message, len, 0);
}

int extract_message(char **buf, char **msg);
char *str_join(char *buf, char *add);

int main(int ac, char **av) {
    if (ac != 2) {
        write(2, ARG_ERR, strlen(ARG_ERR));
        return 1;
    }
    int port = atoi(av[1]);
    max_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (port <= 0 || max_fd < 0) fatal();

    FD_ZERO(&active_fds);
    FD_SET(max_fd, &active_fds);

    struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(2130706433);
	servaddr.sin_port = htons(port);
	if ((bind(max_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) fatal();
	if (listen(max_fd, SOMAXCONN) != 0) fatal();

    int server_fd = max_fd;
    while(1 == 1) {
        read_fds = write_fds = active_fds;
        if (select(max_fd + 1, &read_fds, &write_fds, 0, 0) < 0) fatal();

        for (int fd = 0; fd <= max_fd; fd++) {
            if (!FD_ISSET(fd, &read_fds)) continue;

            if (server_fd == fd) {
                socklen_t len = sizeof(servaddr);
                int client = accept(server_fd, (struct sockaddr *)&servaddr, &len);
                if (client > max_fd) max_fd = client;
                if (client >= 0) {
                    FD_SET(client, &active_fds);
                    ids[client] = count++;
                    msgs[client] = NULL;
                    sprintf(buf, "server: client %d just arrived\n", ids[client]);
                    notify_all(fd, buf);
                }
            } else {
                int bytes = recv(fd, buf, 1000, 0);
                if (bytes <= 0) {
                    sprintf(buf, "server: client %d just left\n", ids[fd]);
                    notify_all(fd, buf);
                    FD_CLR(fd, &active_fds);
                    close(fd);
                    free(msgs[fd]);
                } else {
                    buf[bytes] = '\0';
                    char *temp;
                    msgs[fd] = str_join(msgs[fd], buf);
                    sprintf(buf, "client %d: ", ids[fd]);
                    while (extract_message(&msgs[fd], &temp)) {
                        notify_all(fd, buf);
                        notify_all(fd, temp);
                        free(temp);
                    }
                }
            }
        }
    }

}

// Copy from main
int extract_message(char **buf, char **msg)
{
    char *newbuf;
    int i;

    *msg = 0;
    if (*buf == 0)
        return (0);
    i = 0;
    while ((*buf)[i])
    {
        if ((*buf)[i] == '\n')
        {
            newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
            if (newbuf == 0)
                return (-1);
            strcpy(newbuf, *buf + i + 1);
            *msg = *buf;
            (*msg)[i + 1] = 0;
            *buf = newbuf;
            return (1);
        }
        i++;
    }
    return (0);
}

// Copy from main
char *str_join(char *buf, char *add)
{
    char *newbuf;
    int len;

    if (buf == 0)
        len = 0;
    else
        len = strlen(buf);
    newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
    if (newbuf == 0)
        return (0);
    newbuf[0] = 0;
    if (buf != 0)
        strcat(newbuf, buf);
    free(buf);
    strcat(newbuf, add);
    return (newbuf);
}

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <sys/select.h>

int extract_message(char **buf, char **msg);
char *str_join(char *buf, char *add);

fd_set afds, wfds, rfds;
int max_fd, count = 0, ids[65536];
char *msgs[65536], buf[1001];

void fatal() {
    write(2, "Fatal error\n", 12);
    exit(1);
}

void notify(int sender, char *msg) {
    int len = strlen(msg);
    for (int fd = 0; fd <= max_fd; fd++)
        if (FD_ISSET(fd, &wfds) && sender != fd)
            send(fd, msg, len, 0);
}

int main(int ac, char **av) {
    if (ac != 2) {
        write(2, "Wrong number of arguments\n", 26);
        return 1;
    }
    FD_ZERO(&afds);
    max_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (max_fd < 0) fatal();
    FD_SET(max_fd, &afds);
    int server_fd = max_fd;

    struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433);
	servaddr.sin_port = htons(atoi(av[1]));

	if ((bind(server_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
        fatal();
	if (listen(server_fd, SOMAXCONN))
        fatal();

    while (1) {
        rfds = wfds = afds;
        if (select(max_fd + 1, &rfds, &wfds, 0, 0) < 0) fatal();

        for (int fd = 0; fd <= max_fd; fd++) {
            if (!FD_ISSET(fd, &rfds)) continue;
            // Accept
            if(fd == server_fd) {
                socklen_t len = sizeof(servaddr);
                int c = accept(server_fd, (struct sockaddr*)&servaddr, &len);
                if (c >= 0) {
                    if (c > max_fd) max_fd = c;
                    FD_SET(c, &afds);
                    ids[c] = count++;
                    msgs[c] = NULL;
                    sprintf(buf, "server: client %d just arrived\n", ids[c]);
                    notify(c ,buf);
                }
            }
            else {
                int bts = recv(fd, buf, 1000, 0);
                // Left
                if (bts <= 0) {
                    sprintf(buf, "server: client %d just left\n", ids[fd]);
                    notify(fd, buf);
                    free(msgs[fd]);
                    FD_CLR(fd, &afds);
                    close(fd);
                }
                // Handle
                else {
                    buf[bts] = '\0';
                    char *m;
                    msgs[fd] = str_join(msgs[fd], buf);
                    while (extract_message(&msgs[fd], &m)) {
                        sprintf(buf, "client %d: ", ids[fd]);
                        notify(fd, buf);
                        notify(fd, m);
                        free(m);
                    }
                }
            }
        }
    }
    return 0;
}

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

// END COPY-PASTE

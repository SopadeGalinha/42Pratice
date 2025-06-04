#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/select.h>

void initserver(int port);
char *str_join(char *buf, char *add);
int extract_message(char **buf, char **msg);


#define FATAL "Fatal error\n"
#define ARGS "Wrong number of arguments\n"
#define LOCALHOST 2130706433

typedef struct s_client {
	int fd, id;
	char *buffer;
} t_client;

t_client clients[1024];
int server_fd, next_id = 0, last_fd = 0;
fd_set active_fds, read_fds;
struct sockaddr_in servaddr;

void error(const char *msg) {
	write(2, msg, strlen(msg));
	exit(1);
}

void send_to_all(int sender, const char *msg) {
	size_t len = strlen(msg);
	for (int fd = 0; fd <= last_fd; fd++)
		if (FD_ISSET(fd, &active_fds) && (fd != sender && fd != server_fd))
			send(fd, msg, len, 0);
}

void handle(int fd) {
	t_client *c = &clients[fd];
	char tmp[1001];
	ssize_t bytes = recv(fd, tmp, sizeof(tmp) -1, 0);
	if (bytes <= 0) {
		char msg[64];
		snprintf(msg, sizeof(msg), "server: client %d just left\n", c->id);
		send_to_all(fd, msg);
		FD_CLR(fd, &active_fds);
		free(c->buffer);
		return;
	}
	tmp[bytes] = '\0';
	c->buffer = str_join(c->buffer, tmp);
	char *msg;
	while (extract_message(&c->buffer, &msg)) {
		char *fmt = malloc(strlen(msg) +32);
		if (!fmt)
			error(FATAL);
		sprintf(fmt, "client %d: %s", c->id, msg);
		send_to_all(c->fd, fmt);
		free(msg);
		free(fmt);
	}
}

void accepts() {
    int c = accept(server_fd, 0, 0);
    if (c < 0) return;
    if (c > last_fd) last_fd = c;
    FD_SET(c, &active_fds);
    clients[c] = (t_client){c, next_id++, 0};
    char msg[64];
    snprintf(msg, sizeof(msg), "server: client %d just arrived\n", clients[c].id);
    send_to_all(c, msg);
}

void run() {
    while(1) {
        read_fds = active_fds;
        if (select(last_fd + 1, &read_fds, 0, 0, 0) < 0)
            continue;
        if (FD_ISSET(server_fd, &read_fds))
            accepts();
        for (int fd = server_fd +1; fd <= last_fd; fd++)
            if(FD_ISSET(fd, &read_fds))
                handle(fd);
    }
}

int main(int ac, char **av) {
	if (ac != 2)
		error(ARGS);
	initserver(atoi(av[1]));
	run();
}

int extract_message(char **buf, char **msg) {
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

char *str_join(char *buf, char *add) {
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

void initserver(int port) {
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0 || port <= 0) error(FATAL);

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(LOCALHOST);
	servaddr.sin_port = htons(port);

	if (bind(server_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		error(FATAL);
	if (listen(server_fd, SOMAXCONN));
	FD_ZERO(&active_fds);
	FD_SET(server_fd, &active_fds);
	last_fd = server_fd;
}

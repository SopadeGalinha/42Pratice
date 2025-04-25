#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/select.h>

#define ERR_FATAL "Fatal error\n"
#define ERR_ARGS  "Wrong number of arguments\n"

typedef struct s_client {
    int fd;
    int id;
    char *buffer;
}               t_client;

static int      server_fd;
static int      next_id = 0;
static int      last_fd = 0;
static fd_set   active_fds;
static fd_set   read_fds;
static t_client clients[1024];
static struct sockaddr_in servaddr;

/* Subject functions */
char    *str_join(char *buf, char *add);
int     extract_message(char **buf, char **msg);

/* Mine functions */
void    error(const char *msg);
void    init_server(int port);
void    send_to_all(int fd_sender, const char *msg);

int     parse_port(char *arg);
void    event_loop(void);
void    accept_new_client(void);
void    handle_client_data(int fd);

int main(int ac, char **av) {
    if (ac != 2)
        error(ERR_ARGS);
    init_server(atoi(av[1]));
    event_loop();
    return 0;
}

void init_server(int port) {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0 || port <= 0)
        error(ERR_FATAL);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(2130706433);
    servaddr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 
    || listen(server_fd, SOMAXCONN) < 0)
        error(ERR_FATAL);
    FD_ZERO(&active_fds);
    FD_SET(server_fd, &active_fds);
    last_fd = server_fd;
}

void event_loop(void) {
    while (1) {
        read_fds = active_fds;
        if (select(last_fd + 1, &read_fds, 0, 0, 0) < 0)
            continue;

        if (FD_ISSET(server_fd, &read_fds))
            accept_new_client();

        for (int fd = server_fd + 1; fd <= last_fd; fd++) {
            if (FD_ISSET(fd, &read_fds))
                handle_client_data(fd);
        }
    }
}

void accept_new_client(void) {
    int client_fd = accept(server_fd, 0, 0);
    if (client_fd < 0)
        return;

    FD_SET(client_fd, &active_fds);
    clients[client_fd] = (t_client){ client_fd, next_id++, 0 };
    if (client_fd > last_fd)
        last_fd = client_fd;

    char msg[64];
    snprintf(msg, sizeof(msg), "server: client %d just arrived\n", clients[client_fd].id);
    send_to_all(client_fd, msg);
}

void handle_client_data(int fd) {
    t_client *client = &clients[fd];
    char temp[1001];
    ssize_t bytes = recv(fd, temp, sizeof(temp) - 1, 0);

    if (bytes <= 0) {
        char msg[64];
        snprintf(msg, sizeof(msg), "server: client %d just left\n", client->id);
        send_to_all(fd, msg);
        FD_CLR(fd, &active_fds);
        close(fd);
        free(client->buffer);
        return;
    }

    temp[bytes] = '\0';
    client->buffer = str_join(client->buffer, temp);

    char *msg;
    while (extract_message(&client->buffer, &msg) > 0) {
        char *formatted = malloc(strlen(msg) + 32);
        if (!formatted) {
            free(msg);
            continue;
        }
        sprintf(formatted, "client %d: %s", client->id, msg);
        send_to_all(fd, formatted);
        free(formatted);
        free(msg);
    }
}

void send_to_all(int fd_sender, const char *msg) {
    int len = strlen(msg);
	for (int fd = 0; fd < last_fd; fd++)
    if (FD_ISSET(fd, &active_fds) && fd != fd_sender && fd != server_fd)
    send(fd, msg, len, 0);
}

void error(const char *msg) {
    write(2, msg, strlen(msg));
    exit(1);
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

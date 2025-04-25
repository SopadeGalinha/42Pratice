# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <netinet/in.h>
# include <sys/select.h>

void send_to_all(int fd_sender, const char *msg);
int extract_message(char **buf, char **msg);
char *str_join(char *buf, char *add);
void error(const char *msg);

typedef struct s_client {
	int fd, id;
	char *buffer;
} t_client;

# define ERR_FATAL "Fatal error\n"
# define ERR_ARGS "Wrong number of arguments\n"

int server;
int next_id = 0;
int last_fd = 0;

fd_set read_fds, active_fds;
t_client clients[1024];
struct sockaddr_in servaddr;

int main(int ac, char **av) {
	if (ac != 2)
		error(ERR_ARGS);
	server = socket(AF_INET, SOCK_STREAM, 0);
	
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(2130706433);
	servaddr.sin_port = htons(atoi(av[1]));

	if (bind(server, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		error(ERR_FATAL);
	if (listen(server, SOMAXCONN) < 0)
		error(ERR_FATAL);
	
	FD_ZERO(&active_fds);
	FD_SET(server, &active_fds);
	last_fd = server;

	while(1) {
		read_fds = active_fds;
		if (select(last_fd + 1, &read_fds, 0, 0, 0) < 0)
			continue;
		if (FD_ISSET(server, &read_fds)) {
			int client_fd = accept(server, 0, 0);
			if (client_fd < 0)
				continue;
			
			FD_SET(client_fd, &active_fds);
			clients[client_fd] = (t_client){client_fd, next_id++, 0};
			if (client_fd > last_fd)
				last_fd = client_fd;
	
			char msg[50];
			snprintf(msg, sizeof(msg), "server: client %d just arrived\n", clients[client_fd].id);
			send_to_all(client_fd, msg);
		}

		for (int fd = 0; fd <= last_fd; fd++) {
			if (!FD_ISSET(fd, &read_fds) || fd == server)
				continue;
			
			t_client *client = &clients[fd];
			char temp[1001];
			ssize_t bytes = recv(fd, temp, 1000, 0);

			if (bytes <= 0) {
				char msg[50];
				snprintf(msg, sizeof(msg), "server: client %d just left\n", client->id);
				send_to_all(fd, msg);
				FD_CLR(fd, &active_fds);
				close(fd);
				free(client->buffer);
				continue;
			} else {
				temp[bytes] = '\0';
				client->buffer = str_join(client->buffer, temp);
				char *msg;
				while (extract_message(&client->buffer, &msg)) {
					char *formatted = malloc(strlen(msg) + 20);
					if (!formatted)
						continue;
					sprintf(formatted, "client %d: %s", client->id, msg);
					send_to_all(fd, formatted);
					free(formatted);
					free(msg);
				}
			}
		}

	}
	return 0;
}

void error(const char *msg) {
	write(2, msg, strlen(msg));
	exit(1);
}

void send_to_all(int fd_sender, const char *msg) {
	size_t len = strlen(msg);
	for (int fd = 0; fd < last_fd; fd++)
		if (FD_ISSET(fd, &active_fds) && fd != fd_sender && fd != server)
			send(fd, msg, len, 0);
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


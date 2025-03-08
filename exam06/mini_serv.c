#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/select.h>

#define MAX_CLIENTS (16 * 4096)
#define BUFFER_SIZE (96 * 4096)

#define ERR_FATAL "Fatal error\n"
#define MSG_CLIENT "client %d: %s"
#define ERR_ARGS "Usage: ./server <port>\n"
#define MSG_LEFT "server: client %d just left\n"
#define MSG_ARRIVED "server: client %d just arrived\n"

int server_fd, max_fd, client_id = 0, client_map[MAX_CLIENTS];
char message[BUFFER_SIZE], buffer[BUFFER_SIZE];
fd_set master_set, read_fds;

void end_process(const char *msg)
{
	write(2, msg, strlen(msg));
	if (server_fd > 2)
		close(server_fd);
	exit(1);
}

void broadcast(int sender_fd)
{
	for (int fd = 3; fd <= max_fd; ++fd)
		if (fd != sender_fd && FD_ISSET(fd, &read_fds) && send(fd, message, strlen(message), 0) < 0)
			end_process(ERR_FATAL);
	bzero(message, sizeof(message));
}

void accept_client()
{
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);
	int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
	if (client_fd < 0)
		end_process(ERR_FATAL);

	client_map[client_fd] = client_id++;
	sprintf(message, MSG_ARRIVED, client_map[client_fd]);
	FD_SET(client_fd, &master_set);
	if (client_fd > max_fd)
		max_fd = client_fd;
	broadcast(client_fd);
}

void handle_client(int client_fd)
{
	bzero(buffer, sizeof(buffer));
	if (recv(client_fd, buffer, sizeof(buffer) - 1, 0) <= 0)
	{
		sprintf(message, MSG_LEFT, client_map[client_fd]);
		FD_CLR(client_fd, &master_set);
		close(client_fd);
	}
	else
		sprintf(message, MSG_CLIENT, client_map[client_fd], buffer);
	broadcast(client_fd);
}

void run_server()
{
	while (1)
	{
		read_fds = master_set;
		if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0)
			continue;
		if (FD_ISSET(server_fd, &read_fds))
			accept_client();
		for (int fd = 3; fd <= max_fd; ++fd)
			if (FD_ISSET(fd, &read_fds))
				handle_client(fd);
	}
}

int main(int ac, char **av)
{
	if (ac != 2)
		end_process(ERR_ARGS);

	int port = atoi(av[1]);
	struct sockaddr_in server_addr = {AF_INET, htons(port), htonl(2130706433)};
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		end_process(ERR_FATAL);
	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		end_process(ERR_FATAL);
	if (listen(server_fd, 10) < 0)
		end_process(ERR_FATAL);

	FD_ZERO(&master_set);
	FD_SET(server_fd, &master_set);
	max_fd = server_fd;
	run_server();
}

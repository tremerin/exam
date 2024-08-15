#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct client {
    int id;
    char *msgs;
} Client;

void print_error(char *str)
{
    write(2, str, strlen(str));
}

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

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
	char	*newbuf;
	int		len;

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


int main(int argc, char **argv) {
	int sockfd, maxfd;
	struct sockaddr_in servaddr;
    Client clients[200];
    char buffer[2000];
    char write_buffer[2000];
    fd_set readfds, writefds, activefds;
    int next_id = 0;

    if (argc != 2)
    {
        print_error("Wrong number of arguments\n");
        exit(1);
    }

	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		print_error("Fatal error\n");
		exit(1); 
	} 
    maxfd = sockfd;
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(argv[1])); 

	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
		print_error("Fatal error\n");
		exit(1); 
	} 

	if (listen(sockfd, 100) != 0) {
		print_error("Fatal error\n");
		exit(1);
	}
    
    FD_ZERO(&activefds);
    FD_SET(sockfd, &activefds);
	socklen_t len;
    int client_fd = 0;

    while (1)
    {
        readfds = writefds = activefds;
        if (select(maxfd + 1, &readfds, &writefds, NULL, NULL) < 0)
        {
            print_error("Fatal error\n");
		    exit(1);
        }
        for (int i = 0; i <= maxfd; i++)
        { 
            if (!FD_ISSET(i, &readfds))
                continue;
            if (i == sockfd)
            {
                len = sizeof(servaddr);
                client_fd = accept(sockfd, (struct sockaddr *)&servaddr, &len);
                if (client_fd < 0)
                { 
                    print_error("Fatal error\n");
                    exit(1);
                }
                clients[client_fd].id = next_id++;
                clients[client_fd].msgs = NULL;
                if (client_fd > maxfd)
                    maxfd = client_fd;
                FD_SET(client_fd, &activefds);
                sprintf(write_buffer, "server: client %d just arrived\n", clients[client_fd].id);
                for (int j = 0; j <= maxfd; j++)
                {
                    if (FD_ISSET(j, &writefds) && j != i)
                        send(j, write_buffer, strlen(write_buffer), 0);
                }
                break;
            }
            else
            {
                int bytes_recv = recv(i, buffer, 999, 0);
                if (bytes_recv <= 0)
                {
                    sprintf(write_buffer, "server: client %d just left\n", clients[i].id);
                    for (int j = 0; j <= maxfd; j++)
                    {
                        if (FD_ISSET(j, &writefds) && j != i)
                            send(j, write_buffer, strlen(write_buffer), 0);
                    }
                    free(clients[i].msgs);
                    FD_CLR(i, &activefds);
                    close(i);
                    break;
                }
                buffer[bytes_recv] = '\0';
                clients[i].msgs = str_join(clients[i].msgs, buffer);

                char *msg;
                while (extract_message(&(clients[i].msgs), &msg))
                {
                    sprintf(write_buffer, "client %d: ", clients[i].id);
                    for (int j = 0; j <= maxfd; j++)
                    {
                        if (FD_ISSET(j, &writefds) && j != i)
                        {
                            send(j, write_buffer, strlen(write_buffer), 0);
                            send(j, msg, strlen(msg), 0);
                        }
                    }
                    free(msg);
                }
            }         
        }
    }
    return 0;
}

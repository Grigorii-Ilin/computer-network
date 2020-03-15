#include <windows.h> 
#include <stdio.h> 
#define PORT 3550 
#define MAXDATASIZE 100 
int main(int argc, char* argv[])
{
	WSADATA wsdata;
	WSAStartup(0x0101, &wsdata);
	int fd, numbytes;
	char buf[MAXDATASIZE];
	struct hostent* he;
	struct sockaddr_in server;
	if (argc != 2)
	{
		printf("Usage: %s <IP Address>\n", argv[0]);
		exit(-1);
	}
	if ((he = gethostbyname(argv[1])) == NULL)
	{
		printf("gethostbyname() error\n");
		exit(-1);
	}
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("socket() error\n");
		exit(-1);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr = *((struct in_addr*)he->h_addr);
	if (connect(fd, (struct sockaddr*) & server, sizeof(struct sockaddr)) == -1)
	{
		printf("connect() error\n");
		exit(-1);
	}
	if ((numbytes = recv(fd, buf, MAXDATASIZE, 0)) == -1)
	{
		printf("recv() error\n");
		exit(-1);
	}
	buf[numbytes] = '\0';
	printf("Server Message: %s\n", buf);
	closesocket(fd);
	WSACleanup();
	return -1;
}
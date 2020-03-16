#include <windows.h> 
#include <stdio.h> 

#define PORT 3550 
#define MAXDATASIZE 250 

int main(int argc, char* argv[]){
	WSADATA wsdata;
	//WSAStartup(0x0101,&wsdata); 
	//https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup
	WSAStartup(MAKEWORD(2, 2), &wsdata);
	int iConnectedSocket;
	int numbytes;
	char buf[MAXDATASIZE];
	struct hostent* he;
	struct sockaddr_in server;
	//if (argc != 2)
	//{
	//	printf("Usage: %s <IP Address>\n", argv[0]);
	//	exit(-1);
	//}
	if ((he = gethostbyname("localhost")) == NULL)	//(argv[1])
	{
		printf("gethostbyname() error\n");
		exit(-1);
	}
	if ((iConnectedSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("socket() error\n");
		exit(-1);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr = *((struct in_addr*)he->h_addr);
	if (connect(iConnectedSocket, (struct sockaddr*)&server, sizeof(struct sockaddr)) == -1)
	{
		printf("connect() error\n");
		exit(-1);
	}
	if ((numbytes = recv(iConnectedSocket, buf, MAXDATASIZE, 0)) == -1) // like read in linux
	{
		printf("recv() error\n");
		exit(-1);
	}
	buf[numbytes] = '\0';
	printf("Server Message: %s\n", buf);


	char* sendbuf = "22222";
	send(iConnectedSocket, sendbuf, (int)strlen(sendbuf), 0);

	closesocket(iConnectedSocket);
	WSACleanup();

	return 0;
}
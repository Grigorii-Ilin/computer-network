#include <windows.h> 
#include <stdio.h> 
#include <locale.h>

#define PORT 3550 
#define MAXDATASIZE 254
#define SIZE 1024
//#define PATH_MAX 255


int sendName(int sockfd, char* name);
int getNameFromPath(char* path, char* name);
int sendFile(int sockfd, char* path);


int sendFile(int sockfd, char* path) {
	FILE* fd = fopen(path, "r");
	if (!fd) {
		perror("Error: fopen");
		return 2;
	}
	char* buff = (char*)calloc(sizeof(char), SIZE);
	if (!buff) {
		printf("Error: Failed to allocate memory");
		return 3;
	}

	int read = 0;
	//send filename, MD5 then file
	if (sendName(sockfd, path) < 0) {
		perror("Error: sendName");
		return -1;
	}
	if (sendMD5(sockfd, fd) < 0) {
		perror("Error: sendMD5");
		return -1;
	}
	while ((read = fread(buff, sizeof(char), SIZE, fd)) > 0) {
		if (send(sockfd, buff, read, 0) != read) {
			perror("Error: send");
			return 4;
		}
	}
	free(buff);
	if (feof(fd)) {
		printf("File sent");
	}
	if (ferror(fd)) {
		perror("Error: read");
		fclose(fd);
		return -1;
	}
	fclose(fd);
	return 1;
}

////convert path to name and send it throught socket
//int sendName(int sockfd, char* path) {
//	char* sname = (char*)calloc(sizeof(char), MAXDATASIZE);
//	getNameFromPath(path, sname);
//	int res = send(sockfd, sname, MAXDATASIZE * sizeof(char), 0);
//	free(sname);
//	return res;
//}
//
//
//
//int getNameFromPath(char* path, char* name) {
//	char* c = path;
//	int pos = -1;
//	int i = 0;
//	for (; *c; i++) {
//		if (*c++ == '/')
//			pos = i;
//	}
//	if (path[pos + 1] != '\0') {
//		strcpy(name, path + pos + 1);
//	}
//	return 0;
//}


int main(int argc, char* argv[]){
	setlocale(LC_ALL, "RUS");

	//printf("%s", "¬ведите пароль (A-Z):\n");
	//char key[255];
	//scanf_s("%254s", key, 255);

	WSADATA wsdata;
	//https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup
	WSAStartup(MAKEWORD(2, 2), &wsdata);

	struct hostent* he = gethostbyname("localhost");
	int iConnectedSocket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr = *((struct in_addr*)he->h_addr);

	connect(iConnectedSocket, (struct sockaddr*) & server, sizeof(struct sockaddr));

	//char buf[MAXDATASIZE];
	//int numbytes = recv(iConnectedSocket, buf, MAXDATASIZE, 0);
	//buf[numbytes] = '\0';
	//printf("—ообщение от сервера: %s\n", buf);

	//decrypt(key, strlen(key), buf, numbytes);

	send(iConnectedSocket, sname, MAXDATASIZE * sizeof(char), 0);

	send(iConnectedSocket, buf, (int)strlen(buf), 0);

	closesocket(iConnectedSocket);
	WSACleanup();

	return 0;
}
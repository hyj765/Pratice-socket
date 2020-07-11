#pragma comment(lib,"ws2_32.lib");
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "userstruct.h"

#define BUFSIZE 1024

void ErrorHandling(char *message);


int connectsocket(char *ip, int port)
{
	int retval;
	
	if (retval = socket(AF_INET, SOCK_STREAM, 0) == -1)
	{
		ErrorHandling("Socket error");
	}
	struct sockaddr_in servaddr = { 0 };
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = inet_addr(ip);

	if(connect(retval,(struct sockaddr*)&servaddr,sizeof(servaddr)));
	{
		ErrorHandling("Connect Error...");
	}

	return retval;
};


int main(int argc, char **argv) {

	SOCKET clntSock;
	WSADATA wsaData;
	SOCKADDR_IN servAddr;

	char sAddr[15];
	int sPort;
	int nRcv;
	unsigned int Addr;
	char message[BUFSIZE];

	struct hostent *host;

	printf("Server Address : ");       gets(sAddr);
	printf("Port Number : ");       gets(message);
	sPort = atoi(message);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("Load WinSock 2.2 DLL Error");

	connectsocket(sAddr, sPort);

	while (1) {
		printf("\nSend Message : ");
		gets(message);

		if (strcmp(message, "exit") == 0) {
			send(clntSock, message, (int)strlen(message), 0);
			break;
		}

		send(clntSock, message, (int)strlen(message), 0);
		printf("Message Receives ...\n");

		nRcv = recv(clntSock, message, sizeof(message) - 1, 0);
		if (nRcv == SOCKET_ERROR) {
			printf("Receive Error..\n");
			break;
		}
		message[nRcv] = '\0';

		if (strcmp(message, "exit") == 0) {
			printf("Close Server Connection..\n");
			break;
		}

		printf("Receive Message : %s", message);
	}

	closesocket(clntSock);
	WSACleanup();
	printf("Close Connection..\n");
	_getch();

	return 0;
}


void ErrorHandling(char *message) {
	WSACleanup();
	fputs(message, stderr);
	fputc('\n', stderr);
	_getch();
	exit(1);
}
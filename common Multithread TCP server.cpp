#pragma comment(lib,"ws2_32")
#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>
#include<Windows.h>
#define BUFSIZE 512
#define PORT 9003

DWORD WINAPI recvn(LPVOID arg);

int main()
{
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 2),&wsa) !=0);
	{
		return 1;
	}
	int retval;

	SOCKET s_sock = socket(AF_INET, SOCK_STREAM, 0);
	

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(s_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	SOCKET c_sock;
	SOCKADDR_IN clientaddr;
	retval = listen(s_sock, SOMAXCONN);
	
	
	int len;
	HANDLE Threadone;
	while (1)
	{
		printf("waitting for client");
		len = sizeof(clientaddr);
		c_sock = accept(s_sock, (SOCKADDR*)&clientaddr, &len);
		

		Threadone = CreateThread(NULL, 0,recvn, (LPVOID)c_sock, 0, NULL);
		if (Threadone == NULL)
		{
			closesocket(c_sock);
		}
		else
		{
			CloseHandle(Threadone);
		}

	}

	closesocket(s_sock);
	WSACleanup();
	system("pause");
	return 0;
}


DWORD WINAPI recvn(LPVOID arg)
{
	SOCKET c_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE];
	addrlen = sizeof(clientaddr);
	getpeername(c_sock, (SOCKADDR*)&clientaddr,&addrlen);
	while(1)
	{ 
	
		retval=recv(c_sock, buf, sizeof(buf), 0);
		if (retval == SOCKET_ERROR)
		{
			printf("recv error");
			break;
		}
		else if (retval == 0)
		{
			break;
		}
	
		buf[retval] = '/0';
		printf("[TCP]Connecting user IP : %s PORT NUMBER : %s", inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
			
		retval=send(c_sock, buf, sizeof(buf),0);
		if (retval == SOCKET_ERROR)
		{
			printf("socket error");
			break;
		}
	
	}
	closesocket(c_sock);
	printf("[TCP]Connecting Lost IP : %s PORT NUMBER : %s", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}
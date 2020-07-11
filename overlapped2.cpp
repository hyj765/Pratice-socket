#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define FILE_FLAG_OVERLAPPED
#pragma comment(lib , "ws2_32")

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>



#define PORT 9000
#define BUFSIZE 512


struct SOCKETINFO
{
	WSAOVERLAPPED overlapped;
	SOCKET sock;
	char buf[BUFSIZE];
	int recvbyte;
	int sendbyte;
	WSABUF wsabuf;
};



SOCKET client;
HANDLE hreadevent, hwriteevent;

DWORD WINAPI workthread(LPVOID arg);
void CALLBACK CompletionRoutine(
	DWORD dwError, DWORD cbTransferred,
	LPWSAOVERLAPPED ipOverlapped, DWORD dwFlags
);

void err_quit(char *msg);

void err_display(char *msg);


int main(int argc, char *argv[])
{
	
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	SOCKET listensock = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(listensock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	listen(listensock, SOMAXCONN);

	hreadevent = CreateEvent(NULL, FALSE, TRUE, NULL);
	hwriteevent = CreateEvent(NULL, FALSE, FALSE, NULL);

	HANDLE hthread = CreateThread(NULL, 0, workthread, NULL, 0, NULL);
	
	CloseHandle(hthread);

	while (1)
	{
		WaitForSingleObject(hreadevent, INFINITE);
		client = accept(listensock, NULL, NULL);
		


		SetEvent(hwriteevent);

	}


	WSACleanup();
	return 0;


}


DWORD WINAPI workthread(LPVOID arg)
{
	int retval;
	
	while (1) {
		while (1) {
			DWORD result = WaitForSingleObjectEx(hwriteevent, INFINITE, TRUE);
			if (result == WAIT_OBJECT_0)
			{
				break;
			}
			if (result != WAIT_IO_COMPLETION)
			{
				return 1;
			}
		}

		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(client, (SOCKADDR*)&clientaddr, &addrlen);
		printf("\n [TCP Server] 클라이언트 접속 IP번호 =%s,포트번호 =%d \n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	    
		SOCKETINFO *ptr = new SOCKETINFO;
		if (ptr == NULL)
		{
			printf("메모리 부족");
			return 1;
		}
		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
		ptr->sock = client;
		SetEvent(hreadevent);
		ptr->recvbyte = ptr->sendbyte = 0;
		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = BUFSIZE;
		DWORD recvbytes;
		DWORD flags = 0;
		retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, CompletionRoutine);
		
	}
	return 0;
}

void CALLBACK CmpletionRoutine(
	DWORD dwError, DWORD cbTransferred,
	LPWSAOVERLAPPED ipoverlaaped, DWORD dwFlags)
{
	int retval;
	SOCKETINFO *ptr = (SOCKETINFO*)ipoverlaaped;
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);
	if (ptr->recvbyte == 0)
	{
		ptr->recvbyte = cbTransferred;
		ptr->sendbyte = 0;

		ptr->buf[ptr->recvbyte] = '\0';
		printf("TCP %s : %d %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), ptr->buf);
	}
	else
	{
		ptr->sendbyte += cbTransferred;
	}	

	if (ptr->recvbyte > ptr->sendbyte)
	{
	  ptr->overlapped = { 0 };
	  ptr->wsabuf.buf = ptr->buf + ptr->sendbyte;
	  ptr->wsabuf.len = ptr->recvbyte - ptr->sendbyte;
	
	  DWORD sendbytes;
	  retval = WSASend(ptr->sock, &ptr->wsabuf, 1, &sendbytes, 0, &ptr->overlapped, CompletionRoutine);

	}
	else
	{
		ptr->recvbyte = 0;
		ptr->overlapped = { 0 };
		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = BUFSIZE;
		DWORD recvbytes;
		DWORD flags = 0;
		retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, CompletionRoutine);
	}

}
	
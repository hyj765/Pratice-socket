#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib,"ws2_32")
#include<iostream>
#include<winsock2.h>
#define loopback "127.0.0.1"
using namespace std;


int main() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	SOCKET cs = socket(AF_INET, SOCK_STREAM, 0);
	if (cs == NULL) {
		cout << "client socket create fail" << endl;
	}
	SOCKADDR_IN usradr;
	usradr.sin_family = AF_INET;
	usradr.sin_port = htons(80002);
	usradr.sin_addr.s_addr = inet_addr(loopback);

	if (connect(cs, (SOCKADDR*)&usradr, sizeof(usradr)) == SOCKET_ERROR) {
		cout << "Connect Fail" << endl;
		closesocket(cs);
		WSACleanup();
		return 0;
	}
	while (1) {
		char buffer[512];
		int f = recv(cs, buffer, 512, 0);
		if (f == 0) {
			break;
		}
		cout << f << "byte Data received" << endl;
		cout << "server :" << buffer << endl;
		fflush(stdout);
	}
	closesocket(cs);
	WSACleanup();
	return 0;
}

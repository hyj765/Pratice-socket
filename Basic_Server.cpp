#pragma comment(lib, "ws2_32")
#include<iostream>
#include<WinSock2.h>
#include<string>
using namespace std;

int main() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	SOCKET s = socket(AF_INET,SOCK_STREAM ,0);
	SOCKADDR_IN adr;
	SOCKADDR_IN cadr;
	SOCKET sc;
	adr.sin_family = AF_INET;
	adr.sin_port = htons(80002);
	adr.sin_addr.S_un.S_addr = INADDR_ANY; // sever의 ip주소를 자동으로 찾아서 할당해주는 명령어
	if (0 > bind(s, (SOCKADDR*)&adr, sizeof(adr)) ) {
		cout << "binding system error" <<endl;
		closesocket(s);
		WSACleanup();
		return 0;
	}
	if (0 > listen(s, 5)) {
		cout << "listen error" << endl;
		closesocket(s);
		WSACleanup();
		return 0;
	}
	int len = sizeof(cadr);
	sc = accept(s, (SOCKADDR*)&cadr, &len);
	cout << cadr.sin_addr.S_un.S_addr << "가 접속했습니다." << endl;
	while (1) {
		string message;
		getline(cin, message);
		if (message == "quit") {
			send(sc,"", 0, 0);
			break;
		}
		send(sc, message.c_str(), sizeof(message), 0);
		
	}
	closesocket(sc);
	WSACleanup();
	return 0;
}
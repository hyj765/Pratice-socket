#pragma comment(lib,"ws2_32")
#include<stdio.h>
#include<WinSock2.h>
#include<Windows.h>
#include<string.h>
#include"protocol.h"
#include"util.h"

#define BUFSIZE 1500

int main()
{	
	int count;
	char domain[512];
	printf("input domain name :");
	gets_s(domain,sizeof(domain));
	printf("input ping count :");
	scanf_s("%d", &count);
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}
	SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	int tr;
	int optval = 1000;
	tr = setsockopt(sock, SOL_SOCKET,SO_SNDTIMEO,(char*)&optval,sizeof(optval));
	
	tr = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&optval, sizeof(optval));

	SOCKADDR_IN destaddr;
	ZeroMemory(&destaddr, sizeof(destaddr));
	destaddr.sin_family = AF_INET;
	IN_ADDR addr;
	if (GetIPAddr(domain, &addr)){
		destaddr.sin_addr = addr;
	}
	else {
		return -1;
	}
	
	ICMPMSG icmppro;
	char buf[BUFSIZE];
	SOCKADDR_IN peeraddr;
	int addrlen;
	for (int i = 1; i <= count; i++){
		ZeroMemory(&icmppro, sizeof(icmppro));
		icmppro.icmp_type = ICMP_ECHOREQUEST;
		icmppro.icmp_code = 0;
		icmppro.icmp_id = (uint16_t)GetCurrentProcessId();
		icmppro.icmp_seq = i;
		icmppro.icmp_cksum = checksum((uint16_t*)&icmppro, sizeof(icmppro));
	
		tr = sendto(sock, (char*)&icmppro, sizeof(icmppro), 0, (SOCKADDR*)&destaddr, sizeof(destaddr));
		if (tr == SOCKET_ERROR){
			if (WSAGetLastError() == WSAETIMEDOUT)
			{
				printf("Send time out\n");
				continue;
			}
			printf("Unknown error\n");
			break;
		}
		
		addrlen = sizeof(peeraddr);
		tr = recvfrom(sock, buf, BUFSIZE,0, (SOCKADDR*)&peeraddr, &addrlen);
		if (tr == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAETIMEDOUT)
			{
				printf("Recv time out\n");
				continue;
			}
			printf("Recv Unknown error\n");
			break;
		}
		DecodeICMPMessage(buf, tr, &peeraddr);

		Sleep(1000);
	}
	closesocket(sock);
	WSACleanup();
	return 0;
}


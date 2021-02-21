#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32")
#include<stdio.h>
#include<string.h>
#include<WinSock2.h>


int main(int argc,char*argv[])
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	IN_ADDR ResultIP;
	char domainname[512];

	printf("if you want quit input 'exit'\n\n\n\n");
	while (1) {
		printf("DomainName input: ");
		gets_s(domainname, sizeof(domainname));
		if (strcmp(domainname, "exit") != 0)
		{
			if (nslookup(domainname, &ResultIP) != FALSE)
			{
				printf("%s IP Address : %s\n", domainname, inet_ntoa(ResultIP));
			}
			else {
				printf("DomainName %s not found\n", domainname);
			}
		}
		else {
			break;
		}
	}
	WSACleanup();
}

BOOL nslookup(char* DomainName, IN_ADDR* ip)
{
	HOSTENT* dn = gethostbyname(DomainName);
	if (dn == NULL)
	{
		printf("gethostbyname error\n");
		return FALSE;
	}
	memcpy_s(ip, sizeof(ip), dn->h_addr_list, sizeof(dn->h_length));
	return TRUE;
}
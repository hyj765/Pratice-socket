#include<stdio.h>
#include"util.h"
#include"protocol.h"

BOOL GetIPAddr(char* name, IN_ADDR* addr)
{
	HOSTENT* pt = gethostbyname(name);
	if (pt == NULL)
	{
		printf("error get IP");
		return FALSE;
	}
	if (pt->h_addrtype != AF_INET)
	{
		printf("address type was not AF_INET");
		return FALSE;
	}
	memcpy(addr, pt->h_addr, pt->h_length);
	return TRUE;
}

uint16_t checksum(uint16_t* buf, int len)
{
	uint32_t cksum = 0;
	uint16_t* ptr = buf;
	int left = len;

	while (left > 1)
	{
		cksum += *ptr++;
		left -= sizeof(uint16_t);
	}

	if (left == 1)
	{
		cksum += *(uint16_t*)buf;
	}

	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	return (uint16_t)(~cksum);
}

void DecodeICMPMessage(char* buf, int len, SOCKADDR_IN* from)
{
	IP* ip = (IP*)buf;
	int ipheaderlen = ip->ip_hl * 4;
	ICMPMSG* icmp = (ICMPMSG*)(buf + ipheaderlen);

	if (len - ipheaderlen < 8)
	{
		printf("length error\n");
		return;
	}

	if (icmp->icmp_id != (uint16_t)GetCurrentProcessId())
	{
		printf("error NOt a reponse to our echo request!\n");
		return;
	}

	if (icmp->icmp_type != ICMP_ECHOREPLY) {
		printf("Not a echo reply packet!\n");
		return;
	}

	printf("Reply from %s : total bytes = %d, seq = %d\n", inet_ntoa(from->sin_addr), len, icmp->icmp_seq);
	return;
}

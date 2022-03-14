#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "pack_print.h"
#include "protocol.h"
#pragma once

void ethernet_print(const unsigned char* packet,int flag) {
	struct ether_header* ethernet;
	ethernet = (struct ether_header*)packet;
	printf("============packet print======================== \n");
	printf("Mac Destnation: %02x:%02x:%02x:%02x:%02x:%02x\n",
		ethernet->ether_dhost.ether_addr_octet[0],
		ethernet->ether_dhost.ether_addr_octet[1],
		ethernet->ether_dhost.ether_addr_octet[2],
		ethernet->ether_dhost.ether_addr_octet[3],
		ethernet->ether_dhost.ether_addr_octet[4],
		ethernet->ether_dhost.ether_addr_octet[5]
	);
	printf("Mac Destnation: %02x:%02x:%02x:%02x:%02x:%02x\n",
		ethernet->ether_shost.ether_addr_octet[0],
		ethernet->ether_shost.ether_addr_octet[1],
		ethernet->ether_shost.ether_addr_octet[2],
		ethernet->ether_shost.ether_addr_octet[3],
		ethernet->ether_shost.ether_addr_octet[4],
		ethernet->ether_shost.ether_addr_octet[5]
	);
	int innerflag = flag - 1;
	if (innerflag == 0) {
		return;
	}
	ip_print((char*)(packet + sizeof(ethernet)),innerflag);
}
void ip_print(const unsigned char* packet,int flag) {
	struct ip_header* ip;
	ip= (struct ip_header*)packet;
	printf("src address : %s \n", inet_ntoa(ip->ip_srcaddr));
	printf("src address : %s \n", inet_ntoa(ip->ip_destaddr));
	int innerflag = flag - 1;
	if (innerflag == 0) {
		return;
	}
	tcp_print((char*)(packet + ip->ip_header_len * 4),flag);
}
void tcp_print(const unsigned char* packet, int flag) {
	struct tcp_header* tcp = (struct packet*)packet;
	printf("Src port : %d\n" ,ntohs(tcp->source_port));
	printf("Dst port : %d\n", ntohs(tcp->dest_port));
	int innerflag = flag - 1;
	if (innerflag == 0) {
		return;
	}
}


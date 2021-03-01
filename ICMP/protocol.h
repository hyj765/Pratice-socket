#pragma once
#include<windows.h>
#include <cstdint>

#define ICMP_ECHOREQUEST 8
#define ICMP_ECHOREPLY 0

typedef struct ICMP {
	uint8_t icmp_type;
	uint8_t icmp_code;
	uint16_t icmp_cksum;
	uint16_t icmp_id;
	uint16_t icmp_seq;
}ICMPMSG;

typedef struct ip_header
{
	uint8_t ip_hl : 4;
	uint8_t ip_v : 4;
	uint8_t ip_tos;
	uint16_t ip_len;
	uint16_t ip_id;
	uint16_t ip_off;
#define IP_RF 0x8000
#define IP_DF 0x4000
#define IP_MF 0x2000
#define IP_OFFMASK 0x1fff
	uint8_t ip_ttl;
	uint8_t ip_p;
	uint16_t ip_sum;
	struct in_addr ip_src, ip_dsc;
}IP;

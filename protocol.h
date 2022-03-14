#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#pragma once


struct ether_addr
{
    unsigned char ether_addr_octet[6];
};

struct ether_header
{
    struct  ether_addr ether_dhost;
    struct  ether_addr ether_shost;
    unsigned short ether_type;
};

struct ip_header
{
    unsigned char ip_header_len : 4;
    unsigned char ip_version : 4;
    unsigned char ip_tos;
    unsigned short ip_total_length;
    unsigned short ip_id;
    unsigned char ip_frag_offset : 5;
    unsigned char ip_more_fragment : 1;
    unsigned char ip_dont_fragment : 1;
    unsigned char ip_reserved_zero : 1;
    unsigned char ip_frag_offset1;
    unsigned char ip_ttl;
    unsigned char ip_protocol;
    unsigned short ip_checksum;
    struct in_addr ip_srcaddr;
    struct in_addr ip_destaddr;
};

struct tcp_header
{
    unsigned short source_port;
    unsigned short dest_port;
    unsigned int sequence;
    unsigned int acknowledge;
    unsigned char ns : 1;
    unsigned char reserved_part1 : 3;
    unsigned char data_offset : 4;
    unsigned char fin : 1;
    unsigned char syn : 1;
    unsigned char rst : 1;
    unsigned char psh : 1;
    unsigned char ack : 1;
    unsigned char urg : 1;
    unsigned char ecn : 1;
    unsigned char cwr : 1;
    unsigned short window;
    unsigned short checksum;
    unsigned short urgent_pointer;
};
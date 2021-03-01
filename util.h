#pragma once
#include<Windows.h>
#include<cstdint>


BOOL GetIPAddr(char* name, IN_ADDR* addr);
uint16_t checksum(uint16_t* buf, int len);
void DecodeICMPMessage(char* buf, int len, SOCKADDR_IN* from);
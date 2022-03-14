#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include "protocol.h"
#include "pcap.h"
#include "pack_print.h"
#include <stdio.h>
#include <winsock2.h>

#pragma comment (lib, "wpcap.lib")
#pragma comment (lib, "ws2_32.lib" )

void device_print(pcap_if_t* device, int D_number) {
	
	printf("%d-th dev: %s ", D_number,device->name);
	if (device->description)
		printf(" (%s)\n", device->description);
	else
		printf(" (No description available)\n");
}

int main() {
	
	pcap_if_t* devices = NULL;
	char errbuf[PCAP_ERRBUF_SIZE];

	if (pcap_findalldevs(&devices, errbuf) == -1) {
		printf("device find fail\n");
		return;
	}
	pcap_if_t* d; int i;
	for (d = devices, i = 0; d != NULL; d = d->next) {
		device_print(d, i);
		i++;
	}
	int inum;

	printf("enter the interface number: ");
	scanf("%d", &inum);
	for (d = devices, i = 0; i < inum - 1; d = d->next, i++); // jump to the i-th dev
	
	pcap_t* fp;
	if ((fp = pcap_open_live(d->name,      // name of the device
		65536,                   // capture size
		1,  // promiscuous mode
		20,                    // read timeout
		errbuf
	)) == NULL) {
		printf("pcap open failed\n");
		pcap_freealldevs(devices);
		return -1;
	}
	struct pcap_pkthdr* header;
	const unsigned char* pkt_data;
	pcap_freealldevs(devices);
	int res;
	int offset = 0;
	while ((res = pcap_next_ex(fp, &header, &pkt_data)) >= 0) {
		if (res == 0) continue;
		ethernet_print(pkt_data,3);
	}
}
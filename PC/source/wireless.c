#include "general.h"

#include "wireless.h"

SOCKET listener;
SOCKET client;

struct sockaddr_in client_in;

int sockaddr_in_sizePtr = (int)sizeof(struct sockaddr_in);

//char buffer[sizeof(struct packet)];
struct packet buffer;
char hostName[80];

void initNetwork(void) {
	WSADATA wsaData;
	
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	if(gethostname(hostName, sizeof(hostName)) == SOCKET_ERROR) {
		error("gethostname()");
	}
}

void startListening(void) {
	int nret;
	
	listener = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
	if(listener == INVALID_SOCKET) {
		error("socket()");
	}
	
	SOCKADDR_IN serverInfo;
	
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr.s_addr = IP;
	serverInfo.sin_port = htons(PORT);
	
	u_long one = 1;
	ioctlsocket(listener, FIONBIO, &one);
	
	nret = bind(listener, (LPSOCKADDR)&serverInfo, sizeof(struct sockaddr));
	
	if(nret == SOCKET_ERROR) {
		error("bind()");
	}
}

void sendBuffer(int length) {
	if(sendto(listener, (char *)&buffer, length, 0, (struct sockaddr *)&client_in, sizeof(struct sockaddr_in)) != length) {
		error("sendto");
	}
}

int receiveBuffer(int length) {
	return recvfrom(listener, (char *)&buffer, length, 0, (struct sockaddr *)&client_in, &sockaddr_in_sizePtr);
}

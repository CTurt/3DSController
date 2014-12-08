#include "wireless.h"

int sock;
struct sockaddr_in sain, saout;
//char outBuf[sizeof(struct packet)], rcvBuf[sizeof(struct packet)];
struct packet outBuf, rcvBuf;

bool openSocket(int port) {
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	saout.sin_family = sain.sin_family = AF_INET;
	saout.sin_port = sain.sin_port = htons(port);
	sain.sin_addr.s_addr = INADDR_ANY;
	
	bind(sock, (struct sockaddr *)&sain, sizeof(sain));
	
	fcntl(sock, F_SETFL, O_NONBLOCK);
	
	return true;
}

void sendBuf(int length) {
	sendto(sock, (char *)&outBuf, length, 0, (struct sockaddr *)&saout, sizeof(saout));
}

void sendConnectionRequest(void) {
	outBuf.command = CONNECT;
	sendBuf(1);
}

void sendKeys(unsigned int keys, circlePosition circlePad, touchPosition touch) {
	//outBuf[0] = KEYS;
	outBuf.command = keys;
	
	//memcpy(outBuf + 1, &keys, 4);
	//memcpy(outBuf + 5, &cstick, 4);
	
	//#pragma GCC diagnostic push
	//#pragma GCC diagnostic ignored "-Wstrict-aliasing"
	//memcpy(&((struct packet *)outBuf)->keys, &keys, 4);
	//memcpy(&((struct packet *)outBuf)->cstick, &cstick, 4);
	//memcpy(&((struct packet *)outBuf)->touch, &touch, 4);
	//sendBuf(sizeof(struct packet));
	//#pragma GCC diagnostic pop
	
	memcpy(&outBuf.keys, &keys, 4);
	memcpy(&outBuf.circlePad, &circlePad, 4);
	memcpy(&outBuf.touch, &touch, 4);
	sendBuf(sizeof(struct packet));
}

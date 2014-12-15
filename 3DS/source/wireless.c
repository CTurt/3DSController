#include "keyboard.h"

#include "wireless.h"

int sock;
struct sockaddr_in sain, saout;
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
	outBuf.keyboardActive = keyboardActive;
	sendBuf(1);
}

void sendKeys(unsigned int keys, circlePosition circlePad, touchPosition touch) {
	outBuf.command = KEYS;
	outBuf.keyboardActive = keyboardActive;
	memcpy(&outBuf.keys, &keys, 4);
	memcpy(&outBuf.circlePad, &circlePad, 4);
	memcpy(&outBuf.touch, &touch, 4);
	sendBuf(sizeof(struct packet));
}

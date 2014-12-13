#pragma once

#include <string.h>

#include <3ds.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "inet_pton.h"

#define DEFAULT_PORT 8889

enum NET_COMMANDS {
	CONNECT,
	KEYS,
};

struct packet {
	unsigned char command;
	
	unsigned int keys;
	
	struct {
		short x;
		short y;
	} circlePad;
	
	struct {
		unsigned short x;
		unsigned short y;
	} touch;
};

extern int sock;
extern struct sockaddr_in sain, saout;
extern struct packet outBuf, rcvBuf;

bool openSocket(int port);
void sendBuf(int length);
void sendConnectionRequest(void);
void sendKeys(unsigned int keys, circlePosition circlePad, touchPosition touch);

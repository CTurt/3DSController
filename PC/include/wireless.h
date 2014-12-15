#pragma once

#ifndef WINVER
	#define WINVER 0x0500
#endif

#include <windows.h>
#include <winsock.h>

#define IP INADDR_ANY

enum NET_COMMANDS {
	CONNECT,
	KEYS,
};

struct packet {
	struct packetHeader {
		unsigned char command;
		unsigned char keyboardActive;
	};
	
	union {
		// CONNECT
		struct connectPacket {
		};
		
		// KEYS
		struct keysPacket {
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
	};
};

extern SOCKET listener;
extern SOCKET client;

extern struct sockaddr_in client_in;

extern int sockaddr_in_sizePtr;

extern struct packet buffer;
extern char hostName[80];

void initNetwork(void);
void printIPs(void);
void startListening(void);
void sendBuffer(int length);
int receiveBuffer(int length);

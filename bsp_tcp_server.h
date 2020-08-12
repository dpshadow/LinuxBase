#ifndef _BSP_TCP_SERVER_H
#define _BSP_TCP_SERVER_H

#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

typedef struct
{
	int sfp;
	int nfp;
	unsigned short portnum;
	struct sockaddr_in s_add;
	struct sockaddr_in c_add;
}tcp_server_typedef;

int tcp_init(tcp_server_typedef *tcp_server_typedef);

#endif


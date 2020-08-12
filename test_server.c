#include "bsp_tcp_server.h"

int main(void )
{
	int num = 0;
	char buffer[100] = {0};
	
	tcp_server_typedef tcp_server_instruct;
	
	tcp_server_instruct.portnum = 0x8888;
	
	tcp_init(&tcp_server_instruct);
	
	while (1)
	{
		memset(buffer, 0, 100);
		sprintf(buffer, "tcp test: num - %d\r\n", num++);
		send(tcp_server_instruct.nfp, buffer, strlen(buffer), 0);
	}
	
	return 0;
}

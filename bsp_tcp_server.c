#include "bsp_tcp_server.h"

int tcp_init(tcp_server_typedef *tcp_server_typedef)
{
	int sin_size;
	
	tcp_server_typedef->sfp = socket(AF_INET, SOCK_STREAM, 0);
	
	if (-1 == tcp_server_typedef->sfp)
	{
		printf("Error: socket fail\r\n");
		
		return -1;
	}
	
	printf("Infor: socket success\r\n");
	
	bzero(&tcp_server_typedef->s_add, sizeof(struct sockaddr_in));
	
	tcp_server_typedef->s_add.sin_family      = AF_INET;
	tcp_server_typedef->s_add.sin_addr.s_addr = htonl(INADDR_ANY);
	tcp_server_typedef->s_add.sin_port        = htons(tcp_server_typedef->portnum);
	
	if (-1 == bind(tcp_server_typedef->sfp, (struct sockaddr *)(&tcp_server_typedef->s_add), sizeof(struct sockaddr)))
	{
		printf("Error: bind fail\r\n");

		return -1;
	}

	printf("Infor: bind success\r\n");

	if (-1 == listen(tcp_server_typedef->sfp, 5))
	{
		printf("Error: listen fail\r\n");

		return -1;
	}

	printf("Infor: listen success\r\n");

	sin_size = sizeof(struct sockaddr_in);

	tcp_server_typedef->nfp = accept(tcp_server_typedef->sfp, (struct sockaddr *)(&tcp_server_typedef->c_add), &sin_size);

	if (-1 == tcp_server_typedef->nfp)
	{
		printf("Error: accept fail\r\n");

		return -1;
	}

	printf("accept ok\r\nServer start get connect from %#x : %#x\r\n", ntohl(tcp_server_typedef->c_add.sin_addr.s_addr), ntohs(tcp_server_typedef->c_add.sin_port));
	
	return 0;
}

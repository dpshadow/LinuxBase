#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
	int fd;
	int ret;
	
	char *i2c_bus = "/dev/i2c_control";
	unsigned char rd_buf[5];
	
	printf("i2c_bus read test\r\n");
	
	fd = open(i2c_bus, O_RDWR | O_NDELAY);
	
	if (fd < 0)
		printf("can't open i2c_bus devices\r\n");
	else
		printf("open i2c_bus devices success\r\n");
		
	rd_buf[0] = 0x18;
	rd_buf[1] = 0x0F;
	
	ret = read(fd, rd_buf, 1);
	
	if (ret < 0)
		printf("read error\r\n");
	else
		printf("read success\r\n");
		
	printf("read data is: %02x\r\n", rd_buf[0]);
	
	return ret;
}



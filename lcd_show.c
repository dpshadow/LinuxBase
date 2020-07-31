#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <malloc.h>
#include <linux/fb.h>

struct 
{
	int fb;
	unsigned int *fb_mem; //frame buffer mmap
	int fb_width;
	int fb_height;
	int fb_line_len;
	int fb_size;
	int fb_bpp;
} fb_dev;

//***********************************************
// get fb information

int get_fb_infor(int fd)
{
	struct fb_fix_screeninfo fb_finfo;
	struct fb_var_screeninfo fb_vinfo;

	if (ioctl(fd, FBIOGET_FSCREENINFO, &fb_finfo))
	{
		perror(__func__);
		return -1;
	}

	if (ioctl(fd, FBIOGET_VSCREENINFO, &fb_vinfo))
	{
		perror(__func__);
		return -1;
	}

	fb_dev.fb_width     = fb_vinfo.xres;
	fb_dev.fb_height    = fb_vinfo.yres;
	fb_dev.fb_bpp       = fb_vinfo.bits_per_pixel;
	fb_dev.fb_line_len  = fb_finfo.line_length;
	fb_dev.fb_size      = fb_finfo.smem_len;

	return 0;
}

int main(void )
{
	int fd;
	int i,j;
	unsigned int *show_pixel;
	unsigned int color = 0xFF0000;

	fd = open("/dev/fb0", O_RDWR);

	if (fd < 0)
	{
		perror(__func__);
		return -1;
	}

	get_fb_infor(fd);

	printf("fb width is:%d\r\n", fb_dev.fb_width);
	printf("fb height is:%d\r\n", fb_dev.fb_height);
	printf("fb line len is:%d\r\n", fb_dev.fb_line_len);
	printf("fb size is:%d\r\n", fb_dev.fb_size);
	printf("fb bpp is:%d\r\n", fb_dev.fb_bpp);

	// memory map
	fb_dev.fb_mem = mmap(NULL, fb_dev.fb_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	for (i=0; i<fb_dev.fb_height; i++)
	{
		for (j=0; j<fb_dev.fb_width; j++)
		{
			//*show_pixel = ((unsigned int *)fb_dev.fb_mem + fb_dev.fb_width*i + j);
			//*show_pixel = 0xFF0000;
			*(fb_dev.fb_mem + fb_dev.fb_width*i + j) = color;
		}
	}

	munmap(fb_dev.fb_mem, fb_dev.fb_size);
	close(fd);
	
	return 0;
}

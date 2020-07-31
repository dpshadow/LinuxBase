#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif
#include <linux/regulator/consumer.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <asm/uaccess.h> 
#include <linux/miscdevice.h>

MODULE_AUTHOR("DPShadow");
MODULE_LICENSE("GPL");

static struct i2c_client *this_client;

void i2c_bus_test(struct i2c_client *this_client)
{
	u8 buf1[4] = {0, 0, 0, 0};
	u8 buf2[4] = {0, 0, 0, 0};
	
	struct i2c_msg msgs[] = {
		{
			.addr	= 0x48,
			.flags	= 0,
			.len	= 1,
			.buf	= buf1,
		},
		{
			.addr	= 0x48,
			.flags	= I2C_M_RD,
			.len	= 1,
			.buf	= buf2,
		},
	};
	
	buf1[0] = 0x00;
	
	i2c_transfer(this_client->adapter, msgs, 2);
	
	printk("MSB :%02x, CSB:%02x, LSB:%02x\r\n", buf2[1], buf2[0], buf2[3]);
}

static int i2c_open_func(struct inode *inode, struct file *fp)
{
	return 0;
}

static int i2c_release_func(struct inode *inode, struct file *fp)
{
	return 0;
}

static ssize_t i2c_write_func(struct file *fp, const char __user *buf, size_t count, loff_t *pposs)
{
	int ret = 0;
	u8 len = count;
	u8 user_data[count+2];
	struct i2c_msg msgs[1];
	u8 i;

	ret = copy_from_user(&user_data, buf, count+2);
	
	for (i=0; i<count; i++)
	{
		user_data[i] = user_data[i+2];
	}

	msgs[0].addr	= user_data[0];
	msgs[0].flags	= 0;
	msgs[0].len		= len;
	msgs[0].buf		= user_data;
	
	ret = i2c_transfer(this_client->adapter, msgs, 1);
	
	if (ret < 0)
		pr_err("Can't Write\n");
	
//	ret = copy_to_user(buf, user_data, 1);
	
	return ret;
}

static ssize_t i2c_read_func(struct file *fp, char __user *buf, size_t count, loff_t *ppos)
{
	u8 i;
	int ret;
	u8 addr = 0;
	u8 len  = 0;
	u8 user_data[count+2];
	u8 rd_buf[count];
	u8 reg_addr = 0;
	
	struct i2c_msg msgs[2];/* = {
		{
			.addr	= addr,
			.flags	= 0,
			.len	= 1,
			.buf	= &reg_addr,
		},
		{
			.addr	= addr,
			.flags	= I2C_M_RD,
			.len	= len,
			.buf	= rd_buf,
		},
	};*/


	ret = copy_from_user(user_data, buf, count+2);

	// debug printf	
	for (i=0; i<count+2; i++)
		printk("buf[%d] is:%02x\r\n", i, user_data[i]);

	if (ret != 0)
		pr_err("copy user data fail\r\n");

	addr     = user_data[0];
	reg_addr = user_data[1];
	len      = count;

	msgs[0].addr	= addr;
	msgs[0].flags	= 0;
	msgs[0].len	= 1;
	msgs[0].buf	= &reg_addr;

	msgs[1].addr	= addr;
	msgs[1].flags	= I2C_M_RD;
	msgs[1].len	= len;
	msgs[1].buf	= rd_buf;
	
	ret = i2c_transfer(this_client->adapter, msgs, 2);
	
	if (ret < 0)
		pr_err("Read op error\n");
	
	ret = copy_to_user(buf, rd_buf, len);
	
	return ret;
}

static struct file_operations i2c_ops = {
	.owner		= THIS_MODULE,
	.open		= i2c_open_func,
	.release	= i2c_release_func,
	.write		= i2c_write_func,
	.read		= i2c_read_func,
};

static struct miscdevice i2c_dev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.fops	= &i2c_ops,
	.name	= "i2c_control",
};

static int i2c_bus_probe(struct i2c_client *client, const struct i2c_device_id *id)
{

	printk("Enter Probe function\r\n");

	i2c_bus_test(client);

	this_client = client;
	
	misc_register(&i2c_dev);
	
	return 0;
}

static int i2c_bus_remove(struct i2c_client *client)
{
	i2c_set_clientdata(client, NULL);
	misc_deregister(&i2c_dev);
	
	return 0;
}

static const struct i2c_device_id i2c_bus_id[] = {
	{"HDC1080", 0},
	{}
};

static struct i2c_driver i2c_bus_driver = {
	.probe		= i2c_bus_probe,
	.remove		= i2c_bus_remove,
	.id_table	= i2c_bus_id,
	.driver		= {
		.name	= "i2c_bus",
		.owner	= THIS_MODULE,
	},
};

int i2c_init(void )
{
	int rt = 0;
	
	printk("Enter Driver\r\n");
	
	rt = i2c_add_driver(&i2c_bus_driver);
	
	return rt;
}

void i2c_exit(void )
{
	i2c_del_driver(&i2c_bus_driver);
	
	printk("Exit driver\r\n");
}

late_initcall(i2c_init);
module_exit(i2c_exit);


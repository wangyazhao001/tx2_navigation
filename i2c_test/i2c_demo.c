#include<stdio.h>
#include<linux/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<errno.h>
#include<string.h>
#include "i2c.h"
#include<linux/i2c-dev.h>

int main(int argc, char** argv)
{
	struct i2c_rdwr_ioctl_data work_queue;
 
	unsigned int slave_address,reg_address,dat;
	
	int i,ret;
	unsigned char val;
	unsigned int fd;
 
	
	fd=open("/dev/i2c-1",O_RDWR);

	if(!fd)
	{
		perror("can't open i2c");
		exit(1);
	}

	ioctl(fd,I2C_TIMEOUT,2);//超时时间
	ioctl(fd,I2C_RETRIES,1);//重复次数
 
	slave_address = 0x77;   // BMP280 address    
	reg_address=0xf4;      // control address
	dat=0xf7;             // control mode
	
	//reg_address = (argv[1][2]-48)<<4 | (argv[1][3]-48);
	//dat = (argv[2][2]-48)<<4 | (argv[2][3]-48);

	//nmsgs决定了有多少start信号
	//一个msgs对应一个start信号
	//在nmsg个信号结束后总线会产生一个stop
	//下面因为在操作时序中最多用到2个start信号(字节读操作中)
	
	work_queue.nmsgs = 2;
	work_queue.msgs = (struct i2c_msg *)malloc(work_queue.nmsgs * sizeof(work_queue.msgs));

	if(!work_queue.msgs)
	{
		perror("memory alloc failed");
		close(fd);
		exit(1);
	}
	//往i2c里面写数据
	printf("began to write!\n");
	work_queue.nmsgs  = 1;	
	(work_queue.msgs[0]).len = 2;//buf的长度
	(work_queue.msgs[0]).flags = 0;//write

	(work_queue.msgs[0]).addr =(0x77);//设备地址

	(work_queue.msgs[0]).buf = (unsigned char *)malloc(2);
	(work_queue.msgs[0]).buf[0] = reg_address;//写的地址
	(work_queue.msgs[0]).buf[1] = dat;//你要写的数据
 
	ret = ioctl(fd, I2C_RDWR, (unsigned long)&work_queue);
	if(ret < 0)
	perror("error during I2C_RDWR ioctl with error code");
 	//sleep(1);

	//从i2c里面读出数据
	printf("began to read!\n");
	work_queue.nmsgs  = 2;
	//先设定一下地址
	(work_queue.msgs[0]).len = 1;
	(work_queue.msgs[0]).flags = 0;//write
	(work_queue.msgs[0]).addr = (0x77);
	(work_queue.msgs[0]).buf[0] = 0xfc;

	(work_queue.msgs[1]).len = 1;
	(work_queue.msgs[1]).flags = I2C_M_RD;
	(work_queue.msgs[1]).addr = (0x77);
	(work_queue.msgs[1]).buf = (unsigned char *)malloc(1);
	(work_queue.msgs[1]).buf[0] = 0;//初始化读缓冲
 
	ret = ioctl(fd, I2C_RDWR, (unsigned long)&work_queue);
	if(ret < 0)
		perror("error during I2C_RDWR ioctl with error code");
 
	printf("%ld\n",(work_queue.msgs[1]).buf[0]);
	close(fd);
	return 0; 
	
}

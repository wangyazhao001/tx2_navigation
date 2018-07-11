#include     <stdio.h>      
#include     <stdlib.h>     
#include     <unistd.h>     
#include     <sys/types.h>  
#include     <sys/stat.h>   
#include     <fcntl.h>      
#include     <termios.h>    
#include     <errno.h>      
#include     <time.h>
#include     <string.h>
#include "fun.h"

#define      TRUE   1
#define      FALSE  0


int speed_arr[] = { B38400, B19200, B9600, B4800, B2400, B1200, B300,
	    B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = {38400,  19200,  9600,  4800,  2400,  1200,  300,
	    38400,  19200,  9600, 4800, 2400, 1200,  300, };

char *freq[]={ 
 "B5 62 06 08 06 00 20 4E 01 00 01 00 84 00 B5 62 06 08 00 00 0E 30",
"B5 62 06 08 06 00 10 27 01 00 01 00 4D DD B5 62 06 08 00 00 0E 30",
"B5 62 06 08 06 00 88 13 01 00 01 00 B1 49 B5 62 06 08 00 00 0E 30",
"B5 62 06 08 06 00 B8 0B 01 00 01 00 D9 41 B5 62 06 08 00 00 0E 30",
"B5 62 06 08 06 00 E8 03 01 00 01 00 01 39",
"B5 62 06 08 06 00 C8 00 01 00 01 00 DE 6A B5 62 06 08 00 00 0E 30",
"B5 62 06 08 06 00 64 00 01 00 01 00 7A 12 B5 62 06 08 00 00 0E 30",
};
char *GPL[]={
"B5 62 06 09 0D 00 FF FF 00 00 00 00 00 00 FF FF 00 00 07 1F 9E",
"B5 62 06 3E 24 00 00 00 16 04 00 04 FF 00 00 00 00 00 01 01 03 00 00 00 00 00 05 00 03 00 00 00 00 00 06 08 FF 00 01 00 00 00 A0 D9 B5 62 06 3E 00 00 42 D2",

};

void set_speed(int fd, int speed)
{
  int   i;
  int   status;
  struct termios   Opt;
  tcgetattr(fd, &Opt);


  for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
   {
   	if  (speed == name_arr[i])
   	{
   	    tcflush(fd, TCIOFLUSH);
    	cfsetispeed(&Opt, speed_arr[i]);
    	cfsetospeed(&Opt, speed_arr[i]);
    	status = tcsetattr(fd, TCSANOW, &Opt);
    	if  (status != 0)
            perror("tcsetattr fd1");
     	return;
     	}
   tcflush(fd,TCIOFLUSH);
   }
}

int set_Parity(int fd,int databits,int stopbits,int parity)
{
	struct termios options;
 if  ( tcgetattr( fd,&options)  !=  0)
  {
  	perror("SetupSerial 1");
  	return(FALSE);
  }
  options.c_cflag &= ~CSIZE;
  switch (databits) 
  {
  	case 7:
  		options.c_cflag |= CS7;
  		break;
  	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		fprintf(stderr,"Unsupported data size\n");
		return (FALSE);
	}
  switch (parity)
  	{
  	case 'n':
	case 'N':
		options.c_cflag &= ~PARENB;   /* Clear parity enable */
		options.c_iflag &= ~INPCK;     /* Enable parity checking */
		break;
	case 'o':
	case 'O':
		options.c_cflag |= (PARODD | PARENB);   
		options.c_iflag |= INPCK;             /* Disnable parity checking */
		break;
	case 'e':
	case 'E':
		options.c_cflag |= PARENB;     /* Enable parity */
		options.c_cflag &= ~PARODD;     
		options.c_iflag |= INPCK;       /* Disnable parity checking */
		break;
	case 'S':
	case 's':  /*as no parity*/
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		fprintf(stderr,"Unsupported parity\n");
		return (FALSE);
		}
  	
  	switch (stopbits)
  	{
  	case 1:
  		options.c_cflag &= ~CSTOPB;
		break;
	case 2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		fprintf(stderr,"Unsupported stop bits\n");
		return (FALSE);
	}
  	/* Set input parity option */
  	if (parity != 'n')
  		options.c_iflag |= INPCK;
    	options.c_cc[VTIME] = 15; // 1.5 seconds
    	options.c_cc[VMIN] = 1;

  	tcflush(fd,TCIFLUSH); /* Update the options and do it NOW */
  	if (tcsetattr(fd,TCSANOW,&options) != 0)
  	{
  		perror("SetupSerial 3");
		return (FALSE);
	}
  	return (TRUE);
}

int OpenDev(char *Dev)
{
	int	fd = open( Dev, O_RDWR );         //| O_NOCTTY | O_NDELAY
	if (-1 == fd)
	{ 
	
		perror("Can't Open Serial Port");
		return -1;
	}
	else
		return fd;
}
/*
*@breif 	main()
*/
int main(int argc, char *argv[])
{
	int fd,res;
	FILE *fd1;
	int count=0,sum=0;
        long int length;
	int nread;
	long int nwrite;
	char buff[1024];
	char *dev ="/dev/ttyTHS2";

        char *p=GPL[1];
	
	struct timespec	time_out;
	

	fd = OpenDev(dev);

	if (fd>0)
		 set_speed(fd,9600);
	else
	{
		perror("Can't Open Serial Port!");
		exit(0);
	}
  	if (set_Parity(fd,8,1,'N')== FALSE)
  	{
    		perror("Set Parity Error\n");
    		exit(1);
  	}
/*************************************************************************/
	if(argc>1)
	{
	int flag=atoi(argv[1]);	
	switch(flag)
	{
	case 1:{
		length=strlen(p);
		if((nwrite = write(fd,p,length))==length)
		{
		printf("%s\n %ld\n %ld\n",p,nwrite,length);
		printf("write successfully***********\n");
		}
		else
		perror("write error!");
		}
		break;
	case 2:{
		p=freq[6];
		length=strlen(p);
		if((nwrite = write(fd,p,length))==length)
		{
		printf("%s\n %ld\n %ld\n",p,nwrite,length);
		printf("write successfully***********\n");
		}
		else
		perror("write error!");
		}
		break;
	default:printf("please choose 1 or 2\n");break;
	}
	}
	else
	printf("you should choose mode\n");
/*****************************************************************/
	fd1=fopen("/home/nvidia/gpsdata.txt","w");
	if(fd1 == NULL)
		perror("create file fail");

	if(res=setvbuf(fd1,NULL,_IONBF,0)!=0)
		printf("setvbuf failure\n");
/*******************************************************************/
	while(1)
  	{
	
   		while((nread = read(fd,buff,1024))>0)
   		{
			if((nread>30)&&(nread<90))
			{
			count++;
      			buff[nread+1]='\0';
      			clock_gettime(CLOCK_REALTIME, &time_out);
      
			fprintf(fd1,"%ld %ld\n %s\n",
				time_out.tv_sec,time_out.tv_nsec,buff);
		
      			printf("%ld %ld\n",
				time_out.tv_sec,time_out.tv_nsec);
  				analyse(buff);

			
			fflush(stdout);
			sum++;
			}
			else
				break;
   	 		}	
	if(count==128)
	{
	printf("totally %d message got\n",sum);
	fprintf(fd1,"totally %d message got\n",sum);
	fflush(stdout);
	sum=0;
	count=0;
	break;
	}
  	}
    	close(fd);
	fclose(fd1);
	printf("just list these datas\n");
    	exit(0);
}

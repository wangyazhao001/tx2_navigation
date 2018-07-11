#include "SocketServer.hpp"

SocketMatTransmissionServer::SocketMatTransmissionServer(void)
{
}

SocketMatTransmissionServer::~SocketMatTransmissionServer(void)
{
}

int SocketMatTransmissionServer::socketConnect(int PORT)
{
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);  
  
    struct sockaddr_in server_sockaddr;  
    server_sockaddr.sin_family = AF_INET;  
    server_sockaddr.sin_port = htons(PORT);  
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);  
  
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)  
    {  
        perror("bind");  
        return -1;  
    }  
  
    if(listen(server_sockfd,5) == -1)  
    {  
        perror("listen");  
        return -1;  
    }  
  
    struct sockaddr_in server_addr;  
    socklen_t length = sizeof(server_addr);  
  
    sockServer = accept(server_sockfd, (struct sockaddr*)&server_addr, &length);  
    if(sockServer<0)  
    {  
        perror("connect");  
        return -1;  
    }  
    else  
    {  
        printf("connect successful!\n");  
    }  
      
    close(server_sockfd); 
    return 1; 
}

void SocketMatTransmissionServer::socketDisconnect(void)
{
	close(sockServer);
}

int SocketMatTransmissionServer::transmit(cv::Mat image)
{
	if(image.empty())
	{
		printf("empty image\n");
		return -1;
	}
	if(image.cols!=IMG_WIDTH||image.rows!=IMG_HEIGHT||image.type()!=CV_8UC1)//used to be CV_8UC3
	{
		if(image.type()!=CV_8UC1)
		 printf("the image must satisfy : cols == IMG_WIDTH（%d）  rows == IMG_HEIGHT（%d） type == CV_8UC1\n\n", IMG_WIDTH, IMG_HEIGHT);  
		return -1;
	}
	for(int k=0;k<PACKAGE_NUM;k++)//将一行分成PACKAGE_num块
	{
		int num1=IMG_HEIGHT/PACKAGE_NUM*k;//num1为分快后的行首位置
		for(int i=0;i<IMG_HEIGHT/PACKAGE_NUM;i++)
		{
			//int num2=i*IMG_WIDTH*3;
			int num2=i*IMG_WIDTH;
			uchar* ucdata=image.ptr<uchar>(i+num1);
			//for(int j=0;j<IMG_WIDTH*3;j++)
			for(int j=0;j<IMG_WIDTH;j++)
			{
				data.buf[num2+j]=ucdata[j];
			}
		}
		if(k==PACKAGE_NUM-1)
			data.flag=2;
		else
			data.flag=1;
		
		if(send(sockServer,(char *)(&data),sizeof(data),0)<0)
		{
			printf("send image error: %s(errno: %d)\n", strerror(errno), errno);  
			return -1;  
		}	
	}
	return 0;
}

#include "SocketClient.hpp"  
  
SocketMatTransmissionClient::SocketMatTransmissionClient(void)  
{  
}  
  
  
SocketMatTransmissionClient::~SocketMatTransmissionClient(void)  
{  
}  
  
  
int SocketMatTransmissionClient::socketConnect(const char* IP,int PORT)
{
	struct sockaddr_in clientaddr;
	if((sockClient=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		printf("create socket error : %s (error: %d)\n",strerror(errno),errno);
		return -1;
	}
	memset(&clientaddr,0,sizeof(clientaddr));
	clientaddr.sin_family=AF_INET;
	clientaddr.sin_port=htons(PORT);
	if(inet_pton(AF_INET,IP,&clientaddr.sin_addr)<=0)//将IP地址转化为二进制
	{
		printf("inet_pton error for %s\n",IP);
		return -1;
	}
	if(connect(sockClient, (struct sockaddr*)&clientaddr,sizeof(clientaddr))<0)
	{
		 printf("connect error: %s(errno: %d)\n", strerror(errno), errno);  
		 return -1;
	}
	else
	{
		printf("connect success\n");
	}
	return 0;
}  
  
  
void SocketMatTransmissionClient::socketDisconnect(void)  
{  
    close(sockClient);  
}  
  
int SocketMatTransmissionClient::receive(cv::Mat& image)  
{
    int returnflag = 0;  
    cv::Mat img(IMG_HEIGHT, IMG_WIDTH, CV_8UC1, cv::Scalar(0,0,255));  
    needRecv = sizeof(recvBuf);  
    count = 0;  
    memset(&data,0,sizeof(data));  
  
    for (int i = 0; i < PACKAGE_NUM; i++)  
    {  
        int pos = 0;  
        int len0 = 0;  
  
        while (pos < needRecv)  
        {  
            len0 = recv(sockClient, (char*)(&data) + pos, needRecv - pos, 0);  
            if (len0 < 0)  
            {  
                printf("Server Recieve Data Failed!\n");  
                break;  
            }  
            pos += len0;  
        }  
  
        count = count + data.flag;  
  
        int num1 = IMG_HEIGHT / PACKAGE_NUM * i;  
        for (int j = 0; j < IMG_HEIGHT / PACKAGE_NUM; j++)  
        {  
            int num2 = j * IMG_WIDTH;  
            uchar* ucdata = img.ptr<uchar>(j + num1);  
            for (int k = 0; k < IMG_WIDTH; k++)  
            {  
                ucdata[k] = data.buf[num2 + k];  
            }  
        }  
  
        if (data.flag == 2)  
        {  
            if (count == PACKAGE_NUM + 1)  
            {  
                image = img;  
                returnflag = 1;  
                count = 0;  
            }  
            else  
            {  
                count = 0;  
                i = 0;  
            }  
        }  
    }  
    if(returnflag == 1)  
        return 1;  
    else  
        return -1;    
}  

#include "SocketClient.hpp"  
  
int main()  
{  
    SocketMatTransmissionClient socketMat;  
    if (socketMat.socketConnect("127.0.0.1",6666) < 0)  
    {  
        return 0;  
    }  
  
    cv::Mat image;  
    while (1)  
    {  
        if(socketMat.receive(image) > 0)  
        {  
            cv::imshow("image from socket",image);  
            cv::waitKey(30);  
        }
	else
	{
	    socketMat.socketDisconnect();
	    return -1;
	}  
    }  
  
//    socketMat.socketDisconnect();  
    return 0;  
}  

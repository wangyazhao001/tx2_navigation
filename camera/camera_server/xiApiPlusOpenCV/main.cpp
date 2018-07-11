// xiApiPlusOcvExample.cpp : program opens first camera, captures and displays 40 images

#include "header.hpp"
#include "SocketServer.hpp"
int main(int argc, char* argv[])
{
	int res=access("./picture",F_OK);
	if(res!=0)
	{
		res=mkdir("./picture",07777);//create a directory
		if(res!=0)
			printf("mkdir error\n");
	}
	SocketMatTransmissionServer socketMat;
	if(socketMat.socketConnect(6666)<0)
	{
		printf("connect error\n");
		return 0;
	}	
	
	try
	{
		// Sample for XIMEA OpenCV
		xiAPIplusCameraOcv cam;

		// Retrieving a handle to the camera device
		printf("Opening first camera...\n");
		cam.OpenFirst();
		
		// Set exposure
		cam.SetExposureTime(10000); //10000 us = 10 ms
		// Note: The default parameters of each camera might be different in different API versions
		
		printf("Starting acquisition...\n");
		cam.StartAcquisition();
		
		printf("First pixel value \n");
		XI_IMG_FORMAT format = cam.GetImageDataFormat();
		#define EXPECTED_IMAGES 500
		for (int images=0;images < EXPECTED_IMAGES;images++)
		{
			string res="";
			stringstream app;
			app<<images;
			app>>res;//将images转化为字符串
			res.append(".jpg");
			string dir="./picture/";
			dir.append(res);
			Mat cv_mat_image = cam.GetNextImageOcvMat();
			if (format == XI_RAW16 || format == XI_MONO16) 
				normalize(cv_mat_image, cv_mat_image, 0, 65536, NORM_MINMAX, -1, Mat()); // 0 - 65536, 16 bit unsigned integer range
			socketMat.transmit(cv_mat_image);
//			cv::imshow("Image from camera",cv_mat_image);
//			cv::imwrite(dir,cv_mat_image);
			cvWaitKey(20);
			//printf("\t%d\n",cv_mat_image.at<unsigned char>(0,0));
			printf("\t%d\t%d\n",cv_mat_image.cols,cv_mat_image.rows);	
	}
		
		cam.StopAcquisition();
		cam.Close();
		printf("Done\n");
		socketMat.socketDisconnect();
		cvWaitKey(500);
	}
	catch(xiAPIplus_Exception& exp)
	{
		printf("Error:\n");
		exp.PrintError();
#ifdef WIN32
		Sleep(2000);
#endif
		cvWaitKey(2000);
		return -1;
	}
	return 0;
}


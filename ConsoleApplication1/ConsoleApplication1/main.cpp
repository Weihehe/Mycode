//常量

const int DESIRED_CAMERA_WIDTH = 640;
const int DESIRED_CAMERA_HEIGHT = 480;

const char *windowName = "Cartoonifier"; 

#if !defined VK_ESCAPE
    #define VK_ESCAPE 0x1B     
#endif


#include <opencv2\opencv.hpp>  
#include<iostream>
using namespace std;
using namespace cv;
#include<windows.h>
#include"cartoons.h"
bool g_sketchMode = false;
bool g_alienMode= true;
bool g_evilMode = false;


void main(int argc, char *argv[])
{
	//设置计算机的编号，一些计算机有多个摄像机或将0作为默认摄像机编号使程序不能运行
	//将用户指定摄像机编号作为命令行参数
	int cameraNumber = 0;   
    if (argc > 1) {
        cameraNumber = atoi(argv[1]);
    }
	//打开摄像机
	cv::VideoCapture camera;
	camera.open(cameraNumber);
	
	if(!camera.isOpened())
	{
		std::cerr << "打开摄像机失败"<<endl;
		exit(1);
	}
	//设置摄像机的长和宽
	camera.set(CV_CAP_PROP_FRAME_WIDTH, DESIRED_CAMERA_WIDTH);
    camera.set(CV_CAP_PROP_FRAME_HEIGHT, DESIRED_CAMERA_HEIGHT);

	//循环更新窗口
	Sleep(2000);
	while (true) 
	{
		
		cv::Mat cameraFrame;
		camera >> cameraFrame;
		if(cameraFrame.empty())
		{
			std::cerr << "当前摄像机帧为空"<<endl;
			exit(1);
		}
		//定义一个新矩阵
		cv::Mat displayedFrame = Mat(cameraFrame.size(), CV_8UC3);

		cartoonifyImage(cameraFrame,displayedFrame,g_sketchMode,g_evilMode,g_alienMode);

		if(g_alienMode)
		{
			drawFaceStickFigureOut(displayedFrame);
		}
		imshow(windowName, displayedFrame);

		//更新GUI，暂停程序
		char keypress = waitKey(20);  
        if (keypress == VK_ESCAPE) {   
            //退出程序
            break;
        }
	}

}
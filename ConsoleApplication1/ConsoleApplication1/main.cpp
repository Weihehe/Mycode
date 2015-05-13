//����

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
bool g_artMode = false;
bool g_blurMode = false;


void main(int argc, char *argv[])
{
	//���ü�����ı�ţ�һЩ������ж���������0��ΪĬ����������ʹ����������
	//���û�ָ������������Ϊ�����в���
	int cameraNumber = 0;   
    if (argc > 1) {
        cameraNumber = atoi(argv[1]);
    }
	//�������
	cv::VideoCapture camera;
	camera.open(cameraNumber);
	
	if(!camera.isOpened())
	{
		std::cerr << "�������ʧ��"<<endl;
		exit(1);
	}
	//����������ĳ��Ϳ�
	camera.set(CV_CAP_PROP_FRAME_WIDTH, DESIRED_CAMERA_WIDTH);
    camera.set(CV_CAP_PROP_FRAME_HEIGHT, DESIRED_CAMERA_HEIGHT);

	//ѭ�����´���
	Sleep(2000);
	while (true) 
	{
		
		cv::Mat cameraFrame;
		camera >> cameraFrame;
		if(cameraFrame.empty())
		{
			std::cerr << "��ǰ�����֡Ϊ��"<<endl;
			exit(1);
		}
		//����һ���¾���
		cv::Mat displayedFrame = Mat(cameraFrame.size(), CV_8UC3);

		cartoonifyImage(cameraFrame,displayedFrame,g_sketchMode,g_evilMode,g_alienMode,g_artMode,g_blurMode);

		if(g_alienMode)
		{
			drawFaceStickFigureOut(displayedFrame);
		}
		imshow(windowName, displayedFrame);

		//����GUI����ͣ����
		char keypress = waitKey(20);  
        if (keypress == VK_ESCAPE) {   
            //�˳�����
            break;
        }
	}

}
#include <opencv2\opencv.hpp>  
#include<iostream>
using namespace std;
using namespace cv;

//����ͼ��Ĵ�������
void cartoonifyImage(Mat srcColor, Mat dst,bool sketchMode = false, bool   alienMode= false, bool evilMode = false);
//Ƥ����ɫ��
void changeFacialSkinColor(Mat smallImgBGR, Mat bigEdges);
//����ȷ���û�������λ�ã�����Ļ�м仭����������
void drawFaceStickFigureOut(Mat dst);
#include <opencv2\opencv.hpp>  
#include<iostream>
using namespace std;
using namespace cv;

//进行图像的处理函数
void cartoonifyImage(Mat srcColor, Mat dst,bool sketchMode = false, bool   alienMode= false, bool evilMode = false);
//皮肤变色器
void changeFacialSkinColor(Mat smallImgBGR, Mat bigEdges);
//用来确定用户放脸的位置，在屏幕中间画出人脸轮廓
void drawFaceStickFigureOut(Mat dst);
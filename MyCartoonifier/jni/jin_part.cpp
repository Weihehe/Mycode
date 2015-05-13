#include <jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

//#include "cartoons.h"
#include "cartoons.h"
//#include "ImageUtils.h" // Handy functions for debugging OpenCV images, by Shervin Emami.


using namespace std;
using namespace cv;
extern "C" {


// 将图像由YUV格式转换成BGRA格式
JNIEXPORT void JNICALL Java_com_Cartoonifier_CartoonifierView_ShowPreview(JNIEnv* env, jobject,
        jint width, jint height, jbyteArray yuv, jintArray bgra)
{
	//访问给定的java数组
	jbyte* _yuv = env->GetByteArrayElements(yuv,0);
	jint *_bgra = env->GetIntArrayElements(bgra,0);
	//定义了2个cv：：Mat对象，让其指向给定像素缓冲区的数据
	//myuv指向_yuv的数据，mbgra指向_bgra的数据
	Mat myuv = Mat(height+ height/2,width,CV_8UC1,(uchar *)_yuv);
	Mat mbgra = Mat(height,width,CV_8UC4,(uchar *)_bgra);

	//Android系统默认的摄像机格式是CV_YUV420sp，通过cvtColor来转换为BGRA格式
	cvtColor(myuv,mbgra,CV_YUV420sp2BGRA);

	//释放加在java数组上的本地锁
	env->ReleaseIntArrayElements(bgra,_bgra,0);
	env->ReleaseByteArrayElements(yuv,_yuv,0);
}
JNIEXPORT void
JNICALL Java_com_Cartoonifier_CartoonifierView_CartoonifyImage(JNIEnv* env, jobject,
        jint width, jint height, jbyteArray yuv, jintArray bgra,
        jboolean sketchMode, jboolean alienMode, jboolean evilMode, jboolean artMode, jboolean blurMode, jboolean drawFace)
{
		jbyte* _yuv = env->GetByteArrayElements(yuv,0);
		jint *_bgra = env->GetIntArrayElements(bgra,0);

		Mat myuv = Mat(height+ height/2,width,CV_8UC1,(uchar *)_yuv);
		Mat mbgra = Mat(height,width,CV_8UC4,(uchar *)_bgra);

		Mat mbgr(height,width,CV_8UC3);
		cvtColor(myuv,mbgr,CV_YUV420sp2BGR);

		Mat displayedFrame(mbgr.size(),CV_8UC3);

		//处理BGA的图片像素
	//	displayedFrame  = mbgr;


		if(drawFace)
		{
			displayedFrame  = mbgr;
			drawFaceStickFigureOut(displayedFrame);
		}
		else
		{
			cartoonifyImage(mbgr, displayedFrame, sketchMode, alienMode, evilMode,artMode,blurMode);
		//	if(alienMode)
		//		drawFaceStickFigureOut(displayedFrame);
		}
		cvtColor(displayedFrame,mbgra,CV_BGR2BGRA);




		env->ReleaseIntArrayElements(bgra,_bgra,0);
		env->ReleaseByteArrayElements(yuv,_yuv,0);

}



}

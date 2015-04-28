#include <jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "cartoon.h"
#include "ImageUtils.h"

using namespace cv;
extern "C" {


// ��ͼ����YUV��ʽת����BGRA��ʽ
JNIEXPORT void JNICALL Java_com_Cartoonifier_CartoonifierView_ShowPreview(JNIEnv* env, jobject,
        jint width, jint height, jbyteArray yuv, jintArray bgra)
{
	//���ʸ�����java����
	jbyte* _yuv = env->GetByteArrayElements(yuv,0);
	jint *_bgra = env->GetIntArrayElements(bgra,0);
	//������2��cv����Mat��������ָ��������ػ�����������
	//myuvָ��_yuv�����ݣ�mbgraָ��_bgra������
	Mat myuv = Mat(height+ height/2,width,CV_8UC1,(uchar *)_yuv);
	Mat mbgra = Mat(height,width,CV_8UC4,(uchar *)_bgra);

	//AndroidϵͳĬ�ϵ��������ʽ��CV_YUV420sp��ͨ��cvtColor��ת��ΪBGRA��ʽ
	cvtColor(myuv,mbgra,CV_YUV420sp2BGRA);

	//�ͷż���java�����ϵı�����
	env->ReleaseIntArrayElements(bgra,_bgra,0);
	env->ReleaseByteArrayElements(yuv,_yuv,0);
}
JNIEXPORT void
JNICALL Java_com_Cartoonifier_CartoonifierView_CartoonifyImage(
		JNIEnv* env, jobject,jint width, jint height, jbyteArray yuv, jintArray bgra)
{
		jbyte* _yuv = env->GetByteArrayElements(yuv,0);
		jint *_bgra = env->GetIntArrayElements(bgra,0);

		Mat myuv = Mat(height+ height/2,width,CV_8UC1,(uchar *)_yuv);
		Mat mbgra = Mat(height,width,CV_8UC4,(uchar *)_bgra);

		Mat mbgr(height,width,CV_8UC3);
		cvtColor(myuv,mbgr,CV_YUV420sp2BGR);

		Mat displayedFrame(mbgr.size(),CV_8UC3);

		//����BGA��ͼƬ����
		displayedFrame  = mbgr;

		cvtColor(displayedFrame,mbgra,CV_BGR2BGRA);

		env->ReleaseIntArrayElements(bgra,_bgra,0);
		env->ReleaseByteArrayElements(yuv,_yuv,0);

}



}
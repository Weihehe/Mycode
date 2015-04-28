#include "cartoons.h"

void cartoonifyImage(Mat srcColor, Mat dst,bool sketchMode, bool evilMode, bool alienMode)
{
	Mat gray;
	Mat edges;
	Mat mask;
	//ת��Ϊ�ڰ�ͼƬ
	cvtColor(srcColor, gray, CV_BGR2GRAY);
	//��ֵ�˲���
	/***
		void medianBlur(InputArray src, OutputArray dst, int ksize) Parameters:src 
		�C ֧��1��3��4ͨ��ͼƬ���룬��ksizeΪ3����5ʱ��ͼƬ�����ֻ����CV_8U,��CV_16U������ CV_32F��
		����������׾��ߴ�ֻ֧�����ΪCV_8U��dst �C ���ͼƬ��������ͼƬ��ͬ��С����ȡ�
		ksize �C ����ֱ����С��ֻ����һ������1�����������磺3, 5, 7 ...
		��ֵ�˲���ͼ���ÿ������������ (�Ե�ǰ����Ϊ���ĵ�����������)���ص���ֵ���� �� 
	***/
	//�������ص�����ֵ����7*7���������һ����ֵ����������д�����㣬��ǰ���ر�����ֵ�滻�������ã�����ȥ��
	const int MEDIAN_BLUR_FILTER_SIZE = 7;
	medianBlur(gray, gray, MEDIAN_BLUR_FILTER_SIZE);

	Size size = srcColor.size();

	if(!evilMode)
	{
		//Laplacian��Ե�˲�����ȡ��Ե
		const int LAPLACIAN_FILTER_SIZE = 5;
		Laplacian(gray,edges,CV_8U,LAPLACIAN_FILTER_SIZE);
	//	Canny( gray, edge, 3, 9,3 );
		//Ϊ��ʹ��Ե����ȥ�������裬�ɲ��ö�ֵ����ֵʹ��Եֻ�а�ɫ���ɫ
		//���threshold�����ĵ�һ������ src(x,y)>threshold��dst(x,y) = 0 ; ����dst(x,y) = src(x,y).
		const int EDGES_THRESHOLD = 80;
		threshold(edges,mask,EDGES_THRESHOLD,255,THRESH_BINARY_INV);
	}
	else
	{

		Mat edges2;
		//Scharr�˲������������x��y�����ͼ����
		//����x��y�������3*3��Scharr�ݶ��˲���
		Scharr(gray, edges, CV_8U, 1, 0);
		Scharr(gray, edges2, CV_8U, 1, 0, -1);
		edges += edges2;
		//���ýض�ֵ�ܵ͵Ķ�ֵ����ֵ�ķ���
		threshold(edges, mask, 12, 255, THRESH_BINARY_INV);
		//����3*3����ֵ�˲���
		medianBlur(mask, mask, 3);
	}
	if (sketchMode) {
		//����mask���ص�dst
        cvtColor(mask, dst, CV_GRAY2BGR);
        return;
    }


	//���ɲ�ɫͼ��Ϳ�ͨ������˫���˲�����
	//˫���˲�����ȱ����Ч�ʵͣ������ڵͷֱ�����ʹ��˫���˲������Եõ���߷ֱ��������Ƶ�Ч�����������ٶȸ��졣
	//��ͼ��ķֱ��ʼ���Ϊԭ����1/4
	Size smallSize;
    smallSize.width = size.width/2;
    smallSize.height = size.height/2;
    Mat smallImg = Mat(smallSize, CV_8UC3);
    resize(srcColor, smallImg, smallSize, 0,0, INTER_LINEAR);
	

	Mat tmp = Mat(smallSize, CV_8UC3);
    int repetitions = 4;        
    for (int i=0; i<repetitions; i++) {
        int size = 9;           
        double sigmaColor = 9;  
        double sigmaSpace = 7; 
		//˫���˲����Ĳ�����ɫ��ǿ�ȣ�λ��ǿ�ȣ���С���ظ�������
		//bilateralFilter���ܸ�����������ֵ����Ҫһ����ʱ�ı�����
        bilateralFilter(smallImg, tmp, size, sigmaColor, sigmaSpace);
        bilateralFilter(tmp, smallImg, size, sigmaColor, sigmaSpace);
    }

	if (alienMode) {
        // ������ģʽ����������Ϊ��ɫ
        changeFacialSkinColor(smallImg, edges);
    }



	Mat bigImg;
	resize(smallImg,bigImg,size,0,0,INTER_LINEAR);
	//����dst������Ԫ�ض�ֵΪ0
	dst.setTo(0);
	//��bigImg�����ظ��Ƶ�dst�ϡ������mask�ı�Ե����
	bigImg.copyTo(dst,mask);
}
void changeFacialSkinColor(Mat smallImgBGR, Mat bigEdges)
{
	//����ɫͼ��ת��ΪYUV ��ʽ
	//YCrCb��YUV��ʽ�ı��֣����Ըı�Ƥ�������Ⱥ���ɫ����RGB�޷���ɫ���л�ȡ����
	Mat yuv = Mat(smallImgBGR.size(),CV_8UC3);
	cvtColor(smallImgBGR,yuv,CV_BGR2YCrCb);

	int sw = smallImgBGR.cols;
    int sh = smallImgBGR.rows;

	//��ͼ���ⲿ��ӵ�����Ϊ1�ı�Ե����ֹˮ����䷨floodFill()Խ��
	Mat mask,maskPlusBorder;
	maskPlusBorder = Mat::zeros(sh+2,sw+2,CV_8UC1);
	mask = maskPlusBorder(Rect(1,1,sw,sh));
	resize(bigEdges, mask, smallImgBGR.size());
	// ��ֵ����ֵ
	threshold(mask,mask,80,255,THRESH_BINARY);

	//����ɾ��һЩ��Ե�ļ�϶����С���
		//���Ͳ���
		//ԭ���������滻��ǰ����λ���ؼ������ҵ����������ֵ��
		dilate(mask,mask,Mat());
		//��ʴ����
		//ԭ����ʴ�滻��ǰ����λ���ؼ������ҵ�����С����ֵ��
		erode(mask,mask,Mat());

	//��������������ص�ʹ����ˮ��䷨����֤����ͼ��Ĵ���
	int const NUM_SKIN_POINTS = 6;
    Point skinPts[NUM_SKIN_POINTS];
    skinPts[0] = Point(sw/2,          sh/2 - sh/6);
    skinPts[1] = Point(sw/2 - sw/11,  sh/2 - sh/6);
    skinPts[2] = Point(sw/2 + sw/11,  sh/2 - sh/6);
    skinPts[3] = Point(sw/2,          sh/2 + sh/16);
    skinPts[4] = Point(sw/2 - sw/9,   sh/2 + sh/16);
    skinPts[5] = Point(sw/2 + sw/9,   sh/2 + sh/16);

	//ˮ����䷨���½���Ͻ�
	const int LOWER_Y = 60;
    const int UPPER_Y = 80;
    const int LOWER_Cr = 25;
    const int UPPER_Cr = 15;
    const int LOWER_Cb = 20;
    const int UPPER_Cb = 15;
	//�������ȣ���ɫ��������ɫ����
    Scalar lowerDiff = Scalar(LOWER_Y, LOWER_Cr, LOWER_Cb);
    Scalar upperDiff = Scalar(UPPER_Y, UPPER_Cr, UPPER_Cb);

	//����flag����
	const int CONNECTED_COMPONENTS = 4;
	const int flags = CONNECTED_COMPONENTS | FLOODFILL_FIXED_RANGE | FLOODFILL_MASK_ONLY;
	Mat edgeMask = mask.clone();
	for(int i=0;i<NUM_SKIN_POINTS;i++)
	{
		//imageΪ������ͼ��seed_pointΪ�������꣬new_valΪ���ֵ�����ص㱻Ⱦɫ��ֵ��lo_diffΪ����ֵ�����޲�ֵ,up_diffΪ����ֵ�����޲�ֵ
		//�Ӻ�����ʽ�ɿ������ú����ɴ����ͨ��ͼ��
		//maskΪ���룬 ע�⣺ ������ͼ���СΪwidth * height, ������Ĵ�С����Ϊ (width+2) * (height+2) ,  
		//mask��Ϊ�����Ҳ����Ϊ���� ����flags����
		floodFill(yuv,maskPlusBorder,skinPts[i],Scalar(),NULL,lowerDiff,upperDiff,flags);
	}
	//�����ͼ�����mask��������ֵ
	/**
		ֵΪ255�ı�Ե����
		ֵΪ1��Ƥ������
		ʣ����ֵΪ0������
	*/
	//Ϊ�˵õ�ֻ��Ƥ�����ص�����
	mask -= edgeMask;

	//������ģʽ��Ƥ������
	int Red = 0;
	int Green = 70;
	int Blue = 0;
	//mask ΪƤ������
	add(smallImgBGR, Scalar(Blue, Green, Red), smallImgBGR, mask);
}
void drawFaceStickFigureOut(Mat dst)
{
	Size size = dst.size();

	Mat faceOutline = Mat::zeros(size,CV_8UC3);
	Scalar color = CV_RGB(255,255,0);
	int thickness = 4;
	int sw = size.width;
	int sh = size.height;
	int faceH = sh/2 * 70 /100;
	int faceW = faceH *72/100;
	//����������������faceOutline
	/**ellipse����
	angle��ƫת�ĽǶȡ�
	start_angle ��Բ����ʼ�ǵĽǶȡ�.
	end_angle ��Բ���ս�ǵĽǶȡ�
	color ����������ɫ��
	thickness �������Ĵ�ϸ�̶ȡ�
	line_type �����������͡�
	shift ��Բ������������ľ��ȡ�
	*/
	ellipse(faceOutline,Point(sw/2,sh/2),Size(faceW,faceH),0,0,360,color,thickness,CV_AA);

	int eyeW = faceW * 23/100;
	int eyeH = faceH * 11/100;
	int eyeX = faceW * 48/100;
	int eyeY = faceH * 13/100;

	Size eyeSize = Size(eyeW,eyeH);

	int eyeA = 15;
	int eyeYshift = 11;
	//���������ϰ벿��
	ellipse(faceOutline, Point(sw/2 - eyeX, sh/2 - eyeY), Size(eyeW, eyeH), 0, 180+eyeA, 360-eyeA, color, thickness, CV_AA);
    //���������°벿��
    ellipse(faceOutline, Point(sw/2 - eyeX, sh/2 - eyeY - eyeYshift), Size(eyeW, eyeH), 0, 0+eyeA, 180-eyeA, color, thickness, CV_AA);
    //���������ϰ벿��
    ellipse(faceOutline, Point(sw/2 + eyeX, sh/2 - eyeY), Size(eyeW, eyeH), 0, 180+eyeA, 360-eyeA, color, thickness, CV_AA);
    //���������°벿��
    ellipse(faceOutline, Point(sw/2 + eyeX, sh/2 - eyeY - eyeYshift), Size(eyeW, eyeH), 0, 0+eyeA, 180-eyeA, color, thickness, CV_AA);

	//�����촽
	int mouthY = faceH * 53/100;
    int mouthW = faceW * 45/100;
    int mouthH = faceH * 6/100;
    ellipse(faceOutline, Point(sw/2, sh/2 + mouthY), Size(mouthW, mouthH), 0, 0, 180, color, thickness, CV_AA);

	//д��
	int fontFace = FONT_HERSHEY_COMPLEX;
    float fontScale = 1.0f;
    int fontThickness = 2;
    putText(faceOutline, "Put your face here", Point(sw * 23/100, sh * 10/100), fontFace, fontScale, color, fontThickness, CV_AA);

	//������������ӵ���ʾͼ��
	addWeighted(dst, 1.0, faceOutline, 0.7, 0, dst, CV_8UC3);
}
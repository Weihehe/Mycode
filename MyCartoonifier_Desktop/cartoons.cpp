#include "cartoons.h"

void cartoonifyImage(Mat srcColor, Mat dst,bool sketchMode, bool alienMode , bool  evilMode)
{
	Mat gray;
	Mat edges;
	Mat mask;
	//转换为黑白图片
	cvtColor(srcColor, gray, CV_BGR2GRAY);
	//中值滤波器
	/***
		void medianBlur(InputArray src, OutputArray dst, int ksize) Parameters:src 
		– 支持1、3、4通道图片输入，当ksize为3或者5时，图片的深度只能是CV_8U,，CV_16U，或者 CV_32F，
		对于其他大孔径尺寸只支持深度为CV_8U。dst – 输出图片，和输入图片相同大小和深度。
		ksize – 线性直径大小，只能是一个大于1的奇数，例如：3, 5, 7 ...
		中值滤波将图像的每个像素用邻域 (以当前像素为中心的正方形区域)像素的中值代替 。 
	***/
	//即对像素的中心值及其7*7邻域组成了一个数值集，对其进行处理计算，当前像素被其中值替换掉。作用：用来去噪
	const int MEDIAN_BLUR_FILTER_SIZE = 7;
	medianBlur(gray, gray, MEDIAN_BLUR_FILTER_SIZE);

	Size size = srcColor.size();

	if(!evilMode)
	{
		//Laplacian边缘滤波器提取边缘
		const int LAPLACIAN_FILTER_SIZE = 5;
		Laplacian(gray,edges,CV_8U,LAPLACIAN_FILTER_SIZE);
	//	Canny( gray, edge, 3, 9,3 );
		//为了使边缘看上去更像素描，可采用二值化阈值使边缘只有白色或黑色
		//如果threshold函数的第一个参数 src(x,y)>threshold，dst(x,y) = 0 ; 否则dst(x,y) = src(x,y).
		const int EDGES_THRESHOLD = 80;
		threshold(edges,mask,EDGES_THRESHOLD,255,THRESH_BINARY_INV);
	}
	else
	{

		Mat edges2;
		//Scharr滤波器运算符计算x或y方向的图像差分
		//沿着x和y方向采用3*3的Scharr梯度滤波器
		Scharr(gray, edges, CV_8U, 1, 0);
		Scharr(gray, edges2, CV_8U, 1, 0, -1);
		edges += edges2;
		//采用截断值很低的二值化阈值的方法
		threshold(edges, mask, 12, 255, THRESH_BINARY_INV);
		//采用3*3的中值滤波器
		medianBlur(mask, mask, 3);
	}
	if (sketchMode) {
		//复制mask像素到dst
        cvtColor(mask, dst, CV_GRAY2BGR);
        return;
    }


	//生成彩色图像和卡通，采用双边滤波器，
	//双边滤波器的缺点是效率低，所以在低分辨率下使用双边滤波器，以得到与高分辨率下相似的效果，但运行速度更快。
	//将图像的分辨率减少为原来的1/4
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
		//双边滤波器的参数：色彩强度，位置强度，大小，重复计数。
		//bilateralFilter不能覆盖它的输入值，需要一个临时的变量。
        bilateralFilter(smallImg, tmp, size, sigmaColor, sigmaSpace);
        bilateralFilter(tmp, smallImg, size, sigmaColor, sigmaSpace);
    }

	if (alienMode) {
        // 外星人模式，将人脸变为绿色
        changeFacialSkinColor(smallImg, edges);
    }



	Mat bigImg;
	resize(smallImg,bigImg,size,0,0,INTER_LINEAR);
	//设置dst的所有元素都值为0
	dst.setTo(0);
	//想bigImg的像素复制到dst上。并添加mask的边缘像素
	bigImg.copyTo(dst,mask);
}
void changeFacialSkinColor(Mat smallImgBGR, Mat bigEdges)
{
	//将彩色图像转换为YUV 格式
	//YCrCb是YUV格式的变种，可以改变皮肤的亮度和颜色，而RGB无法从色彩中获取亮度
	Mat yuv = Mat(smallImgBGR.size(),CV_8UC3);
	cvtColor(smallImgBGR,yuv,CV_BGR2YCrCb);

	int sw = smallImgBGR.cols;
    int sh = smallImgBGR.rows;

	//在图像外部添加的像素为1的边缘，防止水漫填充法floodFill()越界
	Mat mask,maskPlusBorder;
	maskPlusBorder = Mat::zeros(sh+2,sw+2,CV_8UC1);
	mask = maskPlusBorder(Rect(1,1,sw,sh));
	resize(bigEdges, mask, smallImgBGR.size());
	// 二值化阈值
	threshold(mask,mask,80,255,THRESH_BINARY);

	//用来删除一些边缘的间隙，减小误差
		//膨胀操作
		//原理：膨胀是替换当前像素位像素集合中找到的最大像素值。
		dilate(mask,mask,Mat());
		//腐蚀操作
		//原理：腐蚀替换当前像素位像素集合中找到的最小像素值。
		erode(mask,mask,Mat());

	//对人脸的许多像素点使用漫水填充法，保证人脸图像的处理
	int const NUM_SKIN_POINTS = 6;
    Point skinPts[NUM_SKIN_POINTS];
    skinPts[0] = Point(sw/2,          sh/2 - sh/6);
    skinPts[1] = Point(sw/2 - sw/11,  sh/2 - sh/6);
    skinPts[2] = Point(sw/2 + sw/11,  sh/2 - sh/6);
    skinPts[3] = Point(sw/2,          sh/2 + sh/16);
    skinPts[4] = Point(sw/2 - sw/9,   sh/2 + sh/16);
    skinPts[5] = Point(sw/2 + sw/9,   sh/2 + sh/16);

	//水漫填充法的下界和上界
	const int LOWER_Y = 60;
    const int UPPER_Y = 80;
    const int LOWER_Cr = 25;
    const int UPPER_Cr = 15;
    const int LOWER_Cb = 20;
    const int UPPER_Cb = 15;
	//定义亮度，红色分量，蓝色分量
    Scalar lowerDiff = Scalar(LOWER_Y, LOWER_Cr, LOWER_Cb);
    Scalar upperDiff = Scalar(UPPER_Y, UPPER_Cr, UPPER_Cb);

	//定义flag参数
	const int CONNECTED_COMPONENTS = 4;
	const int flags = CONNECTED_COMPONENTS | FLOODFILL_FIXED_RANGE | FLOODFILL_MASK_ONLY;
	Mat edgeMask = mask.clone();
	for(int i=0;i<NUM_SKIN_POINTS;i++)
	{
		//image为待处理图像，seed_point为种子坐标，new_val为填充值，像素点被染色的值，lo_diff为像素值的下限差值,up_diff为像素值的上限差值
		//从函数形式可看出，该函数可处理多通道图像。
		//mask为掩码， 注意： 设输入图像大小为width * height, 则掩码的大小必须为 (width+2) * (height+2) ,  
		//mask可为输出，也可作为输入 ，由flags决定
		floodFill(yuv,maskPlusBorder,skinPts[i],Scalar(),NULL,lowerDiff,upperDiff,flags);
	}
	//处理后图像变量mask包含如下值
	/**
		值为255的边缘像素
		值为1的皮肤像素
		剩下是值为0的像素
	*/
	//为了得到只有皮肤像素的掩码
	mask -= edgeMask;

	//外星人模式的皮肤像素
	int Red = 0;
	int Green = 70;
	int Blue = 0;
	//mask 为皮肤掩码
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
	//画脸。将参数传入faceOutline
	/**ellipse函数
	angle：偏转的角度。
	start_angle ：圆弧起始角的角度。.
	end_angle ：圆弧终结角的角度。
	color ：线条的颜色。
	thickness ：线条的粗细程度。
	line_type ：线条的类型。
	shift ：圆心坐标点和数轴的精度。
	*/
	ellipse(faceOutline,Point(sw/2,sh/2),Size(faceW,faceH),0,0,360,color,thickness,CV_AA);

	int eyeW = faceW * 23/100;
	int eyeH = faceH * 11/100;
	int eyeX = faceW * 48/100;
	int eyeY = faceH * 13/100;

	Size eyeSize = Size(eyeW,eyeH);

	int eyeA = 15;
	int eyeYshift = 11;
	//画出右眼上半部分
	ellipse(faceOutline, Point(sw/2 - eyeX, sh/2 - eyeY), Size(eyeW, eyeH), 0, 180+eyeA, 360-eyeA, color, thickness, CV_AA);
    //画出右眼下半部分
    ellipse(faceOutline, Point(sw/2 - eyeX, sh/2 - eyeY - eyeYshift), Size(eyeW, eyeH), 0, 0+eyeA, 180-eyeA, color, thickness, CV_AA);
    //画出左眼上半部分
    ellipse(faceOutline, Point(sw/2 + eyeX, sh/2 - eyeY), Size(eyeW, eyeH), 0, 180+eyeA, 360-eyeA, color, thickness, CV_AA);
    //画出左眼下半部分
    ellipse(faceOutline, Point(sw/2 + eyeX, sh/2 - eyeY - eyeYshift), Size(eyeW, eyeH), 0, 0+eyeA, 180-eyeA, color, thickness, CV_AA);

	//画出嘴唇
	int mouthY = faceH * 53/100;
    int mouthW = faceW * 45/100;
    int mouthH = faceH * 6/100;
    ellipse(faceOutline, Point(sw/2, sh/2 + mouthY), Size(mouthW, mouthH), 0, 0, 180, color, thickness, CV_AA);

	//写字
	int fontFace = FONT_HERSHEY_COMPLEX;
    float fontScale = 1.0f;
    int fontThickness = 2;
    putText(faceOutline, "Put your face here", Point(sw * 23/100, sh * 10/100), fontFace, fontScale, color, fontThickness, CV_AA);

	//将人脸轮廓添加到显示图层
	addWeighted(dst, 1.0, faceOutline, 0.7, 0, dst, CV_8UC3);
}
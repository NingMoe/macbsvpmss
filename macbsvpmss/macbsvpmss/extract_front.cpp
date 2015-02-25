/*
	计算差分图像并二值化的函数，修改自高云晖师兄的毕业设计代码
	该函数暂时无法处理大视频
*/
#include "macbsvpmss.h"

//input[]代表输入视频的绝对路径字符串,back_img[]代表背景图片的绝对路径字符串
void ExtraFront(char input[],char back_img[])
{
	// 读入背景（已经通过求平均处理好的背景灰度图，运行时若读取路径中没该名称的图像则会报错）
	IplImage* background = cvLoadImage(back_img, 1);
	// 创建存储背景灰度图的变量 
	IplImage* background_gray = cvCreateImage(cvGetSize(background), IPL_DEPTH_8U, 1);
	// 将背景转成灰度图 
	cvCvtColor(background, background_gray, CV_BGR2GRAY);
	// 创建存储矩阵格式的背景灰度图的变量并将初始背景转化为矩阵格式 
	CvMat* background_grayMat = cvCreateMat(background -> height, background -> width, CV_32FC1);
	cvConvert(background_gray, background_grayMat);	
	// 创建与原背景进行加权叠加的背景更新图 
	CvMat* background_renewMat = cvCreateMat(background -> height, background -> width, CV_32FC1);
	// 创建存储当前帧及其灰度图的变量 
	IplImage* frame = NULL;
	IplImage* frame_gray = NULL;
	// 创建中值滤波后的当前帧灰度图以及存储其矩阵格式的变量 
	IplImage* frame_median = NULL;
	CvMat* frame_medianMat = NULL;
	// 创建存储前景的变量 
	IplImage* front = NULL;
	CvMat* frontMat = NULL;
	// 创建存储二值化后的前景的变量 
	IplImage* front_bin = NULL;
	CvMat* front_binMat = NULL;
	// 读入视频并设置视频帧到建立背景后的下一帧 
	CvCapture* capture = cvCreateFileCapture(input);
	int frame_count = 30;	//这里假设了用前1000帧图像进行求平均处理作为背景，所以对于视频帧的前景掩膜输出从第1001帧开始
	cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, frame_count);
	// 创建显示输入以及输出的视频的窗口 
	cvNamedWindow("avi_input");
	cvNamedWindow("avi_output");

	double fps = cvGetCaptureProperty( capture, CV_CAP_PROP_FPS );
	CvSize size = cvSize( 
		(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
		(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
	CvVideoWriter* wrVideo = cvCreateVideoWriter("F:\\vs2005\\out.avi", CV_FOURCC('X','V','I','D'), fps, size);
	while(1)
	{	
		frame = cvQueryFrame(capture);	//读取当前帧		

		if (!frame)
			break;
		// 将当前帧转化为灰度图	
		if (frame_gray == NULL)
			frame_gray = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		cvCvtColor(frame, frame_gray, CV_BGR2GRAY);
		// 对当前帧进行中值滤波 
		if (frame_median == NULL)
		{
			frame_median = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
			frame_medianMat = cvCreateMat(frame -> height, frame -> width, CV_32FC1);
		}
		cvSmooth(frame_gray, frame_median, CV_MEDIAN);	//默认为3*3的掩膜
		cvConvert(frame_median, frame_medianMat);
		// 进行减景操作得到前景 
		if (front == NULL)
		{
			front = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
			frontMat = cvCreateMat(frame -> height, frame -> width, CV_32FC1);
		}
		cvAbsDiff(frame_medianMat, background_grayMat, frontMat);
		cvConvert(frontMat, front);
		// 对前景进行二值化，算法为改进的OTSU 
		if (front_bin == NULL)
		{
			front_bin = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
			front_binMat = cvCreateMat(frame -> height, frame -> width, CV_32FC1);
		}
		int threshold = Otsu(front);
		cvThreshold(front, front_bin, threshold, 255, CV_THRESH_BINARY); 
		// 对二值化后的前景做开运算 
		cvErode(front_bin, front_bin);	//腐蚀,迭代次数1
		cvDilate(front_bin, front_bin);	//再膨胀,迭代次数1
		cvConvert(front_bin, front_binMat);
		// 对背景进行更新 
		SurendraRenew(front_binMat, frame_medianMat, background_grayMat, background_renewMat);	//Surendra背景更新算法
		cvRunningAvg(background_renewMat, background_grayMat, 0.03, 0);		//将背景更新图与原背景进行加权叠加，权值为0.03
		// 显示当前帧以及二值化后的前景 
		cvShowImage("avi_input", frame);
		cvShowImage("avi_output", front_bin);
		frame_count++;
		cvWriteFrame( wrVideo, front_bin);
		char c1 = cvWaitKey(30);	//每帧延时30ms
		//若按下esc键，进行相关图片的保存，若按下回车键则直接退出程序 
		if (c1 == 27)
		{
			// 保存当前帧灰度图 
			char savename_frame[50];
			sprintf(savename_frame, "%s%d%s", "F:\\vs2005\\frame_gray_", frame_count, ".png");
			cvSaveImage(savename_frame, frame_gray);
			cvNamedWindow("frame_gray");
			cvShowImage("frame_gray", frame_gray);
			// 保存减景操作结果 
			char savename_front[50];
			sprintf(savename_front, "%s%d%s", "F:\\vs2005\\front_", frame_count, ".png");
			cvSaveImage(savename_front, front);
			cvNamedWindow("front");
			cvShowImage("front", front);
			// 保存二值化结果 
			char savename_frontbin[50];
			sprintf(savename_frontbin, "%s%d%s", "F:\\vs2005\\front_bin_", frame_count, ".png");
			cvSaveImage(savename_frontbin, front_bin);
			// 保存当前背景 
			cvConvert(background_grayMat, background_gray);
			char savename_background[50];
			sprintf(savename_background, "%s%d%s", "F:\\vs2005\\background_", frame_count, ".png");
			cvSaveImage(savename_background, background_gray);
			cvNamedWindow("background");
			cvShowImage("background", background_gray);
			// 输出当前帧数以及阈值 
			printf("%s%d%s", "frames: ", frame_count, "	");
			printf("%s%d\n", "threshold: ", threshold);

			// 等待按键响应，若按下esc键，则继续运行程序，否则退出程序 
			char c2 = cvWaitKey(0);
			if(c2 != 27)
				break;
		}
		else if (c1 == 13)
			break;


	}

	cvReleaseCapture(&capture);
	cvDestroyAllWindows();

	cvReleaseImage(&background);
	cvReleaseImage(&frame_gray);
	cvReleaseImage(&frame_median);
	cvReleaseImage(&background_gray);
	cvReleaseImage(&front);
	cvReleaseImage(&front_bin);

	cvReleaseMat(&frame_medianMat);
	cvReleaseMat(&background_grayMat);
	cvReleaseMat(&frontMat);
	cvReleaseMat(&front_binMat);
	cvReleaseMat(&background_renewMat);

}
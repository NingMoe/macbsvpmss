/*
	������ͼ�񲢶�ֵ���ĺ������޸��Ը�����ʦ�ֵı�ҵ��ƴ���
	�ú�����ʱ�޷��������Ƶ
*/
#include "macbsvpmss.h"

//input[]����������Ƶ�ľ���·���ַ���,back_img[]������ͼƬ�ľ���·���ַ���
void ExtraFront(char input[],char back_img[])
{
	// ���뱳�����Ѿ�ͨ����ƽ������õı����Ҷ�ͼ������ʱ����ȡ·����û�����Ƶ�ͼ����ᱨ��
	IplImage* background = cvLoadImage(back_img, 1);
	// �����洢�����Ҷ�ͼ�ı��� 
	IplImage* background_gray = cvCreateImage(cvGetSize(background), IPL_DEPTH_8U, 1);
	// ������ת�ɻҶ�ͼ 
	cvCvtColor(background, background_gray, CV_BGR2GRAY);
	// �����洢�����ʽ�ı����Ҷ�ͼ�ı���������ʼ����ת��Ϊ�����ʽ 
	CvMat* background_grayMat = cvCreateMat(background -> height, background -> width, CV_32FC1);
	cvConvert(background_gray, background_grayMat);	
	// ������ԭ�������м�Ȩ���ӵı�������ͼ 
	CvMat* background_renewMat = cvCreateMat(background -> height, background -> width, CV_32FC1);
	// �����洢��ǰ֡����Ҷ�ͼ�ı��� 
	IplImage* frame = NULL;
	IplImage* frame_gray = NULL;
	// ������ֵ�˲���ĵ�ǰ֡�Ҷ�ͼ�Լ��洢������ʽ�ı��� 
	IplImage* frame_median = NULL;
	CvMat* frame_medianMat = NULL;
	// �����洢ǰ���ı��� 
	IplImage* front = NULL;
	CvMat* frontMat = NULL;
	// �����洢��ֵ�����ǰ���ı��� 
	IplImage* front_bin = NULL;
	CvMat* front_binMat = NULL;
	// ������Ƶ��������Ƶ֡���������������һ֡ 
	CvCapture* capture = cvCreateFileCapture(input);
	int frame_count = 30;	//�����������ǰ1000֡ͼ�������ƽ��������Ϊ���������Զ�����Ƶ֡��ǰ����Ĥ����ӵ�1001֡��ʼ
	cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, frame_count);
	// ������ʾ�����Լ��������Ƶ�Ĵ��� 
	cvNamedWindow("avi_input");
	cvNamedWindow("avi_output");

	double fps = cvGetCaptureProperty( capture, CV_CAP_PROP_FPS );
	CvSize size = cvSize( 
		(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
		(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
	CvVideoWriter* wrVideo = cvCreateVideoWriter("F:\\vs2005\\out.avi", CV_FOURCC('X','V','I','D'), fps, size);
	while(1)
	{	
		frame = cvQueryFrame(capture);	//��ȡ��ǰ֡		

		if (!frame)
			break;
		// ����ǰ֡ת��Ϊ�Ҷ�ͼ	
		if (frame_gray == NULL)
			frame_gray = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		cvCvtColor(frame, frame_gray, CV_BGR2GRAY);
		// �Ե�ǰ֡������ֵ�˲� 
		if (frame_median == NULL)
		{
			frame_median = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
			frame_medianMat = cvCreateMat(frame -> height, frame -> width, CV_32FC1);
		}
		cvSmooth(frame_gray, frame_median, CV_MEDIAN);	//Ĭ��Ϊ3*3����Ĥ
		cvConvert(frame_median, frame_medianMat);
		// ���м��������õ�ǰ�� 
		if (front == NULL)
		{
			front = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
			frontMat = cvCreateMat(frame -> height, frame -> width, CV_32FC1);
		}
		cvAbsDiff(frame_medianMat, background_grayMat, frontMat);
		cvConvert(frontMat, front);
		// ��ǰ�����ж�ֵ�����㷨Ϊ�Ľ���OTSU 
		if (front_bin == NULL)
		{
			front_bin = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
			front_binMat = cvCreateMat(frame -> height, frame -> width, CV_32FC1);
		}
		int threshold = Otsu(front);
		cvThreshold(front, front_bin, threshold, 255, CV_THRESH_BINARY); 
		// �Զ�ֵ�����ǰ���������� 
		cvErode(front_bin, front_bin);	//��ʴ,��������1
		cvDilate(front_bin, front_bin);	//������,��������1
		cvConvert(front_bin, front_binMat);
		// �Ա������и��� 
		SurendraRenew(front_binMat, frame_medianMat, background_grayMat, background_renewMat);	//Surendra���������㷨
		cvRunningAvg(background_renewMat, background_grayMat, 0.03, 0);		//����������ͼ��ԭ�������м�Ȩ���ӣ�ȨֵΪ0.03
		// ��ʾ��ǰ֡�Լ���ֵ�����ǰ�� 
		cvShowImage("avi_input", frame);
		cvShowImage("avi_output", front_bin);
		frame_count++;
		cvWriteFrame( wrVideo, front_bin);
		char c1 = cvWaitKey(30);	//ÿ֡��ʱ30ms
		//������esc�����������ͼƬ�ı��棬�����»س�����ֱ���˳����� 
		if (c1 == 27)
		{
			// ���浱ǰ֡�Ҷ�ͼ 
			char savename_frame[50];
			sprintf(savename_frame, "%s%d%s", "F:\\vs2005\\frame_gray_", frame_count, ".png");
			cvSaveImage(savename_frame, frame_gray);
			cvNamedWindow("frame_gray");
			cvShowImage("frame_gray", frame_gray);
			// �������������� 
			char savename_front[50];
			sprintf(savename_front, "%s%d%s", "F:\\vs2005\\front_", frame_count, ".png");
			cvSaveImage(savename_front, front);
			cvNamedWindow("front");
			cvShowImage("front", front);
			// �����ֵ����� 
			char savename_frontbin[50];
			sprintf(savename_frontbin, "%s%d%s", "F:\\vs2005\\front_bin_", frame_count, ".png");
			cvSaveImage(savename_frontbin, front_bin);
			// ���浱ǰ���� 
			cvConvert(background_grayMat, background_gray);
			char savename_background[50];
			sprintf(savename_background, "%s%d%s", "F:\\vs2005\\background_", frame_count, ".png");
			cvSaveImage(savename_background, background_gray);
			cvNamedWindow("background");
			cvShowImage("background", background_gray);
			// �����ǰ֡���Լ���ֵ 
			printf("%s%d%s", "frames: ", frame_count, "	");
			printf("%s%d\n", "threshold: ", threshold);

			// �ȴ�������Ӧ��������esc������������г��򣬷����˳����� 
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
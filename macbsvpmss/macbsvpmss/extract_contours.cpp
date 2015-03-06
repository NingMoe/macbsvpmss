/*
	提取轮廓并用矩形圈出目标轮廓的函数
	该函数有待扩展为连续帧轮廓的追踪
*/
#include "macbsvpmss.h"

//input代表背景图片的绝对路径字符串
void ExtraContours(char input[])
{
	IplImage* test_gray = cvLoadImage(input,0);
	//IplImage* test_gray = cvCreateImage(cvGetSize(test), IPL_DEPTH_8U, 1);
	CvMemStorage *ms = cvCreateMemStorage();
	CvSeq *seq = NULL;  
	cvNamedWindow("test");
	cvShowImage("test",test_gray);
	cvWaitKey(0);
	//默认：mode=CV_RETR_LIST，检索所偶轮廓
	int cnt = cvFindContours(test_gray,ms,&seq,sizeof(CvContour),2);
	for(;seq;seq=seq->h_next)
	{
		//当前轮廓包含多少个元素，这里的元素为点
		int c=seq->total;
		 //得到指定的那个轮廓的周长
		double length = cvArcLength(seq);
		//该函数有3个参数：序列，起点（默认计算整条曲线），是否封闭曲线
		double area = cvContourArea(seq);  //得到指定的那个轮廓的面积
		CvRect rect = cvBoundingRect(seq,1);  //根据序列，返回轮廓外围矩形；
		CvBox2D box = cvMinAreaRect2(seq,NULL); //最小外围矩形 

		std::cout<<"Length = "<<length<<std::endl;  
		std::cout<<"Area = "<<area<<std::endl;  
		if(length<1000)
			continue;
		CvPoint pt1,pt2;
		pt1.x=rect.x;
		pt1.y=rect.y;
		pt2.x=rect.x+rect.width;
		pt2.y=rect.y+rect.height;

		//创建一个存储目标图像的内存
		IplImage *dst = cvCreateImage(cvGetSize(test_gray),8,3); //目标图像为3通道图
		cvZero(dst);  
		cvDrawContours(dst,seq,CV_RGB(255,255,255),CV_RGB(0,0,0),0);  
		cvRectangle(dst,pt1,pt2,CV_RGB(0,255,0)); 
		cvNamedWindow("dst",1);
		cvShowImage("dst",dst);  
		cvWaitKey();  
		cvReleaseImage(&dst); 
	}
	cvReleaseImage(&test_gray);  

	cvReleaseMemStorage(&ms);    
}

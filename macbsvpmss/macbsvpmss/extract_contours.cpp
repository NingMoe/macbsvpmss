/*
	��ȡ�������þ���Ȧ��Ŀ�������ĺ���
	�ú����д���չΪ����֡������׷��
*/
#include "macbsvpmss.h"

//input������ͼƬ�ľ���·���ַ���
void ExtraContours(char input[])
{
	IplImage* test_gray = cvLoadImage(input,0);
	//IplImage* test_gray = cvCreateImage(cvGetSize(test), IPL_DEPTH_8U, 1);
	CvMemStorage *ms = cvCreateMemStorage();
	CvSeq *seq = NULL;  
	cvNamedWindow("test");
	cvShowImage("test",test_gray);
	cvWaitKey(0);
	//Ĭ�ϣ�mode=CV_RETR_LIST��������ż����
	int cnt = cvFindContours(test_gray,ms,&seq,sizeof(CvContour),2);
	for(;seq;seq=seq->h_next)
	{
		//��ǰ�����������ٸ�Ԫ�أ������Ԫ��Ϊ��
		int c=seq->total;
		 //�õ�ָ�����Ǹ��������ܳ�
		double length = cvArcLength(seq);
		//�ú�����3�����������У���㣨Ĭ�ϼ����������ߣ����Ƿ�������
		double area = cvContourArea(seq);  //�õ�ָ�����Ǹ����������
		CvRect rect = cvBoundingRect(seq,1);  //�������У�����������Χ���Σ�
		CvBox2D box = cvMinAreaRect2(seq,NULL); //��С��Χ���� 

		std::cout<<"Length = "<<length<<std::endl;  
		std::cout<<"Area = "<<area<<std::endl;  
		if(length<1000)
			continue;
		CvPoint pt1,pt2;
		pt1.x=rect.x;
		pt1.y=rect.y;
		pt2.x=rect.x+rect.width;
		pt2.y=rect.y+rect.height;

		//����һ���洢Ŀ��ͼ����ڴ�
		IplImage *dst = cvCreateImage(cvGetSize(test_gray),8,3); //Ŀ��ͼ��Ϊ3ͨ��ͼ
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

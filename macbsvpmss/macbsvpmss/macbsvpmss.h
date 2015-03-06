#ifndef GUARD_macbsvpmss_h
#define GUARD_macbsvpmss_h

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv/cv.hpp>
#include <list>
#include <vector>
struct tracedata
{
	CvRect rect;
	long num;
};
//������ȡ
IplImage* ExtractBackground(char input[],int k);
//ǰ����ȡ����ֵ��
void ExtraFront(char input[],char background[]);
//ѡ��Ŀ��
void ExtraContours(char input[]);
//ǰ����ȡ����ֵ�㷨
int Otsu(IplImage* src);
//���������㷨
void SurendraRenew(CvMat* bin, CvMat* frame, CvMat* background, CvMat* background_renew);
//��������
void UpdateContour(std::vector<tracedata>src,std::vector<tracedata> des);
#endif
#ifndef GUARD_macbsvpmss_h
#define GUARD_macbsvpmss_h

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv/cv.hpp>

//������ȡ
IplImage* extractBackground(char input[],int k);
//ǰ����ȡ����ֵ��
void extraFront(char input[],char background[]);
//ѡ��Ŀ��
void extraContours(char input[]);
//ǰ����ȡ����ֵ�㷨
int Otsu(IplImage* src);
//���������㷨
void SurendraRenew(CvMat* bin, CvMat* frame, CvMat* background, CvMat* background_renew);

#endif
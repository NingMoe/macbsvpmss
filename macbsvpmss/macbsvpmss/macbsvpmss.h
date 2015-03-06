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
//背景提取
IplImage* ExtractBackground(char input[],int k);
//前景提取并二值化
void ExtraFront(char input[],char background[]);
//选中目标
void ExtraContours(char input[]);
//前景提取的阈值算法
int Otsu(IplImage* src);
//背景更新算法
void SurendraRenew(CvMat* bin, CvMat* frame, CvMat* background, CvMat* background_renew);
//轮廓更新
void UpdateContour(std::vector<tracedata>src,std::vector<tracedata> des);
#endif
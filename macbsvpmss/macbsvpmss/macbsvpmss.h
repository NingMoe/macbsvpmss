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
IplImage* ExtractBackground(char*,int,int );
//前景提取并二值化
void ExtraFront(IplImage*,IplImage*,IplImage*);
//选中目标
void ExtractContours(IplImage*,IplImage*);
//前景提取的阈值算法
int Otsu(IplImage* );
//背景更新算法
void SurendraRenew(CvMat*, CvMat*, CvMat*, CvMat*);
//轮廓更新
void UpdateContour(std::vector<tracedata>,std::vector<tracedata>);
//增强对比度
void RatioAdjust(IplImage*,IplImage*);
//锁定车辆
void MobileDectect(char*,char*);
#endif
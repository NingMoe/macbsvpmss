#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv/cv.hpp>
#include<cstring>
#include "macbsvpmss.h"

char video[][100] ={
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\a2.avi",
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\t1.avi"
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\789759_6.avi",
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\790071_6.avi",
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\790100_5.avi",
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\790100_8.avi",
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\790292_5.avi",
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\790292_8.avi",
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\791745_7.avi",
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\791828_5.avi",
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\791828_8.avi",
};
//第一个参数是视频路径，第二个参数是背景图片路径
int main(int argc,char **argv)
{
	int start;
	int end;
	int opration = 0;
	std::cout<<"选择你的操作:\n1.背景提取. 2.车辆跟踪"<<std::endl;
	while(opration != 1 && opration != 2)
	{
		std::cin>>opration;
	}
	switch(opration)
	{
	case 1:
		std::cout<<"输入开始帧和结束帧:"<<std::endl;
		std::cin>>start>>end;
		ExtractBackground(argv[1],start,end);
		break;
	case 2:
		MobileDectect(argv[1],argv[2]);
		break;
	default:break;
	}
	
	
}
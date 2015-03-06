#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv/cv.hpp>
#include<cstring>
#include "macbsvpmss.h"

int main(int argc,char **argv)
{
	IplImage* img_avg;
	char src[205];
	strcpy(src,argv[1]);
    ExtractBackground(src,0);
//	ExtraFront();
//	ExtraContours();
}
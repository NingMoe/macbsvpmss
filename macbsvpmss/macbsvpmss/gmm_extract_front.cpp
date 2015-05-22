#include "macbsvpmss.h"
#include "opencv2/core/core.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
void GmmExtraFront(IplImage* frame,IplImage* front_bin){
	cv::BackgroundSubtractorMOG mog;
	cv::Mat mat_gray(frame);
	cv::Mat mat_front;
	cvtColor(mat_gray,mat_gray,CV_RGB2GRAY);
	mog(mat_gray,mat_front,0.1);
	IplImage tmp = mat_front;
	front_bin = cvCloneImage(&tmp);
	cv::namedWindow("front");
	cv::imshow("front",mat_front);
}

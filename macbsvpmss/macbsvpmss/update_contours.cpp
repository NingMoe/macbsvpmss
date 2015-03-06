/*更新跟踪当中的轮廓外接矩形：前后帧相同则更新跟踪帧数，
新矩形加入到集合末尾,src是新一帧的矩形集合，des是跟踪当中的矩形集合
*/
#include "macbsvpmss.h"
#include <cmath>
using namespace std;
//自定义坐标类型
struct center{
	double x;
	double y;
};
//用于返回外接矩形中心的辅助函数
center GetCenter(CvRect rect)
{
	center point;
	point.x = rect.x+rect.width/2;
	point.y = rect.y+rect.height/2;
	return point;
}
void UpdateContour(vector<tracedata> src,vector<tracedata> des)
{
	const double thresh = 1;
	bool flag ;
	for(vector<tracedata>::iterator itSrc = src.begin();itSrc != src.end();itSrc ++)
	{
		flag = false;
		for (vector<tracedata>::iterator itDes = des.begin();itDes != des.end();itDes++)
		{
			if(fabs(GetCenter(itSrc->rect).x - GetCenter(itDes->rect).x) < 1 )
			{
				if(fabs(GetCenter(itSrc->rect).y - GetCenter(itDes->rect).y) < 1)
				{
					itDes->num++;
					flag = true;
					break;
				}
			}
		}
		if(!flag)
		{
			des.push_back(*itSrc);
		}
	}
}
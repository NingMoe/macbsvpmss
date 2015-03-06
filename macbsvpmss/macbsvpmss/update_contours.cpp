/*���¸��ٵ��е�������Ӿ��Σ�ǰ��֡��ͬ����¸���֡����
�¾��μ��뵽����ĩβ,src����һ֡�ľ��μ��ϣ�des�Ǹ��ٵ��еľ��μ���
*/
#include "macbsvpmss.h"
#include <cmath>
using namespace std;
//�Զ�����������
struct center{
	double x;
	double y;
};
//���ڷ�����Ӿ������ĵĸ�������
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
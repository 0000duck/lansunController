/*
 * Element.cpp
 *
 *  Created on: 2017��5��3��
 *      Author: deng
 */

#include "Element.h"
#include "robot_lib_2/SwingWelding.h"
#include <fstream>
Element::Element()
{
	// TODO Auto-generated constructor stub
	angle = 180;
	num = 0;
	speed = 3000;
	type = MoveLine;
	startC = {0.0,0.0};
	endC = {0.0,0.0};
	midC = {0.0,0.0};
	startXyzrpw = ArrayXd::Zero(6); //���
	endXyzrpw = ArrayXd::Zero(6); ;  //ĩ��
	midXyzrpw = ArrayXd::Zero(6); ;  //ĩ��
	interpolationPoints.clear();
	interpolationPointsIndexs.clear();
    Index = 0;
    IsArcStric = false;
    IsQuench = false;
    swingpoints.clear();
   }

Element::~Element()
{
	// TODO Auto-generated destructor stub
}

//ֱ�߲岹
bool Element::getLineInterpolations()
{
	list<MJCoint> abcdef = RobotAndCHPositionAttitudeLinearInterpolation(startC,endC,xyzrpw_2_pose(startXyzrpw),xyzrpw_2_pose(endXyzrpw));
	    if(checkInterpolations(abcdef))  //У����ȷ
	    {
	    	interpolationPoints = abcdef;
	    	return true;
	    }
	    else
	    	return false;
}

//Բ���岹
bool Element::getArcInterpolations()
{
	list<MJCoint> abcdef = RobotAndCHPositionAttitude3DInterpolation(startC,midC,endC,xyzrpw_2_pose(startXyzrpw),xyzrpw_2_pose(midXyzrpw),xyzrpw_2_pose(endXyzrpw));
		if(checkInterpolations(abcdef))  //У����ȷ
		{
			interpolationPoints = abcdef;
			return true;
		}
		else
			return false;

}


//��֤�岹���Ƿ���ȷ
bool Element::checkInterpolations(list<MJCoint> abcdef)
{
	int i = 0;
	list<MJCoint>::iterator iter = abcdef.begin();
    if(!lastJIsinit)
    {
       lastJ = NewPositionJointssolution((*iter).m);
	   lastJIsinit = true;
    }
	for (; iter != abcdef.end();iter++)
	{
		(*iter).j = NewPositionJointssolution((*iter).m,lastJ);
		lastJ = (*iter).j;
		i++;
		if ((!(*iter).j.ISOK))
		{
			throw MobileTransfinite;  //�׳��ƶ����޵��쳣
			return false;
		}
		MJCPoint p;
		p.mj = *iter;
		p.index = i;
		interpolationPointsIndexs.push_back(p);
		}
		return true;
}
void Element::getMoveLineInterpolations()
{
	MJCoint startmj,endmj; //������ĩ��Ĳ岹�߶ζ�Ū��ȥ
	startmj.j = NewPositionJointssolution(xyzrpw_2_pose(startXyzrpw)); //�����һ����Ū��ȥ
	startmj.c = startC;
	endmj.j = NewPositionJointssolution(xyzrpw_2_pose(endXyzrpw)); //�����һ����Ū��ȥ
	endmj.c = endC;
	 if(!lastJIsinit)
	{
	   lastJ = endmj.j;
	   lastJIsinit = true;
	}
	MJCPoint p1,p2;
	p1.mj = startmj;
	p1.index = 1;
	p2.mj = endmj;
	p2.index = 2;
	list<MJCPoint> abcdef;
	abcdef.clear();
	abcdef.push_back(p1);       //�ѵ�һ����ӽ�ȥ
	abcdef.push_back(p2);         //�����һ����ӽ�ȥ
	interpolationPointsIndexs = abcdef;   //��ǰ��������

}

bool Element::getInterpolations()   //�õ��岹��
{
	try
    {
		switch(type)
		{
		  case MoveLine: getMoveLineInterpolations(); break;     //������
		  case FireLine: getLineInterpolations();     break;     //������
		  case Arc:      getArcInterpolations();      break;     //�õ�Բ���岹��
		  case SwingLine:getSwingLineInterpolations();break;   //�õ��ں���
		  default: break;
		}
    }
	catch(...)
	{
		cout << "�ƶ�����" << endl;                               //�ƶ�����
		DylCommon::protocol_send("E6,3");                        //��������
	    return false;
	}
	return true;
}


double Element::getTwoLinesAngle(Element &line1,Element &line2)      //�õ������ߵļн�
{
		Vector3f v1(line1.endXyzrpw[0] - line1.startXyzrpw[0],line1.endXyzrpw[1] - line1.startXyzrpw[1],line1.endXyzrpw[2] - line1.startXyzrpw[2]);
		Vector3f v2(line2.endXyzrpw[0] - line2.startXyzrpw[0],line2.endXyzrpw[1] - line2.startXyzrpw[1],line2.endXyzrpw[2] - line2.startXyzrpw[2]);
		return fabs(getTwoVector3AngleValue(v1, v2));
}

bool Element::getSwingLineInterpolations()
{

	list<MJCoint> abcdef = SwingWelding::PositionAttitudeLinearInterpolationTri(xyzrpw_2_pose(swingpoints[0]),xyzrpw_2_pose(swingpoints[1]),xyzrpw_2_pose(swingpoints[2]),xyzrpw_2_pose(swingpoints[3]),5);
	 if(checkInterpolations(abcdef))  //У����ȷ
		{
			interpolationPoints = abcdef;
			return true;
		}
		else
			return false;
}




/*
 * ArcParser.cpp
 *
 *  Created on: 2017��6��28��
 *      Author: deng
 */

#include "ArcParser.h"
#include "math.h"
#define PI 3.1415926
double ArcParser::divideLength = 4;
#include <iostream>
using namespace std;
ArcParser::ArcParser() {
	// TODO Auto-generated constructor stub

}

ArcParser::~ArcParser() {
	// TODO Auto-generated destructor stub
}

//�Ƿ����㹲��
bool ArcParser::isThreePointInLine(double x1, double y1, double z1,double x2, double y2, double z2, double x3, double y3, double z3)
{
	 //��������ֵ n = AB x AC  (���)
	      double i, j, k;
	      i = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
	      j = (z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1);
	      k = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
	      //���ж��Ƿ��ߣ����������������S = 1/2|AB x AC| ,������˵�ģ
	      return i == 0.0 && j == 0.0 && k == 0.0;
}

//�õ�Բ�ķָ��
bool ArcParser::getArcDividePoint(double x1, double y1, double z1, double r1, double p1, double w1,double x2,double y2, double z2, double x3, double y3, double z3, double r3, double p3,double w3, int num,vector<vector<double>>& pointList)
{
	           vector<double> point;
	           if (isThreePointInLine(x1, y1, z1, x2, y2, z2, x3, y3, z3))
	           {
	        	   cout << "���㹲��" << endl;
	               return false;
	           }
	    //�����߱�ʾ���Թ��������Σ���ƽ������� AB AC ����ʾ AO�� AO = aAB + bAC; AO��AB = 1/2 * AB^2, AO��AC = 1/2 * AC^2;
	           double ABAB, ABAC, ACAC, a, b;
	           ABAB = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1);
	           ABAC = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1) + (z2 - z1) * (z3 - z1);
	           ACAC = (x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1) + (z3 - z1) * (z3 - z1);
	           a = ACAC * (ABAB - ABAC) / (ACAC * ABAB - ABAC * ABAC) / 2.0;
	           b = (ABAB * ABAC - ABAB * ACAC) / (ABAC * ABAC - ACAC * ABAB) / 2.0;
	           //Բ������Ͱ뾶
	           double x0, y0, z0, r;
	           x0 = x1 + a * (x2 - x1) + b * (x3 - x1);
	           y0 = y1 + a * (y2 - y1) + b * (y3 - y1);
	           z0 = z1 + a * (z2 - z1) + b * (z3 - z1);
	           r = sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1) + (z0 - z1) * (z0 - z1));
	           //���ǵ���㡢ĩ��֮���Բ�Ľǿ��ܴ���180�ȣ��ѽǶȷֳ�������е㣬�е���ĩ���Ӧ��Բ�Ľ�֮��
	           //ʹ������������cos��0-pi���ǵ����ģ�ѡ�õ��
	           double OAOB = (x1 - x0) * (x2 - x0) + (y1 - y0) * (y2 - y0) + (z1 - z0) * (z2 - z0);
	           double OBOC = (x2 - x0) * (x3 - x0) + (y2 - y0) * (y3 - y0) + (z2 - z0) * (z3 - z0);
	           double angle1 = acos(OAOB / (r * r));
	           double angle2 = acos(OBOC / (r * r));
	           //ÿһ�ȷֶ�Ӧ��Բ�Ľ�
	           double angle = divideLength / r;
	   //        angle = 0.15707963267948966192313216916398;       //������
	           //�ȷ���
	           int divideNum = (int) ((angle1 + angle2) / angle);
	           cout << " divideNum: " << divideNum << endl;
	           if(divideNum < 1)
	           {
	               vector<double> point;
	               point.push_back(x1);
	               point.push_back(y1);
	               point.push_back(z1);
	               point.push_back(r1);
	               point.push_back(p1);
	               point.push_back(w1);
	               point.push_back(1 + num);
	               pointList.push_back(point);
	               point.clear();
	               point.push_back(x2);
	               point.push_back(y2);
	               point.push_back(z2);
	               point.push_back((r1 + r3) / 2.0);
	               point.push_back((p1 + p3) / 2.0);
	               point.push_back((w1 + p3) / 2.0);
	               point.push_back(2 + num);
	               pointList.push_back(point);
	               point.clear();
	               point.push_back(x3);
	               point.push_back(y3);
	               point.push_back(z3);
	               point.push_back(r3);
	               point.push_back(p3);
	               point.push_back(w3);
	               point.push_back(3 + num);
	               pointList.push_back(point);
	               return true;
	           }
	           //��̬�ĵȷ�
	           double rDivide = (r3 - r1) / divideNum;
	           double pDivide = (p3 - p1) / divideNum;
	           double wDivide = (w3 - w1) / divideNum;

	           //ͬ����������������ȷֵ������(x, y, z)
	           //���ж�A��O��B�Ƿ��ߣ����߾���A��O��C����⣬��ΪA��O��B��A��O��C������ͬʱ����
	           //A��O��B�����ߣ���ƽ������� OA OB ����ʾ OD�� OD = aOA + bOB; OA��OD = r*r*cos(angle), OB��OD = r*r*cos(angle1);
	           //A��O��B���ߣ���ƽ������� OA OC ����ʾ OD�� OD = aOA + bOC; OA��OD = r*r*cos(angle), OC��OD = r*r*cos(angle1 + angle2);
	           //A��O��B�����߽�� a = sin(angle)/sin(angle1), b = sin(angle1 - angle)/sin(angle1);
	           //A��O��B���߽�� a = sin(angle)/sin(angle1 + angle2), b = sin(angle1 + angle2 - angle)/sin(angle1 + angle2);
	           //�ж�A��O��B�Ƿ��ߣ�������֮ǰ���жϹ���
	           double x, y, z;     //�ȷֵ������
	           if (fabs(angle1 - PI) < 0.01 || fabs(angle1 - 0.0) < 0.01)
	           {            //����,
	               // ��һ����isThreePointInLine���жϣ��� angle1 �� pi �Ĺ�ϵҲ�����жϵ�
	               for (int i = 0; i < divideNum; i++)
	               {
	                   //�ȷֵ�����ԣ�x, y, z�͵�����, ����ÿ�ζ���ʼ�����󣬲�Ȼ֮ǰpoint��Ҳ����ű仯
	                   vector<double> point;
	                   point.clear();
	                   b = sin(angle * (i + 1)) / sin(angle1 + angle2);
	                   a = sin(angle1 + angle2 - angle * (i + 1)) / sin(angle1 + angle2);
	                   point.push_back(x0 + a * (x1 - x0) + b * (x3 - x0));        //x
	                   point.push_back(y0 + a * (y1 - y0) + b * (y3 - y0));        //y
	                   point.push_back(z0 + a * (z1 - z0) + b * (z3 - z0));        //z
	                   point.push_back(r1 + rDivide * (i + 1));                    //r
	                   point.push_back(p1 + pDivide * (i + 1));                    //p
	                   point.push_back(w1 + wDivide * (i + 1));                    //w
	                   point.push_back(i + 1 + num);              //��ţ��� 1 ��ʼ����ʱ���ȡ��ʱ��ֱ��ǿ��ת����int�Ϳ�����
	                   pointList.push_back(point);
	               }
	           }
	           else {
	               for (int i = 0; i < divideNum; i++)
	               {
	                   vector<double> point;
	                   point.clear();
	                   b = sin(angle * (i + 1)) / sin(angle1);
	                   a = sin(angle1 - angle * (i + 1)) / sin(angle1);
	                   point.push_back(x0 + a * (x1 - x0) + b * (x2 - x0));
	                   point.push_back(y0 + a * (y1 - y0) + b * (y2 - y0));
	                   point.push_back(z0 + a * (z1 - z0) + b * (z2 - z0));
	                   point.push_back( r1 + rDivide * (i + 1));                    //r
	                   point.push_back( p1 + pDivide * (i + 1));                    //p
	                   point.push_back( w1 + wDivide * (i + 1));                    //w
	                   point.push_back(i + 1 + num);
	                   pointList.push_back(point);
	               }
	           }
	           point = pointList.at(divideNum - 1);
	           if (!isEndPoint(point[0], point[1], point[2], x3, y3, z3))
	           {    //�ж��Ƿ����һ��
	               vector<double> point;
	               point.clear();
	               point.push_back(x3);        //x
	               point.push_back(y3);        //y
	               point.push_back(z3);        //z
	               point.push_back(r3);                    //r
	               point.push_back(p3);                    //p
	               point.push_back(w3);                    //w
	               point.push_back(divideNum + 1 + num);            //��ţ��� 1 ��ʼ
	               pointList.push_back(point);
	           }
	           cout << "������" << endl;
	           return true;



}

//�ж����һ���Ƿ��ǵ�����
bool ArcParser::isEndPoint(double x1, double y1, double z1, double x2, double y2, double z2)
{
    return fabs(x1 - x2) < 0.000001 && fabs(y1 - y2) < 0.000001 && fabs(z1 - z2) < 0.000001;
}
//�õ�Բ���ĵȷֵ� ͨ��������õ�     Բ�ĺ���
bool ArcParser::getCircleDividePoint(double x1, double y1, double z1, double r1 , double p1 ,double w1 , double x2,
                                    double y2, double z2, double x3, double y3, double z3, double r3,
                                    double p3,double w3, vector<vector<double>>& midPoint )
{
   vector<vector<double>> arcDividePoint;
   vector<vector<double>> newArcDivide;     //�µ�Բ�ĵȷֺ���
   newArcDivide.clear();                   //Բ������
   if(!getArcDividePoint(x1, y1, z1, r1, p1,w1, x2, y2, z2, x3, y3, z3, r3, p3,w3,0,arcDividePoint))   //
   {
	cout << "1Բ���ʧ��" << endl;
    return false;
   }
      int num = arcDividePoint.size();
          //������һ�������x3, y3, z3,������ȡ�����������㣬Ȼ���������Ϊ��㣬x3, y3, z3Ϊ�е㣬x1, y1, z1Ϊ�յ��ٴεȷ�Բ��
	  vector<double> preLast = arcDividePoint.at(num - 5);
	  //�����µ�Բ���ȷ�
	  if(!getArcDividePoint(preLast[0], preLast[1], preLast[2], preLast[3],
			  preLast[4], preLast[5],x3, y3, z3, x1, y1, z1, r1, p1,w1, num - 1,newArcDivide))
	  {
		  cout << "2Բ���ʧ��" << endl;
		  return false;
	  }
	  vector<double> pos1 = newArcDivide[newArcDivide.size() * 0.25 + 1];
	  vector<double> pos2 = newArcDivide[newArcDivide.size() * 0.5  + 1];
	  vector<double> pos3 = newArcDivide[newArcDivide.size() * 0.75 + 1];
	  midPoint.push_back(pos1);
	  midPoint.push_back(pos2);
	  midPoint.push_back(pos3);
      return true;
}
//�õ�Բ���ĵȷֵ� ͨ��������õ�     Բ�ĺ���
bool ArcParser::getCircleDivide4Point(double x1, double y1, double z1, double r1 , double p1 ,double w1 , double x2,
                                    double y2, double z2, double x3, double y3, double z3, double r3,
                                    double p3,double w3, double x4, double y4, double z4, double r4,
                                    double p4,double w4, vector<vector<double>>& midPoint )
{
   vector<vector<double>> arcDividePoint;
   vector<vector<double>> newArcDivide;     //�µ�Բ�ĵȷֺ���
   vector<vector<double>> newArc1Divide;     //�µ�Բ�ĵȷֺ���
   arcDividePoint.clear();
   newArcDivide.clear();
   newArc1Divide.clear();//Բ������
   if(!getArcDividePoint(x1, y1, z1, r1, p1,w1, x2, y2, z2, x3, y3, z3, r3, p3,w3,0,arcDividePoint))   //
   {
	cout << "1Բ���ʧ��" << endl;
    return false;
   }
     int num = arcDividePoint.size();
	  //������һ�������x3, y3, z3,������ȡ�����������㣬Ȼ���������Ϊ��㣬x3, y3, z3Ϊ�е㣬x1, y1, z1Ϊ�յ��ٴεȷ�Բ��
	  vector<double> preLast = arcDividePoint.at(num - 3);

	  if(!getArcDividePoint(preLast[0], preLast[1], preLast[2], preLast[3],
				  preLast[4], preLast[5],x3, y3, z3, x1, y1, z1, r1, p1,w1, num - 1,newArcDivide))
	  {
		  cout << "����ʧ��" << endl;
		  return false;
	  }
	  //�����µ�Բ���ȷ�
	  if(!getArcDividePoint(preLast[0], preLast[1], preLast[2], preLast[3],
			  preLast[4], preLast[5],x3, y3, z3, x4, y4, z4, r4, p4,w4, num - 1,newArcDivide))
	  {
		  cout << "2Բ���ʧ��" << endl;
		  return false;
	  }
	  vector<double> pos = newArcDivide[newArcDivide.size() * 0.5  + 1];
	  midPoint.push_back(pos);

	  num = newArcDivide.size();
	  //������һ�������x3, y3, z3,������ȡ�����������㣬Ȼ���������Ϊ��㣬x3, y3, z3Ϊ�е㣬x1, y1, z1Ϊ�յ��ٴεȷ�Բ��
	   preLast = newArcDivide.at(num - 3);
	  //�����µ�Բ���ȷ�
	  if(!getArcDividePoint(preLast[0], preLast[1], preLast[2], preLast[3],
			  preLast[4], preLast[5],x4, y4, z4, x1, y1, z1, r1, p1,w1, num - 1,newArc1Divide))
	  {
		  cout << "3Բ���ʧ��" << endl;
		  return false;
	  }
	  pos.clear();
	  pos = newArc1Divide[newArc1Divide.size() * 0.5  + 1];
	  midPoint.push_back(pos);
	  return true;
}


//�õ�Բ���ĵȷֵ� ͨ��������õ�
bool ArcParser::getCircleDividePoint1(double x1, double y1, double z1, double r1, double p1,double w1, double x2,
                                    double y2, double z2, double x3, double y3, double z3, double r3,
                                    double p3,double w3,vector<vector<double>>& arcDividePoint )
{

   vector<vector<double>>newArcDivide;     //�µ�Բ�ĵȷֺ���
   newArcDivide.clear();                   //Բ������
   if(!getArcDividePoint(x1, y1, z1, r1, p1,w1, x2, y2, z2, x3, y3, z3, r3, p3,w3,0,arcDividePoint))   //
    return false;
   int num = arcDividePoint.size();
          //������һ�������x3, y3, z3,������ȡ�����������㣬Ȼ���������Ϊ��㣬x3, y3, z3Ϊ�е㣬x1, y1, z1Ϊ�յ��ٴεȷ�Բ��
          vector<double> preLast = arcDividePoint.at(num - 2);
          //�����µ�Բ���ȷ�
          if(!getArcDividePoint(preLast[0], preLast[1], preLast[2], preLast[3],
                  preLast[4], preLast[5],x3, y3, z3, x1, y1, z1, r1, p1,w1, num - 1,newArcDivide))
              return false;
          arcDividePoint.pop_back();
//          arcDividePoint.removeAt(num - 1);        //ԭ����ȥ��ĩβ
//          arcDividePoint.removeAt(num - 2);
//          arcDividePoint.push_back(newArcDivide);
          for(int i = 0; i < (int)newArcDivide.size();i++)
        	  arcDividePoint.push_back(newArcDivide[i]);
//          arcDividePoint += newArcDivide;
//        arcDividePoint.addAll(newArcDivide);    //�����µ�
          return true;


}


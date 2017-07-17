/*
 * Welding.cpp
 *
 *  Created on: 2016��11��21��
 *      Author: Administrator
 */
#include <sys/time.h>
#include "hps_0.h"
#include "DlyCommon.h"
#include "Welding.h"
#define  ARCSTRICK   0xfffffff7       //��
#define  ARCQUENCH   0xffffffff       //Ϩ��
Welding::Welding() :runing(false), rate(1), IsFireMode(false)
{
	init();
	lastSpeed = 0;
}
Welding::~Welding()
{

}
//���к�����ʼ�˶�����
void Welding::move()
{
	if (!runing)  //û���յ��������е�ָ��
		return;
	double minSpeed = 200;          //��С�ٶ�
	double currentSpeed = minSpeed; //��ǰ�ٶ�
	double speed;                   //Ŀǰ�ٶ�
	bool arcStrickStatic = false;   //Ŀǰ����״̬���Ƿ����˻�
	//��ʼ�˶�
	if (moveFinished)
	{
		moveFinished = false;
		interpolationIndex = 0;
		graphInex = 0;
	}
	if (backFinished)
	{
		backFinished = false;
		interpolationIndex = 0;
		graphInex = 0;
	}
	for (int i = graphInex; i < (int) graph.size(); i++) //��������ͼ������
	{
		double acctimes = 200;          //���ٴ���
		double dectimes = 100;	        //���ٴ���
		speed = graph[i].speed;   //��ȡ��ǰ�ٶ�
//		acctimes = dectimes = 50 * (speed / 1000.0) * rate;   //��ȡ�Ӽ��ٲ���
		if (acctimes < 1)
			acctimes = dectimes = 1;
		//������

		list<MJCPoint> & abcdef = graph[i].interpolationPointsIndexs;    //������е�
		long int linesize = abcdef.size();        //ÿ��0.1ѭ���Ĵ���
		list<MJCPoint>::iterator iter = abcdef.begin();
		if (graph[i].Index - 1 == graphInex && graph[i].type != MoveLine) //�������ϵ�ȥ
		{
			for (int m = 0; m < interpolationIndex; m++)
				iter++;
			linesize -= interpolationIndex;
		}
		double anglerate = 1;
		anglerate = graph[i].angle / 180.0;
		if (linesize < (acctimes + dectimes))
		{
			acctimes = dectimes = linesize / 2.0;
		}
		if (acctimes < 1)
			acctimes = 1;
		if (dectimes < 1)
			dectimes = 1;
		double speedrate = 0;    //�ٶȱ���
		//�սǼӼ��ٿ���
		speedrate = 1 - anglerate;
		//ĳһ���߶����
		long int runtaskNum = 0;   //�ڲ岹�����������
		if(IsFireMode && graph[i].type != MoveLine && !arcStrickStatic)
		{
			if(IsFireLinecPause || IsModeChangeArcStrick)
			{
				arcStrick(lastArcStrickObject.num, lastArcStrickObject.arcStricDelay);                //��ʼ��
				arcStrickStatic = true;  //���º���״̬
				if(IsFireLinecPause)
				IsFireLinecPause = false;
				if(IsModeChangeArcStrick)
			    IsModeChangeArcStrick = false;
			}
		}
		char str[100];
		sprintf(str, "DE,4,%d", graph[i].num);
		cout << "*********" << graph[i].Index << "���˶�*********" << endl;
		DylCommon::protocol_send(str);
		for (; iter != abcdef.end(); iter++)
		{
			//�����ǵ�һ��
			if (graph[i].type == MoveLine)   //����ǿ�����
			{
				iter = abcdef.end();
				iter--;
				cout << graph[i].num << "�߶ο����ƶ�" << endl;
				moto_runJAbs((iter->mj).j, (iter->mj).c, 5000);
				if (Variable::IsStop) //��ͣ
				{

					Variable::IsStop = false;
					interpolationIndex = 0;
					graphInex = graph[i].Index - 1;        //�ߵ���ű�������
					runing = false;
					cout << "graphInex: " << graphInex << endl;
					return;
				}
				cout << graph[i].num << "�߶ο����ƶ���� " << endl;
				break;
			}
			if (Variable::IsStop)   //��ͣ
			{
				if(arcStrickStatic)
					IsFireLinecPause = true;
				IOM->DATA = ARCQUENCH;       //Ϩ��
				interpolationIndex = iter->index - 1;  //�ѵ�ǰ�Ĳ岹����������
				graphInex = graph[i].Index - 1;        //�ߵ���ű�������
				Variable::IsStop = false;
				runing = false;
				cout << "graphInex: " << graphInex << "  interpolationIndex: "
						<< interpolationIndex << endl;
				return;
			}
//            if(currentSpeed != lastSpeed)
//            {
//            	cout << "speed: " << currentSpeed << endl;
//            	lastSpeed = currentSpeed;
//            }
			moto_runInterpolationAbs((iter->mj).j, (iter->mj).c, currentSpeed);
			runtaskNum++;
			//���ٹ���
			double speed1 = speed * rate; //�ٶ���ת��ť���ı��ٶȵĴ�С�õ���һ����ʼ�ٶ�
			//��ǰ�ٶ�С�ڳ�ʼ�趨�ٶ�ʱ����δ�ڼ��ٷ�Χ�ڻ�һֱ���٣�ֱ������Ϊֹ
			if ((currentSpeed < speed1) && (linesize - runtaskNum) > dectimes)
			{//���ַ�ʽ�ļӼ��ٶ�Ҫ����
				currentSpeed += ((speed1 - minSpeed) / acctimes);
				if (currentSpeed > speed1)
					currentSpeed = speed1;
			}
			//�ٶ��ر�С���Ͳ����Ӽ��ٿ�����
			if (speed1 <= minSpeed)
			{   //�����Ӽ��ٿ���
				if (speed1 < 1)
					speed1 = 1;
				currentSpeed = speed1;
			}
			//���ٹ���(���������Ҫ���ٿ���)
			//�ٶȵ��ڰ�ť�õ�ǰ�ٶȴ����趨�ٶ�
			// ������ж�Ӧ�ò���Ҫ��
			if ((currentSpeed > speed1) && (speed1 > minSpeed))
			{
				currentSpeed -= (speed1 / acctimes);
				if (currentSpeed <= speed1)
					currentSpeed = speed1;
			}
			//������߶ο������ĩ�˽��м��ٿ���
			if (((linesize - runtaskNum) <= dectimes) && (speed1 > minSpeed))
			{

				currentSpeed -= (speed1 - minSpeed) / dectimes;
				if (currentSpeed <= speedrate * speed1)
					currentSpeed = speedrate * speed1;
				if (currentSpeed < minSpeed)
					currentSpeed = minSpeed;
			}
		}
		//1.���ں���ģʽ 2.û���𻡵�״̬ 3.�ǲ岹��
		if (!arcStrickStatic)
		{
			if(IsFireMode)
			{
				if (graph[i].IsArcStric)
				{
					arcStrickObject obj = graph[i].arcStrickObj;          //�������Ϣ
					lastArcStrickObject = obj;
					arcStrick(obj.num, obj.arcStricDelay);                //��ʼ��
					arcStrickStatic = true;                              //���º���״̬
				}

			}
			else
			{
				if (graph[i].IsArcStric)
				{
					arcStrickObject obj = graph[i].arcStrickObj;   //�������Ϣ
					lastArcStrickObject = obj;
					IsModeChangeArcStrick = true;
				}

		    }

		}

		//Ϩ������
		//1.����ģʽ 2.����״̬�� 3.��ҪϨ��
		if (graph[i].IsQuench)
		{
			if(IsFireMode)
			{
			   if(arcStrickStatic)
			   {
				arcQuenchObject obj = graph[i].arcQuenchObj;   //��ȡϨ����Ϣ
				arcQuench(obj.num);                                  //��ʼϨ��
				arcStrickStatic = false;                             //���+1
			   }
			}
			else
			{
				IsModeChangeArcStrick = false;
			}
		}

	}
	IOM->DATA = ARCQUENCH;       //Ϩ��;
	if (runing)
	{
		DylCommon::protocol_send("DC,3");
		moveFinished = true;
		cout << "�������" << endl;

	}
	runing = false;
}

//��ʼ�𻡣�������Ҫ��һ��
bool Welding::arcStrick(int currentLineNum, double arcStrictime)
{
	cout << "�𻡺�: " << currentLineNum << "  ��ʱ�䣺" << arcStrictime << endl;
	char str[100];
	sprintf(str, "DE,4,%d", currentLineNum);
	DylCommon::protocol_send(str);
	sleep(arcStrictime);
	IOM->DATA = ARCSTRICK;
	return true;
}

//Ϩ��ָ��
void Welding::arcQuench(int currentLineNum)
{
	cout << "Ϩ���� " << currentLineNum << endl;
	char str[100];
	sprintf(str, "DE,4,%d", currentLineNum);
	DylCommon::protocol_send(str);
	IOM->DATA = ARCQUENCH;       //Ϩ��
}

void Welding::stopStaticChange()
{
	runing = false;
	IOM->DATA = ARCQUENCH;         //Ϩ��
	cout << "������ͣ" << endl;
	Variable::IsStop = false;
}

/**********************************�ؽ�ʽ������************************************/

//���յ�ֱ�ߵ�����,���ʱ��Ҫ���� ֻ�Ǳ����´��ĵ�λ����
void Welding::receiveLinePoints(const ArrayXd &p1, const Coint& c1,const ArrayXd &p2, const Coint& c2, double speed, int num,bool IsFire)
{
	Element element;   //��һ��Ԫ�ص�  ���յ�һ���߾ͷ���Բ������
	element.startXyzrpw = p1;  //���
	element.startC = c1;
	element.endXyzrpw = p2;
	element.endC = c2;
	element.speed = speed;
	element.num = num;
	element.Index = graph.size() + 1;
	if (IsFire)
	{
		//��һ�������и��� ��һ��Ҳ���и�����Ͱ���һ���ߵĽǶȸı� ʹ����ٲ���Ҫ��̫��
		if(graph.size() > 0 && graph[graph.size() - 1].type == FireLine)
            graph[graph.size() - 1].angle = Element::getTwoLinesAngle(graph[graph.size() - 1],element);
		element.type = FireLine;   //�и���
	}
	else
		element.type = MoveLine;   //������
	graph.push_back(element);      //�����ȥ���ȥ

}

//Բ����������
void Welding::receiveArcPoints(const ArrayXd &p1, const Coint& c1,const ArrayXd &p, const Coint& c, const ArrayXd &p2, const Coint& c2,double speed, int num)
{
	Element element;                 //Ԫ���߶�
	element.startXyzrpw = p1;        //���xyzrpw
	element.startC = c1;             //��ʼ��
	element.endXyzrpw = p2;          //ĩ��
	element.endC = c2;               //ĩ���λ����
	element.midXyzrpw = p;           //�е����
	element.midC = c;                //�е�
	element.speed = speed;           //�ٶ�
	element.num = num;               //���
	element.type = Arc;              //��
	element.Index = graph.size() + 1;
	graph.push_back(element);        //������ȥ
}
//Բ����������
void Welding::receiveCircle1Points(const ArrayXd &p1, const Coint& c1,const ArrayXd &p, const Coint& c, const ArrayXd &p2, const Coint& c2,double speed, int num)
{
	vector<vector<double>>  Points;
	double x1 = p1[0];
	double y1 = p1[1];
	double z1 = p1[2];
	double rr1 = p1[3];
	double pp1 = p1[4];
	double ww1 = p1[5];
	double x2 = p[0];
	double y2 = p[1];
	double z2 = p[2];
	double x3 = p2[0];
	double y3 = p2[1];
	double z3 = p2[2];
	double rr3 = p2[3];
	double pp3 = p2[4];
	double ww3 = p2[5];
	ArcParser::getCircleDividePoint1(x1,y1,z1,rr1,pp1,ww1,x2,y2,z2,x3,y3,z3,rr3,pp3,ww3,Points);
//	ArrayXd point1 = p;
//	ArrayXd point2 = p;
	for(int i = 1; i < (int)Points.size(); i++){
		Element element;
		for(int j = 0; j < 6;j++)
		{
			element.startXyzrpw[j] = Points[i - 1][j];
			element.endXyzrpw[j] = Points[i][j];
	   }
		element.speed = speed;           //�ٶ�
		element.num = num;               //���
		element.type = FireLine;              //��
		element.Index = graph.size() + 1;
		graph.push_back(element);        //������ȥ
     }
}
//Բ��������
void Welding::receiveCirclePoints(const ArrayXd &p1, const Coint& c1,const ArrayXd &p, const Coint& c, const ArrayXd &p2, const Coint& c2,double speed, int num)
{
	Element element;                 //Ԫ���߶�
	element.startXyzrpw = p1;        //���xyzrpw
	element.startC = c1;             //��ʼ��
	element.endXyzrpw = p2;          //ĩ��
	element.endC = c2;               //ĩ���λ����
	element.midXyzrpw = p;           //�е����
	element.midC = c;                //�е�
	element.speed = speed;           //�ٶ�
	element.num = num;               //���
	element.type = Arc;              //��
	element.Index = graph.size() + 1;
	graph.push_back(element);        //������ȥ
	vector<vector<double>> midPoint;
	double startPos[6];
	double midPos[6];
	double endPos[6];

	for(int i = 0; i < 6; i++)
	{
		startPos[i] = element.startXyzrpw[i];
		midPos[i] = element.midXyzrpw[i];
		endPos[i] = element.endXyzrpw[i];
	}
	ArcParser::getCircleDividePoint(startPos[0],startPos[1],startPos[2],startPos[3],startPos[4],startPos[5],
			midPos[0],midPos[1],midPos[2],endPos[0],endPos[1],endPos[2],endPos[3],endPos[4],endPos[5],midPoint);
	ArrayXd xyzrpwp1 = p;
	ArrayXd xyzrpwp2 = p;
	ArrayXd xyzrpwp3 = p;
	for(int i = 0; i < 6; i++)
	{
	    xyzrpwp1[i] = midPoint[0][i];
	    xyzrpwp2[i] = midPoint[1][i];
	    xyzrpwp3[i] = midPoint[2][i];
	}
	Element element1; //Ԫ���߶�
	element1.startXyzrpw = p2;        //���xyzrpw
	element1.startC = c2;             //��ʼ��
	element1.midXyzrpw = xyzrpwp1;      //�е����
	element1.midC = c;                //�е�
	element1.endXyzrpw = xyzrpwp2;
	element1.endC = c1;               //ĩ���λ����
	element1.speed = speed;           //�ٶ�
	element1.num = num;               //���
	element1.type = Arc;              //��
	element1.Index = graph.size() + 1; //��ź���Ų�һ��
	graph.push_back(element1);        //������ȥ
	Element element2; //Ԫ���߶�
	element2.startXyzrpw = xyzrpwp2;        //���xyzrpw
	element2.startC = c2;             //��ʼ��
	element2.midXyzrpw = xyzrpwp3;      //�е����
	element2.midC = c;                //�е�
	element2.endXyzrpw = p1;
	element2.endC = c1;               //ĩ���λ����
	element2.speed = speed;           //�ٶ�
	element2.num = num;               //���
	element2.type = Arc;              //��
	element2.Index = graph.size() + 1; //��ź���Ų�һ��
	graph.push_back(element2);        //������ȥ
}
//Բ��������
void Welding::receiveCircle4Points(const ArrayXd &p1, const Coint& c1,const ArrayXd &p, const Coint& c, const ArrayXd &p2, const Coint& c2,const ArrayXd &p3, const Coint& c3,double speed, int num)
{
	Element element;                 //Ԫ���߶�
	element.startXyzrpw = p1;        //���xyzrpw
	element.startC = c1;             //��ʼ��
	element.endXyzrpw = p2;          //ĩ��
	element.endC = c2;               //ĩ���λ����
	element.midXyzrpw = p;           //�е����
	element.midC = c;                //�е�
	element.speed = speed;           //�ٶ�
	element.num = num;               //���
	element.type = Arc;              //��
	element.Index = graph.size() + 1;
	graph.push_back(element);        //������ȥ
	vector<vector<double>> midPoint;
	double startPos[6];
	double midPos[6];
	double endPos[6];
    double otherPos[6];
	for(int i = 0; i < 6; i++)
	{
		startPos[i] = element.startXyzrpw[i];
		midPos[i] = element.midXyzrpw[i];
		endPos[i] = element.endXyzrpw[i];
		otherPos[i] = p3[i];
	}
	ArcParser::getCircleDivide4Point(startPos[0],startPos[1],startPos[2],startPos[3],startPos[4],startPos[5],
			midPos[0],midPos[1],midPos[2],endPos[0],endPos[1],endPos[2],endPos[3],endPos[4],endPos[5],otherPos[0],otherPos[1],otherPos[2],otherPos[3],otherPos[4],otherPos[5],midPoint);
	ArrayXd xyzrpwp1 = p;
	ArrayXd xyzrpwp2 = p;
	for(int i = 0; i < 6; i++)
	{
	    xyzrpwp1[i] = midPoint[0][i];  //0.5d�ĵ�
	    xyzrpwp2[i] = midPoint[1][i];  //0.5d�ĵ�
	}
	Element element1; //Ԫ���߶�
	element1.startXyzrpw = p2;        //���xyzrpw
	element1.startC = c2;             //��ʼ��
	element1.midXyzrpw = xyzrpwp1;      //�е����
	element1.midC = c;                //�е�
	element1.endXyzrpw = p3;
	element1.endC = c3;               //ĩ���λ����
	element1.speed = speed;           //�ٶ�
	element1.num = num;               //���
	element1.type = Arc;              //��
	element1.Index = graph.size() + 1; //��ź���Ų�һ��
	graph.push_back(element1);        //������ȥ

	Element element2; //Ԫ���߶�
	element2.startXyzrpw = p3;        //���xyzrpw
	element2.startC = c3;             //��ʼ��
	element2.midXyzrpw = xyzrpwp2;      //�е����
	element2.midC = c;                //�е�
	element2.endXyzrpw = p1;
	element2.endC = c1;               //ĩ���λ����
	element2.speed = speed;           //�ٶ�
	element2.num = num;               //���
	element2.type = Arc;              //��
	element2.Index = graph.size() + 1; //��ź���Ų�һ��
	graph.push_back(element2);        //������ȥ
}


//�õ�ÿ��Element�Ĳ岹��
bool Welding::getInterpolations()     //�õ��岹��ʧ�ܾͽ����ж�
{
	int lastPercent = 0;
	for (int i = 0; i < (int) graph.size(); i++)
	{
		if (!(graph[i].getInterpolations()))
		{
			IsParserSuccess = false;   //����ʧ��
			return false;
		}
		if(i > 0)
		{
			if(((graph[i].type != MoveLine && graph[i-1].type == Arc)  || (graph[i].type == Arc && graph[i-1].type == FireLine))&& graph[i].interpolationPoints.size() > 5 && graph[i-1].interpolationPoints.size() > 5)
			{
				list<MJCoint>::iterator it1 = graph[i - 1].interpolationPoints.end();
				it1--;  //ĩ��

				ArrayXd xyzrpw12 = pose_2_xyzrpw(it1->m);
				for(int i = 0 ; i < 3; i++)
				    it1--;
			    ArrayXd xyzrpw11 = pose_2_xyzrpw(it1->m);     //���
				Vector3f v1(xyzrpw12[0] - xyzrpw11[0],xyzrpw12[1] - xyzrpw11[1],xyzrpw12[2] - xyzrpw11[2]);
				list<MJCoint>::iterator it2 = graph[i].interpolationPoints.begin();
				ArrayXd xyzrpw21 = pose_2_xyzrpw(it2->m);  //���
				for(int i = 0 ; i < 3; i++)
						it2++;                             //ĩ��
				ArrayXd xyzrpw22 = pose_2_xyzrpw(it2->m);
				Vector3f v2(xyzrpw22[0] - xyzrpw21[0],xyzrpw22[1]- xyzrpw21[1],xyzrpw22[2]- xyzrpw21[2]);
				graph[i - 1].angle = getTwoVector3AngleValue(v1,v2);
//				cout << "  �Ƕ�  " << graph[i -1].angle << endl;
			}
		}
		//progress
		char progress[400];
		int pro = (i + 1.0) / graph.size() * 100;
		if(lastPercent != pro)
		{
		sprintf(progress,"DP,4,%d",pro);
		string str(progress);
		DylCommon::protocol_send(str);
		lastPercent = pro;
		}
	}
	IsParserSuccess = true;          //�����ɹ�
	return true;
}

void Welding::receiveArcStrickData(double delay, int num)  //����������
{
	arcStrickObject obj;                       //����
	obj.arcStricDelay = delay;                 //����ʱ
	obj.num = num;                             //�𻡺�
	graph[graph.size() - 1].IsArcStric = true;
	graph[graph.size() - 1].arcStrickObj = obj;
}
void Welding::receiveArcQuenchData(int num)    //����Ϩ������
{
	arcQuenchObject obj;                      //Ϩ����
	obj.num = num;                            //Ϩ�����
	graph[graph.size() - 1].IsQuench = true;
	graph[graph.size() - 1].arcQuenchObj = obj;

}

void Welding::init()                          //��ʼ������
{
	graph.clear();
	graphInex = 0;
	interpolationIndex = 0;
	IsParserSuccess = true; //Ĭ��Ϊtrue
	moveFinished = false;
	backFinished = false;
	IsFireLinecPause = false;
	IsModeChangeArcStrick = false;
	lastJIsinit = false;
}

void Welding::back()
{
	if (!backruning)  //û���յ����Ӻ��˵�ָ��
		return;
	double minSpeed = 100;          //��С�ٶ�
	double currentSpeed = minSpeed; //��ǰ�ٶ�
	double speed;                   //Ŀǰ�ٶ�
	//��ʼ�˶�
	if (moveFinished)
	{
		moveFinished = false;
		interpolationIndex =  graph[graph.size() - 1].interpolationPointsIndexs.size() - 1;
		graphInex = graph.size() - 1;
	}
	if (backFinished)
	{
		backFinished = false;
		interpolationIndex = 0;
		graphInex = 0;
	}
	for (int i = graphInex; i >= 0; i--) //�������
	{
		double acctimes = 200;          //���ٴ���
		double dectimes = 100;	        //���ٴ���
		speed = graph[i].speed;   //��ȡ��ǰ�ٶ�
//	    acctimes = dectimes = 50 * (speed / 1000.0) * rate;   //��ȡ�Ӽ��ٲ���
		if (acctimes < 1)
		    acctimes = dectimes = 1;
		list<MJCPoint> & abcdef = graph[i].interpolationPointsIndexs;   //������е�
		long int linesize = abcdef.size();                       //ÿ��0.1ѭ���Ĵ���
		list<MJCPoint>::iterator iter = abcdef.end();             //ĩ��
		iter--; //�����һ����ȥ
		if (graph[i].Index - 1 == graphInex && graph[i].type != MoveLine) //�������ϵ�ȥ
		{
			for (int m = linesize; m >= interpolationIndex; m--)
				iter--;
			linesize = interpolationIndex;
		}
		double anglerate = 1;
		if(i > 0)
		anglerate = graph[i -1].angle / 180.0;
		//��ʾ���߶����ܶ�ʱ��û�����ٹ��̣�ֱ�Ӽ��ٺ���ٹ�������
		if (linesize < (acctimes + dectimes))
		{
			acctimes = (acctimes + dectimes) / 2.0;
		}
		if (acctimes < 1)
			acctimes = 1;
		if (dectimes < 1)
			dectimes = 1;
		double speedrate = 0;    //�ٶȱ���
		//�սǼӼ��ٿ���
		speedrate = 1 - anglerate;
		//ĳһ���߶����
		char str[100];
		sprintf(str, "DE,4,%d", graph[i].num);
		DylCommon::protocol_send(str);
		cout << "*********" << graph[i].Index << "�������˶�************" << endl;
		long int runtaskNum = 0;   //�ڲ岹�����������
		for (;; iter--)
		{
			//�����ǵ�һ��
			if (graph[i].type == MoveLine)   //����ǿ�����
			{
				cout << graph[i].num << "�߶���������ƶ�" << endl;
				iter = abcdef.begin();
				moto_runJAbs((iter->mj).j, (iter->mj).c, 5000);  //�����Ϊ׼
				if (Variable::IsStop) //��ͣ
				{
					IOM->DATA = ARCQUENCH;       //Ϩ��
					Variable::IsStop = false;
					interpolationIndex = 0;
					graphInex = graph[i].Index - 1;
					backruning = false;
					return;
				}
				cout << graph[i].num << "�߶���������ƶ����" << endl;
				break;
			}
			if (Variable::IsStop)   //��ͣ
			{
				IOM->DATA = ARCQUENCH;       //Ϩ��
				interpolationIndex = iter->index - 1;  //�ѵ�ǰ�Ĳ岹����������
				graphInex = graph[i].Index - 1;        //�ߵ���ű�������
				Variable::IsStop = false;
				backruning = false;
				IsFireLinecPause = true;
				return;
			}
			moto_runInterpolationAbs((iter->mj).j, (iter->mj).c, currentSpeed);
			runtaskNum++;
			//���ٹ���
			double speed1 = speed * rate; //�ٶ���ת��ť���ı��ٶȵĴ�С�õ���һ����ʼ�ٶ�
			//��ǰ�ٶ�С�ڳ�ʼ�趨�ٶ�ʱ����δ�ڼ��ٷ�Χ�ڻ�һֱ���٣�ֱ������Ϊֹ
			if ((currentSpeed < speed1) && (linesize - runtaskNum) > dectimes) {//���ַ�ʽ�ļӼ��ٶ�Ҫ����
				currentSpeed += ((speed1 - minSpeed) / acctimes);
				if (currentSpeed > speed1)
					currentSpeed = speed1;
			}
			//�ٶ��ر�С���Ͳ����Ӽ��ٿ�����
			if (speed1 <= minSpeed)
			{   //�����Ӽ��ٿ���
				if (speed1 < 1)
					speed1 = 1;
				currentSpeed = speed1;
			}
			//���ٹ���(���������Ҫ���ٿ���)
			//�ٶȵ��ڰ�ť�õ�ǰ�ٶȴ����趨�ٶ�
			// ������ж�Ӧ�ò���Ҫ��
			if ((currentSpeed > speed1) && (speed1 > minSpeed))
			{
				currentSpeed -= (speed1 / acctimes);
				if (currentSpeed <= speed1)
					currentSpeed = speed1;
			}
			//������߶ο������ĩ�˽��м��ٿ���
			if (((linesize - runtaskNum) <= dectimes) && (speed1 > minSpeed))
			{

				currentSpeed -= (speed1 - minSpeed) / dectimes;
				if (currentSpeed <= speedrate * speed1)
					currentSpeed = speedrate * speed1;
				if (currentSpeed < minSpeed)
					currentSpeed = minSpeed;
			}
			if (iter == abcdef.begin())
				break;
		}
		if (i > 0 && graph[i - 1].IsArcStric)
		{
			arcStrickObject obj = graph[i].arcStrickObj;   //�������Ϣ
			lastArcStrickObject = obj;
			IsModeChangeArcStrick = true;
		}
		if (i > 0 && graph[i - 1].IsQuench)
		{

			IsModeChangeArcStrick = true;
		}
}
	if (backruning)
	{
		DylCommon::protocol_send("DC,3");
		backFinished = true;
		cout << "�������" << endl;
	}
	backruning = false;
}
void Welding::run()
{
	if (runing)
		move();
	if (backruning)
		back();
}

/*
 * Element.h
 *
 *  Created on: 2017��5��3��
 *      Author: deng
 */

#ifndef ROBOT_CLASS_ELEMENT_H_
#define ROBOT_CLASS_ELEMENT_H_
#include "moto.h"
#include <vector>
using namespace std;
const int MobileTransfinite = -1;
enum ElementType
{
	MoveLine = 0,  //������
	FireLine ,  //������
	Arc,        //Բ��
   	SwingLine,  //��ֱ��
	SwingArc //��Բ��
};
struct MJCPoint
{
	MJCoint mj;
	long int index;
};

struct arcStrickObject     //����
{

	double arcStricDelay;   //����ʱ��ʱ����Ҫ��ʱ
	int num; //�����
};
struct arcQuenchObject    //Ϩ����
{
	int num;       //�����
};
class Element
{
public:
	Element();
	virtual ~Element();
	double angle; //�Ƕ�
	double drate;
	int num;      //���
	double speed; //�ٶ�
	ElementType type;   //�ߵ�����
	ArrayXd startXyzrpw; //���
	ArrayXd endXyzrpw;   //ĩ��
	ArrayXd midXyzrpw;   //ĩ��
	Coint startC;        //���ı�λ����
	Coint endC;          //�յ�ı�λ����
	Coint midC;          //�е�ı�λ����
	list<MJCoint> interpolationPoints;  //��ǰ�Ĳ岹��
	list<MJCPoint> interpolationPointsIndexs;
	arcStrickObject arcStrickObj;   //����
	arcQuenchObject arcQuenchObj;   //Ϩ����
	bool checkInterpolations(list<MJCoint> abcdef);    //У��岹���Ƿ���ȷ
	void getMoveLineInterpolations();   //�õ������ߵĲ岹�� ��ʵ����ĩ�� Ϊ��ͳһ
	bool getLineInterpolations();       //�õ�ֱ�ߵĲ岹��
	bool getArcInterpolations();        //�õ�Բ���Ĳ岹��
	bool getInterpolations();             //�õ��岹��
	bool getSwingLineInterpolations();   //�õ��ں��岹��
	double getSpeedrate();
	static double getTwoLinesAngle(Element &line1,Element &line2);   //�õ�������֮��ļн�
	int Index; //�߶ε�����ֵ  ���������
	bool IsArcStric;
	bool IsQuench;
	vector<ArrayXd> swingpoints;
};

#endif /* ROBOT_CLASS_ELEMENT_H_ */

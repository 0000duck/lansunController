/*
 * FixedLengthMoving.h
 *
 *  Created on: 2016��11��15��
 *      Author: Administrator
 */
#include <iostream>
using namespace std;
#ifndef SRC_FIXEDLENGTHMOVING_H_
#define SRC_FIXEDLENGTHMOVING_H_
#include "moto.h"
#include "common.h"
//x y ��� С�� ������ ��ת�� ���� ��λ����1 ��λ����2 ����ת �����
enum AxisType
{    XAxis,YAxis,ZAxis,BigArm,SmallArm,UpDownAxis,RotorAxis,SwingAxis,ModifiedGear1,ModifiedGear2,FixedPointRotation,
	 FixedPointSwing,ModifiedGear1Joy,ModifiedGear2Joy,UnKnown
};
//�����ƶ�
class FixedLengthMoving
{
public:
	FixedLengthMoving();
	virtual ~FixedLengthMoving();

private:
	AxisType axistype;      //���
	double speed;           //�ƶ��ٶ�
	double distance;        //�ƶ�����
	bool runing;           //����״̬
	Joint j;    //
	Coint c;
	void SingleAxismove();
public:
	void setData(AxisType,double speed,double distance);
    bool  run();


};

#endif /* SRC_FIXEDLENGTHMOVING_H_ */

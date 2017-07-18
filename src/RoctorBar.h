/*
 * RoctorBar.h
 *
 *  Created on: 2016��11��16��
 *      Author: Administrator
 */
#include "common.h"
#include "moto.h"
#include "FixedLengthMoving.h"
#include "DlyCommon.h"

#ifndef SRC_ROCTORBAR_H_
#define SRC_ROCTORBAR_H_

class RoctorBar
{
public:
	RoctorBar();
	virtual ~RoctorBar();
private:
	TestOutputFile file;
	AxisType axistype; //�������
	double speed;
	bool runing;  //�˶���־λ
	bool Joyruning;//ҡ���˶���־λ
	Joint targetJ;
	Coint targetC;
	int joyruntime;//ҡ����һС������ʱ�䣬��λ΢��
	double rate;
	double lastspeed;
    void RoctorMove();
	void SingleAxisMove();//�����ƶ�
	Vector3d lastV;
    Vector3d lastA;
    double XAxisSpeed;   //X���ٶ�
    double YAxisSpeed;   //Y���ٶ�
    double ZAxisSpeed;   //�������ٶ�
    double ModifiedGear1JoySpeed;    //��λ����1�ٶ�
    double ModifiedGear2JoySpeed;    //��λ����2�ٶ�
    double FixedPointRotationSpeed;     //����ת�ٶ�
    double FixedPointSwingSpeed;        //������ٶ�
    void clearSpeed();
public:
	void RoctorRun();
	void setData(AxisType axistype,double speed);    //��ȡ����
	bool IsSpeedUpDown(const Vector3d & nowV,const Vector3d & nowA);

     //ң���ƶ�
    void run();
};

#endif /* SRC_ROCTORBAR_H_ */

/*
 * RoctorBar.cpp
 *
 *  Created on: 2016��11��16��
 *      Author: Administrator
 */

#include "RoctorBar.h"
#include "Parameter.h"
RoctorBar::RoctorBar() :
		axistype(UnKnown), speed(0), runing(false), Joyruning(false), joyruntime(
				0), lastspeed(0), rate(1) {
	targetJ = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	targetC = {0,0};
	lastV = {0,0,0};
	lastA = {0,0,0};
	IsFixedPoint = false;

	x = 0;
	y = 0;
	z = 0;
}

RoctorBar::~RoctorBar() {

}
void RoctorBar::clearSpeed() {

	XAxisSpeed = 0;
	YAxisSpeed = 0;
	ZAxisSpeed = 0;
	FixedPointRotationSpeed = 0;
	FixedPointSwingSpeed = 0;
	ModifiedGear1JoySpeed = 0;
	ModifiedGear2JoySpeed = 0;
	this->speed = 0;

}
void RoctorBar::setData(AxisType axistype, double speed) {
	clearSpeed();
	this->axistype = axistype;
	rate = fabs(speed / 6000.0);  //��ǰ��6000
	if (rate > 1)
		rate = 1;
	//��ʼ�����ȥ�������ÿ�ʼ�˶�
	if (axistype == XAxis) {
		XAxisSpeed = speed;
	}
	if (axistype == YAxis) {
		YAxisSpeed = speed;
	}
	if (axistype == ZAxis) {
		ZAxisSpeed = speed;
	}
	if (axistype == FixedPointRotation) {
		FixedPointRotationSpeed = speed;
}
if (axistype == FixedPointSwing)
{
	FixedPointSwingSpeed = speed;

}
if(axistype == FixedPointSwing || axistype == FixedPointRotation)
{

	if(!IsFixedPoint)
		{
		    IsFixedPoint = true;
			Joint j;
			Coint c;
			DylCommon::getCurrentPosition(j,c);
			ArrayXd xyzrpw = pose_2_xyzrpw(fksolution(j));
			x = xyzrpw[0];
			y = xyzrpw[1];
			z = xyzrpw[2];
		}
}
else
{
	IsFixedPoint = false;
}
if (axistype == ModifiedGear1Joy)
{
	ModifiedGear1JoySpeed = speed;
}
if (axistype == ModifiedGear2Joy)
{
	ModifiedGear2JoySpeed = speed;   //�ٶ�Ҳ��������
}
else
{
	this->speed = speed;    //���Լ�ʱ��ͣ����
	if (speed == 0)
	{
		cout << "�յ�0" << endl;
		Variable::IsStop = true;
	}
}
}

void RoctorBar::RoctorMove()
{

cout << "ҡ���ƶ���ʼ" << endl;
Joyruning = true;
targetJ = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};  //����Ƿ�������
double ss = 1000.0;
//��֤��������ͣ
//���ҡ������ٶ�     x y z ����� ����ת   ��50%����
int ADDTIM1 = 1;
int VVVSSS1 = 0;
long int VVVCCC1;
if (XAxisSpeed != 0)
	VVVCCC1 = Parameter::JoyMaxJspeed[0];
if (YAxisSpeed != 0)
	VVVCCC1 = Parameter::JoyMaxJspeed[1];
if (ZAxisSpeed != 0)
	VVVCCC1 = Parameter::JoyMaxJspeed[2];
if (FixedPointRotationSpeed != 0)
	VVVCCC1 = Parameter::JoyMaxJspeed[3];
if (FixedPointSwingSpeed != 0)
	VVVCCC1 = Parameter::JoyMaxJspeed[4];
if (ModifiedGear1JoySpeed != 0)
	VVVCCC1 = Parameter::JoyMaxJspeed[6];
if (ModifiedGear2JoySpeed != 0)
	VVVCCC1 = Parameter::JoyMaxJspeed[7];
int VADDTIMES1 = 1000;  //���ٲ���
double VVVAAA1 = ((double) (VVVCCC1 - VVVSSS1) / (VADDTIMES1 - 1) / ADDTIM1);//���ٶ�
while (true) {
	Joint currentJ = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	Coint currentC = { 0.0, 0.0 };
	if(robotType == CoordRobot)
	{
	  currentJ.j1 = (double) (MOT->J1step) / J1PUPR;
	  currentJ.j2 = (double) (MOT->J2step) / J2PUPR;
	}
	else
	{
	currentJ.j1 = (double)(MOT->J1step) * pi / 180.0 / J1PUPR;
	currentJ.j2 = (double)(MOT->J2step) * pi / 180.0 / J2PUPR;
	}
	currentJ.j3 = (double) (MOT->J3step) / J3PUPR;
	currentJ.j4 = (double) (MOT->J4step) * pi / 180.0 / J4PUPR;
	currentJ.j5 = (double) (MOT->J5step) * pi / 180.0 / J5PUPR;

	currentC.c1 = (double) (MOT->J7step) * pi / 180.0 / J7PUPR;
	currentC.c2 = (double) (MOT->J8step) * pi / 180.0 / J8PUPR;
	targetC = currentC;
	Vector3d lastV;
	lastV << XAxisSpeed / ss, YAxisSpeed / ss, ZAxisSpeed / ss ;
	Vector3d lastA;
	lastA << 0, FixedPointSwingSpeed / ss, FixedPointRotationSpeed / ss;
	Matrix4d new_robot_position = transl(lastV) * fksolution(currentJ);
	ArrayXd xyzrpw(6);
	usleep(100);
	xyzrpw = pose_2_xyzrpw(new_robot_position);
	xyzrpw[3] += lastA(0);
	xyzrpw[4] += lastA(1);
	xyzrpw[5] += lastA(2);
	if(FixedPointRotationSpeed != 0 || FixedPointSwingSpeed != 0)
	{
		xyzrpw[0] = x;
		xyzrpw[1] = y;
		xyzrpw[2] = z;
	}
	new_robot_position = xyzrpw_2_pose(xyzrpw);
    targetJ = NewPositionJointssolution(new_robot_position);
	if (ModifiedGear1JoySpeed != 0) {
		targetC.c1 += ModifiedGear1JoySpeed / ss;   //���������
	}
	if (ModifiedGear2JoySpeed != 0) {
		targetC.c2 += ModifiedGear2JoySpeed / ss;   //���������
	}   //����һ��ʼ�Ϳ�ʼ�����˶�
	if (Variable::IsStop || (XAxisSpeed == 0 && YAxisSpeed == 0 && ZAxisSpeed == 0&& FixedPointRotationSpeed == 0 && FixedPointSwingSpeed == 0&& ModifiedGear1JoySpeed == 0 && ModifiedGear2JoySpeed == 0))
	{
		double hh = 30;
		nowSpeed -= hh;
		if (nowSpeed <= 30)
		{
			robot_stop();
			cout << "stop stop stop" << endl;
			robot_stop();
			break;
		}
	}
		nowSpeed += VVVAAA1 * 0.5;		//0.2 * VVVAAA;
		if (nowSpeed >= VVVCCC1 * rate)
		{
			nowSpeed = VVVCCC1 * rate;
		}
}
cout << "ҡ���ƶ�����" << endl;
targetJ = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
targetC = {0,0};
Joyruning = false;
robot_stop();
}

//ҡ�˵�����
void RoctorBar::RoctorRun() {
while (targetJ.ISOK && Joyruning) {
	moto_runJoy(targetJ, targetC);
}
}

//�����ƶ�
void RoctorBar::SingleAxisMove() {
double speed1 = fabs(speed);
if (axistype == BigArm)  //���
{
	if(robotType == CoordRobot)
	moto_SettingJ(1, speed / (fabs(speed) + 1) * 9999, speed1);
	else
	moto_SettingJ(1, speed / (fabs(speed) + 1) * 720, speed1);
	return;
}
else if (axistype == SmallArm)  //С��
{
	if(robotType == CoordRobot)
	moto_SettingJ(2, speed / (fabs(speed) + 1) * 9999, speed1);
	else
	moto_SettingJ(2, speed / (fabs(speed) + 1) * 720, speed1);
	return;
}
else if (axistype == UpDownAxis)  //������

{
	moto_SettingJ(3, speed / (fabs(speed) + 1) * 9999, speed1);
	return;
} else if (axistype == RotorAxis)  //��ת��
		{
	moto_SettingJ(4, speed / (fabs(speed) + 1) * 720, speed1);
	return;
} else if (axistype == SwingAxis)  //����
		{
	moto_SettingJ(5, speed / (fabs(speed) + 1) * 720, speed1);
	return;
} else if (axistype == ModifiedGear1)  //��Ϊ����1
		{
	moto_SettingJ(7, speed / (fabs(speed) + 1) * 720, speed1);
	return;
} else if (axistype == ModifiedGear2)  //��λ����2
		{
	moto_SettingJ(8, speed / (fabs(speed) + 1) * 720, speed1);
	return;
}
}

void RoctorBar::run() {

if (axistype == UnKnown)
	return;
//�����ƶ�
else if (axistype == XAxis || axistype == YAxis || axistype == ZAxis
		|| axistype == FixedPointRotation || axistype == FixedPointSwing
		|| axistype == ModifiedGear1Joy || axistype == ModifiedGear2Joy) {
	RoctorMove();
	axistype = UnKnown;
	return;
} else
	SingleAxisMove();
axistype = UnKnown;
}

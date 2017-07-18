/*
 * Parser.cpp
 *
 *  Created on: 2016��11��15��
 *      Author: Administrator
 */
#include "Parser.h"
#include <semaphore.h>
#include <math.h>
#include <cstring>
#include "DlyCommon.h"
#include "Parameter.h"

extern string lastcmd;
extern Parameter structParameter;
Parser::Parser() :
		sdata(""), back_finished(false), IsConnnect(true), runing(false), IsClear(
				false), IsCutting(false) {
	IsSend = false;
}

Parser::~Parser() {

}

void Parser::Datereceive() {
	if (*(sdata.begin()) == '[')
		sdata.erase(sdata.begin()); //ȥ��ͷ'['
	if (*(sdata.end() - 1) == ']')
		sdata.erase(sdata.end() - 1); //ȥ��β']'
	vector<string> strArray = DylCommon::split(sdata, ","); //���
	for (int i = 0; i < (int) strArray.size() - 2; i++)
		checkstr += (strArray[i] + ",");
	checkstr += strArray[strArray.size() - 2];
	cmd = strArray[0]; //�õ������ַ�
	for (int i = 1; i < (int) strArray.size() - 1; i++)
		parserdata.push_back(strtod(strArray[i].c_str(), NULL)); //stringתfloat
	checkValue = *(strArray.end() - 1);
}

//У�麯��
bool Parser::check() {
	if (parserdata.size() < 1)
		return false;
	MD5 md5;
	md5.reset();
	md5.update(checkstr);
	string str = md5.toString();
	checkstr = "";
	if ((parserdata[0] == parserdata.size() + 2) && str == checkValue)
		return true;
	else
		return false;
}
void Parser::run() {
	updateRun();	// �������³���
	if (sdata == "" || cmd == "")
		return;
	//û������
	if (runing == false)
		return;
	robotback.run();  //����ִ��
	fixposmove.run(); //�����ƶ�ִ��
	roctorbar.run();  //ҡ������ֻ�ǲ��ϼ�����ֵ���˶�������һ���߳�
	welding.run();    //��������
	torchcalibration.run(); //��ǹ�궨
	modifiedGearDemarcate.run(); //��Ϊ�������У�
}

void Parser::cmdparser() {

	//��������
	/**********************Eϵ��ָ�����*******************/
if (cmd == "E0")
{
	cout << "���ֳɹ�" << endl;
	DylCommon::protocol_send("E0,3");
	runing = true;
	return;
}
else if (cmd == "E1")
{
	cout << "�ṹ�����´�" << endl;
	//+�ṹ������������
	structParameter.setData(parserdata[1], parserdata[2], parserdata[3],
			parserdata[4]);
	return;
} else if (cmd == "E2")
{

	cout << "���嵱���´�" << endl;
	//���嵱����������
	J1PUPR = parserdata[1];
	J2PUPR = parserdata[2];
	J3PUPR = parserdata[3];
	J4PUPR = parserdata[4];
	J5PUPR = parserdata[5];
	J7PUPR = parserdata[6];
	J8PUPR = parserdata[7];

	return;
} else if (cmd == "E3") {
	PointType pointtype;
	pointtype = PointType((int) parserdata[1]);
	// debug
	cout << "TCF�궨:" << endl;
	cout << "�궨�㣺" << pointtype << endl;
	//TCF��������
	torchcalibration.setData(pointtype);
	return;
} else if (cmd == "E5") {
	int h = 1;
	Matrix4d mat = Matrix4d::Zero();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			double s = parserdata[h++];
			if (finite(s) == 0)   //��������ֵΪ����ֵ���򷵻س�ȥ
			{
				return;
			}
			mat(i, j) = s;
		}
	}
	cout << "TCF�����´�" << endl;
	//tcf��������
	torchcalibration.setData(mat);

} else if (cmd == "EE") {
	cout << "�ش�ָ��" << endl;
	//�ش���һ��ָ��
	DylCommon::protocol_send2(lastcmd);
} else if (cmd == "EF") {
	DylCommon::protocol_send1("EF,3");
	IsConnnect = true;

}
else if(cmd == "EB") {
	PointType pointtype;
	pointtype = PointType((int) parserdata[1]);
	// debug
	cout << "POS�궨:" << endl;
	cout << "�궨�㣺" << pointtype << endl;
	//TCF��������
	modifiedGearDemarcate.setData(pointtype);
	return;
}
else if(cmd == "EC") {
	int h = 1;
	Matrix4d mat = Matrix4d::Zero();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			double s = parserdata[h++];
			if (finite(s) == 0)   //��������ֵΪ����ֵ���򷵻س�ȥ
			{
				return;
			}
			mat(i, j) = s;
		}
	}
	cout << "POS�����´�" << endl;
	//tcf���Ծ���
	modifiedGearDemarcate.setData(mat);
}

/**********************Aϵ��ָ�����----�����ƶ�*******************/

//XAxis,YAxis,BigArm,SmallArm,UpDownAxis,RotorAxis,SwingAxis,ModifiedGear1,ModifiedGear2,FixedPointRotation,
//FixedPointSwing,UnKnown
//�����ƶ�
else if (cmd == "A0")
{
	fixposmove.setData(XAxis, parserdata[1], parserdata[2]);
	return;
}
else if (cmd == "A1")
{
	fixposmove.setData(YAxis, parserdata[1], parserdata[2]);
	return;
}
else if (cmd == "A2")
{
	fixposmove.setData(BigArm, parserdata[1], parserdata[2]);
	return;
}
else if (cmd == "A3")
{
	fixposmove.setData(SmallArm, parserdata[1], parserdata[2]);
	return;
}
else if (cmd == "A4")
{
	fixposmove.setData(UpDownAxis, parserdata[1], parserdata[2]);
	return;
}
else if (cmd == "A5")
{
	fixposmove.setData(RotorAxis, parserdata[1], parserdata[2]);
	return;
}
else if (cmd == "A6")
{
	fixposmove.setData(SwingAxis, parserdata[1], parserdata[2]);
	return;
}
else if (cmd == "A7")
{
	fixposmove.setData(ModifiedGear1, parserdata[1], parserdata[2]);
	return;
}
else if (cmd == "A8")
{
	fixposmove.setData(ModifiedGear2, parserdata[1], parserdata[2]);
	return;
}

/******************Bϵ��ָ�����***********************/
else if (cmd == "B0")   //��ס��ʱ�Ĺ�
{
	roctorbar.setData(XAxis, parserdata[1]);
//		if (fabs(parserdata[1]) < 100)
//		Variable::IsStop = true;
	return;
}

else if (cmd == "B1")
{
	roctorbar.setData(YAxis, parserdata[1]);
//		if (fabs(parserdata[1]) < 100)
//		Variable::IsStop = true;
	return;
}

else if (cmd == "B2")
{
	roctorbar.setData(BigArm, parserdata[1]);
	return;
}

else if (cmd == "B3")
{
	roctorbar.setData(SmallArm, parserdata[1]);
	return;
}
else if (cmd == "B4")
{
	roctorbar.setData(ZAxis, parserdata[1]);
//		if (fabs(parserdata[1]) < 100)
//		Variable::IsStop = true;
	return;
}

else if (cmd == "B5")
{
	roctorbar.setData(RotorAxis, parserdata[1]);  //ͬһ�ٶȣ��ڶ������ת�����������
	return;
}

else if (cmd == "B6")
{
	roctorbar.setData(SwingAxis, parserdata[1]);
	return;
}

else if (cmd == "B7")
{
	roctorbar.setData(ModifiedGear1, parserdata[1]);
	return;
}
else if (cmd == "B8")
{
	roctorbar.setData(ModifiedGear2, parserdata[1]);
	return;
}
else if (cmd == "B9")
{

	roctorbar.setData(FixedPointRotation, parserdata[1]);
//		if (fabs(parserdata[1]) < 100)
//		Variable::IsStop = true;
	return;
}
else if (cmd == "BA")
{
	roctorbar.setData(FixedPointSwing, parserdata[1]);
//		if (fabs(parserdata[1]) < 100)
//		Variable::IsStop = true;
	return;
}
//������
else if (cmd == "BB")
{
	roctorbar.setData(UpDownAxis, parserdata[1]);
	return;
}

else if(cmd == "BE")   //��λ����1ҡ��ָ��
{
	roctorbar.setData(ModifiedGear1Joy, parserdata[1]);
	return;
}
else if(cmd == "BF")  //��λ����2ҡ��ָ��
{
	roctorbar.setData(ModifiedGear2Joy, parserdata[1]);
	return;
}
//���յ�BCָ�����������ʾ�̹�����
else if (cmd == "BC")
{
	moto_XYZclear();  //XYZ��������
	usleep(400000);
	DylCommon::protocol_send("BC,3");//���ܵ�BC����´�����
}
//���ӹ�������Ҫ����
else if (cmd == "BD")
{
	IsClear = true;
	IsCutting = true;
}


/**********************Cϵ��ָ�����*******************/
//����ָ�����
else if (cmd == "C0")
{
	robotback.setData(1, parserdata[1], parserdata[2]);
	return;
}

else if (cmd == "C1")
{
	robotback.setData(2, parserdata[1], parserdata[2]);
	return;
}
else if (cmd == "C2")
{
	robotback.setData(3, parserdata[1], parserdata[2]);
	return;
}
else if (cmd == "C3")
{
	robotback.setData(4, parserdata[1], parserdata[2]);
	return;
}
//����ָ�����
else if (cmd == "C4")
{
	robotback.setData(5, parserdata[1], parserdata[2]);
	return;
}
else if (cmd == "C5")
{
	robotback.setData(7, parserdata[1], parserdata[2]);
	return;
}
else if (cmd == "C6")
{
	robotback.setData(8, parserdata[1], parserdata[2]);
	return;
}
else if (cmd == "C7")
{
	cout << "ȫ������ɹ���ĵ�λ����" << endl;
	moto_init();
	back_finished = true;
	return;
}
else if (cmd == "C8")
{
	if (parserdata[1] == 0)
	Variable::RotorAxisDirectionChange = false;
	else if (parserdata[1] == 1)
	Variable::RotorAxisDirectionChange = true;
}
else if (cmd == "C9")
{
	if (parserdata[1] == 0)
	Variable::SwingAxisDirectionChange = false;
	else if (parserdata[1] == 1)
	Variable::SwingAxisDirectionChange = true;
}
else if (cmd == "CA")
{
	if (parserdata[1] == 0)
	Variable::ModifiedGear1DirectionChange = false;
	else if (parserdata[1] == 1)
	Variable::ModifiedGear1DirectionChange = true;
}
else if (cmd == "CB")
{
	if (parserdata[1] == 0)
	Variable::ModifiedGear2DirectionChange = false;
	else if (parserdata[1] == 1)
	Variable::ModifiedGear2DirectionChange = true;
}

/************************Dϵ��ָ�����*************************/
else if (cmd == "D1")
{
	parserReceviedCoordPoints();   //���ս�����λ���´��ĵ�λ����
	return;
}
else if (cmd == "D2")
{
	cout << "���л�ָ��" << endl;
	//�����л��������ý�ȥ   ��ʱ�ͱ�ŵ�����
	welding.receiveArcStrickData(parserdata[1],parserdata[6]);
	return;
}
else if (cmd == "D3")
{
	cout << "Ϩ��ָ��" << endl;
	welding.receiveArcQuenchData(parserdata[3]);
	return;
}
else if (cmd == "D4")    //��ʼ�´���ָ��
{
	cout << "��ʼ�´�ͼ������" << endl;
	IsSend = true;    //���͹������ǲ�����У������ָ���
	welding.init();
	DylCommon::protocol_send("D4,3");
	return;
}
else if (cmd == "D5")
{
	//�´��������Կ�ʼ����
	if(IsClear)
	{
		initXyzrpw = welding.graph[0].startXyzrpw;     //���ļ��е�һ���㱣������
		IsClear = false;
	}
	welding.getInterpolations(); //�������ݵ�
	IsSend = false;
	DylCommon::protocol_send("D5,3");
	return;
}
else if (cmd == "D7")
{

	cout << "��ʼ����" << endl;
	if (IsCutting)
	{
		Joint j;
		Coint c;
		DylCommon::getCurrentPosition(j, c);
		Matrix4d mat = fksolution(j);
		ArrayXd xyzrpw = pose_2_xyzrpw(mat);
		for (int i = 3; i < 6; i++)
		xyzrpw[i] = initXyzrpw[i];  //�ѳ�ʼֵ��rpw���ƹ���
		Matrix4d mat1 = xyzrpw_2_pose(xyzrpw);
		j = NewPositionJointssolution(mat1);
		cout << "�ı���̬" << endl;
		moto_runJAbs(j, c, 4000);
		cout << "�ı���̬���" << endl;
		moto_XYZclear();//XYZ��������
		IsCutting = false;
		usleep(1000);
	}
	welding.runing = true;  //��ʼ����
	return;
}
else if (cmd == "D8") //��ָͣ��
{
	Variable::IsStop = true;
	return;
}
else if (cmd == "D9") //ָֹͣ��
{
	Variable::IsStop = true;
	cout << "ֹͣ�˶�" << endl;
	return;
}
else if (cmd == "DA")
{
	cout << "�ٶȵ���ָ��" << endl;
	welding.rate = parserdata[1] / 9999.0;
	return;
}
else if (cmd == "DG")
{
	cout << "ģʽ�л�" << endl;
	welding.IsFireMode = parserdata[1];   //����ģʽ��״̬
	return;
}
else if (cmd == "DH")
{
	cout << "����ָ��" << endl;
	welding.backruning = true;
	return;
}
/***************Fϵ��ָ�����***********************/
else if (cmd == "F0")
{
	cout << "IO��ָ���´�" << endl;
	ioparameter.setData(parserdata[1]);
	return;
}
else if (cmd == "F5")
{
	cout << "ϵͳ�ָ�ָ��" << endl;
	DylCommon::protocol_send("F5,3");
	return;
} else if (cmd == "F7")
{
	cout << "�汾����ʾָ��" << endl;
	DylCommon::protocol_send("F7,4," + version);
	return;
}

//************************** G ָ����� ************************* /
//����ٶȵĲ��� �����ײ���д����

else if (cmd == "G1")
{
	Parameter::SingleAxisMaxJspeed[0] = parserdata[1];
	cout << "X������ٶ��´���" << parserdata[1] << endl;
	return;
}

else if (cmd == "G2")
{
	Parameter::SingleAxisMaxJspeed[1] = parserdata[1];
	cout << "Y������ٶ��´���" << parserdata[1] << endl;
	return;
}
else if (cmd == "G3")
{
	Parameter::SingleAxisMaxJspeed[2] = parserdata[1];
	cout << "Z������ٶ��´���" << parserdata[1] << endl;
	return;
}
else if (cmd == "G4")
{
	Parameter::SingleAxisMaxJspeed[3] = parserdata[1];
	cout << "A������ٶ��´���" << parserdata[1] << endl;
	return;
}
else if (cmd == "G5")
{
	Parameter::SingleAxisMaxJspeed[4] = parserdata[1];
	cout << "B������ٶ��´���" << parserdata[1] << endl;
	return;
}

else if (cmd == "G6")
{
	for (int i = 1; i <= 5; i++)
	{
		if (parserdata[i] <= 0 && finite(parserdata[i]))
		{
			cout << "����ٶ��´����ݲ���" << endl;
			return;
		}
		Parameter::SingleAxisMaxJspeed[i - 1] = parserdata[i];
	}
	for (int i = 6; i <= 10; i++)
	{
		if (parserdata[i] <= 0 && finite(parserdata[i]))
		{
			cout << "ҡ�˺����ٶ��´����ݲ���" << endl;
			return;
		}
		Parameter::JoyMaxJspeed[i - 6] = parserdata[i];
	}
	return;
}

else if (cmd == "G7")
{
	Parameter::JoyMaxJspeed[0] = parserdata[1];

	cout << "X��ҡ�˺����ٶ��´���" << parserdata[1] << endl;
	return;
}
else if (cmd == "G8")
{
	Parameter::JoyMaxJspeed[1] = parserdata[1];
	cout << "y��ҡ�˺����ٶ��´���" << parserdata[1] << endl;
	return;
}
else if (cmd == "G9")
{
	Parameter::JoyMaxJspeed[2] = parserdata[1];
	cout << "z��ҡ�˺����ٶ��´���" << parserdata[1] << endl;
	return;
}
else if (cmd == "GA")
{
	Parameter::JoyMaxJspeed[3] = parserdata[1];
	cout << "����תҡ�˺����ٶ��´���" << parserdata[1] << endl;
	return;
}
else if (cmd == "GB")
{
	Parameter::JoyMaxJspeed[4] = parserdata[1];
	cout << "�����ҡ�˺����ٶ��´���" << parserdata[1] << endl;
	return;
}

else if(cmd == "GC")
{
	Parameter::SingleAxisMaxJspeed[6] = parserdata[1];
	cout << "��λ����1������ٶ��´���" << parserdata[1] << endl;
	return;
}

else if(cmd == "GD")
{
	Parameter::SingleAxisMaxJspeed[7] = parserdata[1];
	cout << "��λ����2������ٶ��´���" << parserdata[1] << endl;
	return;
}

else if(cmd == "GE")
{
	Parameter::JoyMaxJspeed[6] = parserdata[1];
	cout << "��λ����1�ٶ��´���" << parserdata[1] << endl;
    return;
}  //��λ����2�ٶ�
else if(cmd == "GF")
{
	Parameter::JoyMaxJspeed[7] = parserdata[1];
	cout << "��λ����2�ٶ��´���" << parserdata[1] << endl;
    return;
}
}

//���ݽ���
void Parser::DateParser(const string &str) {
sdata = str;
//��������
Datereceive();
	if (cmd != "EF")
	{
		cout << cmd << " ";
	    for (int i = 0; i < (int) parserdata.size(); i++)
		cout << parserdata[i] << " ";   //�����ݴ�ӡ����
	    cout << endl;
	}
if (!check()) //У�鲻ͨ��
{
	string str;
	if (cmd == "D1")    //ͼ���´���У��ʧ��
			{
		str = "D6,3";
		welding.init();
		cout << "ͼ�����ݴ��������ش� " << endl;
	} else {
		str = "EE,3";    //һ������У��ʧ��
		cout << "�������ݴ��������ش�" << endl;
	}
	usleep(100);
	DylCommon::protocol_send1(str);
	cmd = "";
	return;
}

//��������
Variable::IsStop = false;
cmdparser();
parserdata.clear();
}

/***********�ؽ�ʽ��������***************/
//���ս�����λ���´��ĵ�λ����
void Parser::parserReceviedCoordPoints() {

//ֱ��     D1 0   X1 Y1 Z1 R1 P1 W1 U1 V1     X2 Y2 Z2 R2 P2 W2 U2 V2 S N
//Բ��     D1 4   X1 Y1 Z1 R1 P1 W1 U1 V1  X Y Z R P W U V  X2 Y2 Z2 R2 P2 W2 U2 V2 S N
ArrayXd xyzrpw1 = ArrayXd::Zero(6);      //���
Coint c1 = { 0, 0 };
ArrayXd xyzrpw2 = ArrayXd::Zero(6);
Coint c = { 0, 0 };
ArrayXd xyzrpw = ArrayXd::Zero(6);
Coint c2 = { 0, 0 };
for (int i = 2; i < 8; i++)          //���
	xyzrpw1[i - 2] = parserdata[i];
c1.c1 = parserdata[8] * pi / 180;
c1.c2 = parserdata[9] * pi / 180;
for (int i = 10; i < 16; i++)        //�ڶ�����
	xyzrpw2[i - 10] = parserdata[i];
c2.c1 = parserdata[16] * pi / 180;
c2.c2 = parserdata[17] * pi / 180;
double speed = parserdata[18];
int num = parserdata[19];
if (parserdata[1] == 4 || parserdata[1] == 5)   //Բ��
		{
	xyzrpw = xyzrpw2;  //�ѵڶ��㶨Ϊ�е�
	c = c2;
	for (int i = 18; i < 24; i++)    //�������� Ϊĩ��
		xyzrpw2[i - 18] = parserdata[i];
	c2.c1 = parserdata[24] * pi / 180;
	c2.c2 = parserdata[25] * pi / 180;
	speed = parserdata[26];
	num = parserdata[27];
}
switch ((int) parserdata[1])  //���������������������ݵ�       0--������   1--�岹��  4--Բ����
{
case 0:
	welding.receiveLinePoints(xyzrpw1, c1, xyzrpw2, c2, speed, num, false);
	break;
case 1:
	welding.receiveLinePoints(xyzrpw1, c1, xyzrpw2, c2, speed, num, true);
	break;
case 4:
	welding.receiveArcPoints(xyzrpw1, c1, xyzrpw, c, xyzrpw2, c2, speed, num);
	break;
case 5:
	welding.receiveCirclePoints(xyzrpw1, c1, xyzrpw, c, xyzrpw2, c2, speed, num);
//	welding.receiveCircle1Points(xyzrpw1, c1, xyzrpw, c, xyzrpw2, c2, speed, num);
	break;
}
}

void Parser::changeRPW(ArrayXd &xyzrpw) {
for (int i = 3; i < 6; i++) {
	if (xyzrpw[i] < 0)
		xyzrpw[i] += 360;
}

}

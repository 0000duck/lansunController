/*
 * Coordinates.cpp
 *
 *  Created on: 2016��11��17��
 *      Author: Administrator
 */

#include "Coordinates.h"
#include "moto.h"
#include <iostream>
using namespace std;
#include <stdio.h>
#include <string>
#include "robot_class/udp.h"
//��ʼ���б�
Coordinates::Coordinates():conn(0)
{
	lastcurJ={1,1,1,1,1,1};
	lastcurC={1,1};
}

Coordinates::~Coordinates()
{

}
void Coordinates::setConn(int conn)
{
   this->conn=conn;
}
void Coordinates::getPosition()
{
	DylCommon::getCurrentPosition(curJ,curC);                 //�õ���ǰ������
	if(DylCommon::IsCoordEqual(curJ,lastcurJ,curC,lastcurC))   //�ж������Ƿ����
		return;
	lastcurJ=curJ;
	lastcurC=curC;
	xyzrpw = pose_2_xyzrpw(fksolution(curJ));
	ostringstream os;
	os.clear();
	string str = "D0,11";
	for (int i = 0; i < 6; i++)
	{
		os.str("");
		os << xyzrpw[i];
		str.append(",").append(os.str());
	}
	os.str("");
	os << curC.c1 * 180 / pi;
	str.append(",").append(os.str());
	os.str("");
	os << curC.c2 * 180 / pi;
	str.append(",").append(os.str());
	DylCommon::protocol_send1(str);      //�ش�����ֵ
	if(udp::IsOpenUdp)   //����udpģʽ�Żش�����ֵ
	{
		char jdata[1000];
		sprintf(jdata,"UC,16,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",xyzrpw[0],xyzrpw[1],xyzrpw[2],xyzrpw[3],xyzrpw[4],xyzrpw[5],curJ.j1,curJ.j2,curJ.j3,curJ.j4,curJ.j5,curJ.j6,curC.c1,curC.c2);
		string strJData(jdata);
		udp::sendProgramData(strJData);
	}
}
//�������ݻش�
void Coordinates::run()
{
    getPosition();
    if(udp::IsOpenUdp)
    usleep(200000);     //�����ӻش�һ��
    else
    usleep(200000);
}

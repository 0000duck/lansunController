/*
 * moto.h
 *
 *  Created on: 2016��5��23��
 *      Author: Yin
 */

#ifndef LANSUNV2_0_SRC_MOTO_H_
#define LANSUNV2_0_SRC_MOTO_H_

#include "common.h"
#include <iostream>
using namespace std;
void clcPUs(int axis);
void moto_init(void);
void moto_XYZclear(void);
extern double Vspeed;
//��������ƫ��ָ�����룬��֤��е�۳�һ����
//��е��ֹͣ
void robot_stop(void);
void moto_SettingJ(char ch, double angle,double speed);
void moto_runJoy(Joint j, Coint c);
void moto_runJoy1(int ch);
bool moto_runJAbs(Joint j, Coint c,double speed);
void moto_runJoyAbs(Joint j, Coint c);
//=====================================================�岹�߶��˶�����
void moto_runInterpolationAbs(Joint j, Coint c,float speed);
void moto_runJ(Joint j, Coint c,double speed);
void getVList(unsigned long int *table);
void delayNus(int n);
void delay1ms(void);
void delayNms(float n);

//��ȡ��������֮��ĽǶ�
bool getTwoVector3Angle(Vector3d p1, Vector3d p2);
float getTwoVector3AngleValue( Vector3f p1,  Vector3f p2);
//J1���е���λ�ٽ�� ����Ѿ���λ����ʱ����ת������˳ʱ��ת����⵽��λ�ٽ�������ֹͣ
void J1RunToLimit(char ch, int speedL);

//���ô˺���ʱ��һ��Ҫ��֤J4��ǹ��ת��������ڳ�ʼλ�����
void allAxisInit(void);

#endif /* LANSUNV2_0_SRC_MOTO_H_ */

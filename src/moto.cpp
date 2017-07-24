/*
 * moto.cpp
 *
 *  Created on: 2016��5��23��
 *      Author: Administrator
 */

#include "moto.h"
#include "Parameter.h"
using namespace std;
#include <stdio.h>
#include <iostream>

//MOT
static int a1 = 0;	//���ٶ�
double Vspeed=0;


//���������
void clcPUs(int axis)
{
	MOT->field.D3 = 'C';                                   //D1 D2 D3 D4 ����ʲô
	MOT->field.D2 = 'J';
	switch(axis)   //��¼���ĸ��������к͹���
	{
		case 1:MOT->field.D1 = '1';break;   //x
		case 2:MOT->field.D1 = '2';break;   //y
		case 3:MOT->field.D1 = '3';break;   //z
		case 4:MOT->field.D1 = '4';break;
		case 5:MOT->field.D1 = '5';break;
		case 6:MOT->field.D1 = '6';break;
		case 7:MOT->field.D1 = '7';break;
		case 8:MOT->field.D1 = '8';break;
	   default:MOT->field.D1 = '0';break;
	}
	MOT->field.D0 = 'c';
}


//   vs�������ٶ� pu/s
//   vc�������ٶ� pu/s
//    t������ʱ�� s
//table���岹�����������Ƶײ�����岹�ٶȣ�
void getVList(unsigned long int *table)
{

	for(int i=0;i<VADDTIMES;i++)
	{
		table[i] = (unsigned long int)(6250000.0 / (VVVSSS + VVVAAA * ADDTIM * i));//��¼��ͬ���ٶ�
	}

}
//
void delayNus(int n)    //΢�뼶���
{
	struct timeval temp;
	if(n<=0)
		return;
	temp.tv_sec = 0;
	temp.tv_usec = n;
	select(0,NULL,NULL,NULL,&temp);
}

void delay1ms(void)
{
	struct timeval temp;
	temp.tv_sec = 0;
	temp.tv_usec = 1000;
	select(0,NULL,NULL,NULL,&temp);//���ƾ��Ǻ�ʱ�ĳ���
}

void delayNms(float n){
    if(n<=0)
    	return;
	struct timeval temp;
	temp.tv_sec = 0;
	temp.tv_usec = (int)(1000 * n);
	select(0,NULL,NULL,NULL,&temp);
}

//��ȡ��������֮��ĽǶ�
bool getTwoVector3Angle(Vector3d p1, Vector3d p2)
{
	 //����������/ģ  cosֵ  -1 �� 1 ֮��
//		float costheta = p1.dot(p2) / (Vector3d().absV3(p1) * Vector3d().absV3(p2));
	float costheta = p1.dot(p2) / (p1.norm() * p2.norm());
	if(costheta >= 1)
		costheta = 1;
	else if(costheta <= -1)
		costheta = -1;
	float theta = acosf(costheta);   //�Ƕ�ֵ �ǻ���
	return (theta < 0.05*pi) ? true : false;   //�жϻ��ȵķ�Χ������״̬            ����̫С�����ط�Χ����

}

float getTwoVector3AngleValue( Vector3f p1,  Vector3f p2)
{
    //����������/ģ  cosֵ  -1 �� 1 ֮��
//	float costheta = p1.dot(p2) / (Vector3d().absV3(p1) * Vector3d().absV3(p2));
	float costheta = p1.dot(p2) / (p1.norm() * p2.norm());
	if(costheta >= 1.)
		costheta = 1.;
	else if(costheta <= -1.)
		costheta = -1;
	float theta = acosf(costheta);   //�Ƕ�ֵ �ǻ���

	return theta*180/pi;   //�жϻ��ȵķ�Χ������״̬            ����̫С�����ط�Χ����
}

//�˺����Ĺ����Ǹ����ؽڽǵĶ���
void moto_runJ(Joint j, Coint c,double speed)
{
    cout << "�����ƶ���ʼ" << endl;
    double rate = speed / 6000.0;
    if(rate > 1)
    	rate = 1;
	int i = 0;
	int a = 0;	//���ٶ�
	unsigned long int isContia = 0;     //����
	MOT->field.D3 = 'S';//ָֹͣ���

	//ת��Ϊ������
	 long long int j1pu;
	 long long int j2pu;
	if(robotType == CoordRobot)
	{
	  j1pu = j.j1  * J1PUPR;
	  j2pu = j.j2  * J2PUPR;
	}
	else
	{
	  j1pu = j.j1 / pi*180 * J1PUPR;
	  j2pu = j.j2 / pi*180 * J2PUPR;
	}
	long long int j3pu = j.j3          * J3PUPR;
	long long int j4pu = j.j4 / pi*180 * J4PUPR;
	long long int j5pu = j.j5 / pi*180 * J5PUPR;
	long long int j6pu = j.j6 / pi*180 * J6PUPR;
	long long int j7pu = c.c1 / pi*180 * J7PUPR;
	long long int j8pu = c.c2 / pi*180 * J8PUPR;

    //����ǰ��������������
	long int oldJ1step = MOT->J1step;
	long int oldJ2step = MOT->J2step;
	long int oldJ3step = MOT->J3step;
	long int oldJ4step = MOT->J4step;
	long int oldJ5step = MOT->J5step;
	long int oldJ6step = MOT->J6step;
	long int oldJ7step = MOT->J7step;
	long int oldJ8step = MOT->J8step;

   //�ҵ�һ��������ݻ�
	unsigned long int contia = (unsigned long int)(sqrtf(j1pu * j1pu + j2pu * j2pu + \
			   										 j3pu * j3pu + j4pu * j4pu + \
														 j5pu * j5pu + j6pu * j6pu + \
														 j7pu * j7pu + j8pu * j8pu));

	   if(contia == 0)
	   {
		   cout << "����̲��ƶ�" <<endl;
		   return;
	   }
	   long int maxspeed = 0;
	   while(1)
	   {
		   long int maxspeed1 = maxspeed;
		   maxspeed1 += 500;
		   double v1 = fabs((double)j1pu / (double)contia * maxspeed1);
		   double v2 = fabs((double)j2pu / (double)contia * maxspeed1);
		   double v3 = fabs((double)j3pu / (double)contia * maxspeed1);
		   double v4 = fabs((double)j4pu / (double)contia * maxspeed1);
		   double v5 = fabs((double)j5pu / (double)contia * maxspeed1);
		   double v6 = fabs((double)j6pu / (double)contia * maxspeed1);
		   double v7 = fabs((double)j7pu / (double)contia * maxspeed1);
		   double v8 = fabs((double)j8pu / (double)contia * maxspeed1);
		   if(v1 > Parameter::SingleAxisMaxJspeed[0] || v2 > Parameter::SingleAxisMaxJspeed[1] ||
			  v3 > Parameter::SingleAxisMaxJspeed[2] || v4 > Parameter::SingleAxisMaxJspeed[3] ||
			  v5 > Parameter::SingleAxisMaxJspeed[4] || v6 > Parameter::SingleAxisMaxJspeed[5] ||
			  v7 > Parameter::SingleAxisMaxJspeed[6] || v8 > Parameter::SingleAxisMaxJspeed[7]
			 )
			   break;
		   maxspeed =  maxspeed1;

	   }
	   //ͨ��������ٶ�����������ĺ��ٶ�


	int  ADDTIM2 = 1;
	int  VVVSSS2 = 3000;
	long  int  VVVCCC2 = maxspeed *rate;
	int  VADDTIMES2 = 1000;  //���ٲ���
	double VVVAAA2= ((double)(VVVCCC2 - VVVSSS2) / (VADDTIMES2 - 1) / ADDTIM2);	//���ٶ�
	double MINPU2	=	((VVVSSS2 + VVVCCC2) / 2 * ADDTIM2);
	long int vddTable2[VADDTIMES2];
	for(int i=0;i<VADDTIMES;i++)
		{
		vddTable2[i] = (long int)(6250000.0 / (VVVSSS2 + VVVAAA2 * ADDTIM2 * i));//��¼��ͬ���ٶ�
		}
//������д��
	MOT->J1step = j1pu;         //�������λ��
	MOT->J2step = j2pu;
	MOT->J3step = j3pu;
	MOT->J4step = j4pu;
	MOT->J5step = j5pu;
	MOT->J6step = j6pu;
	MOT->J7step = j7pu;
	MOT->J8step = j8pu;
	MOT->contia = contia;
	MOT->Tt = vddTable2[0];//��ʱ������ı��ٶ�
	MOT->field.D3 = 'R';//��ʼ����            Ҫ�ȿ�ʼ����ʱ������Żᴫ���ݸ�����         //Ƕ��ʽ����
//����δ��ɵ�״̬

	while(MOT->field.D3 == 'N')
	{
		 if(Variable::IsStop)   //������յ�ָֹͣ�������ͣ����
		 {
//			 cout<<"����"<<endl;
		  i = i - 5;
		  if(i < 1)
		  {
			cout<<"���ƶ���ͣ"<<endl;
			robot_stop();
			Variable::IsStop = false;
			break;
		  }
		 }
//��ֵ����ֵ�ò�࣬��ɶ�    ����ǲ���ˢ�µ���
		long long int j1has = MOT->J1step - oldJ1step;   //ʵ���Ѿ���ɵ��������
		long long int j2has = MOT->J2step - oldJ2step;
		long long int j3has = MOT->J3step - oldJ3step;
		long long int j4has = MOT->J4step - oldJ4step;
		long long int j5has = MOT->J5step - oldJ5step;
		long long int j6has = MOT->J6step - oldJ6step;
		long long int j7has = MOT->J7step - oldJ7step;
		long long int j8has = MOT->J8step - oldJ8step;

		isContia = (unsigned long int)(sqrtf(j1has * j1has + j2has * j2has + \
											 j3has * j3has + j4has * j4has + \
											 j5has * j5has + j6has * j6has + \
											 j7has * j7has + j8has * j8has));    //�ܵ�������

		if(contia <= 2 * MINPU2){	//���߶γ��Ȳ�����Ӽ��ٱ�Ҫ����ʱ  ��������ٶȺö�

			if(isContia < contia / 2){       //������ﲻ��һ���ʱ��
				a += VVVAAA2;	//���ٶ�         //��һ���ٶ�
			}else{
				a -= VVVAAA2;       //�Ϳ�ʼ��һ���ٶ�
				if(a <= 0){
					a = 0;
				}
			}
			MOT->Tt = (unsigned long int)(6250000.0 / (VVVSSS2 + a));      //������ٶȿ���

		}else{						//��ȫ�Ӽ���

			if(isContia < MINPU2)                          //���������Ǹ�
			{											//����
				MOT->Tt = vddTable2[i];   //�õ���һ���ٶ��ݶȣ����ٶ��ݶȸ�������һ��������ٶȣ���������ô������
				i++;
				if(i >= VADDTIMES2){
					i = VADDTIMES2 - 1;
				}
			}else if(isContia >= MINPU2 && isContia < (contia - MINPU2)){		//����   ������һֱ�����˶����Գ�λ��
				MOT->Tt = vddTable2[i];
			}else{
				MOT->Tt = vddTable2[i];		//�����ٶ��ݶȸ����� ����								//����
				i--;
				if(i <= 0){
					i = 0;
				}
			}
		}
		delay1ms();     //��ʱ1s,����ȫ�����Ϊֹ
	 if(Variable::IsStop)   //������յ�ָֹͣ�������ͣ����
			 {
	//			 cout<<"����"<<endl;
			  i = i - 5;
			  if(i < 1)
			  {
				cout<<"���ƶ���ͣ"<<endl;
				robot_stop();
				Variable::IsStop = false;
				break;
			  }
	}
	}
	robot_stop();
}



//����
bool moto_runJAbs(Joint j, Coint c,double speed)
{
	    int i = 0;
		int a = 0;	//���ٶ�
		double rate = speed / 6000;
		if(rate > 1)   //�ٶȲ��ܳ���1
			rate = 1;
		unsigned long int isContia = 0;
		MOT->field.D3 = 'S';
		long int oldJ1step = MOT->J1step;
		long int oldJ2step = MOT->J2step;
		long int oldJ3step = MOT->J3step;
		long int oldJ4step = MOT->J4step;
		long int oldJ5step = MOT->J5step;
		long int oldJ6step = MOT->J6step;
		long int oldJ7step = MOT->J7step;
		long int oldJ8step = MOT->J8step;
		long long int j1pu;
		long long int j2pu;
        if(robotType == CoordRobot)
        {
		 j1pu = j.j1 * J1PUPR - oldJ1step;
		 j2pu = j.j2 * J2PUPR - oldJ2step;
        }
        else
        {
         j1pu = j.j1 / pi*180 * J1PUPR - oldJ1step;
         j2pu = j.j2 / pi*180 * J2PUPR - oldJ2step;
        }
		long long int j3pu = j.j3          * J3PUPR - oldJ3step;
		long long int j4pu = j.j4 / pi*180 * J4PUPR - oldJ4step;
		long long int j5pu = j.j5 / pi*180 * J5PUPR - oldJ5step;
		long long int j6pu = j.j6 / pi*180 * J6PUPR - oldJ6step;
		long long int j7pu = c.c1 / pi*180 * J7PUPR - oldJ7step;
		long long int j8pu = c.c2 / pi*180 * J8PUPR - oldJ8step;



		unsigned long int contia = (unsigned long int)(sqrtf(j1pu * j1pu + j2pu * j2pu + \
															 j3pu * j3pu + j4pu * j4pu + \
															 j5pu * j5pu + j6pu * j6pu + \
															 j7pu * j7pu + j8pu * j8pu));
		           if(contia == 0)
		           {
		        	   cout << "����̲��ƶ�" <<endl;
		        	   return true;
		           }
				   long int maxspeed = 0;
				   while(1)
				   {
					   long int maxspeed1 = maxspeed;
					   maxspeed1 += 500;
					   double v1 = fabs((double)j1pu / (double)contia * maxspeed1);
					   double v2 = fabs((double)j2pu / (double)contia * maxspeed1);
					   double v3 = fabs((double)j3pu / (double)contia * maxspeed1);
					   double v4 = fabs((double)j4pu / (double)contia * maxspeed1);
					   double v5 = fabs((double)j5pu / (double)contia * maxspeed1);
					   double v6 = fabs((double)j6pu / (double)contia * maxspeed1);
					   double v7 = fabs((double)j7pu / (double)contia * maxspeed1);
					   double v8 = fabs((double)j8pu / (double)contia * maxspeed1);
					   if(v1 > Parameter::SingleAxisMaxJspeed[0] || v2 > Parameter::SingleAxisMaxJspeed[1] ||
						  v3 > Parameter::SingleAxisMaxJspeed[2] || v4 > Parameter::SingleAxisMaxJspeed[3] ||
						  v5 > Parameter::SingleAxisMaxJspeed[4] || v6 > Parameter::SingleAxisMaxJspeed[5] ||
				          v7 > Parameter::SingleAxisMaxJspeed[6] || v8 > Parameter::SingleAxisMaxJspeed[7]
						 )
						   break;
					   maxspeed =  maxspeed1;

				   }
				   //ͨ��������ٶ�����������ĺ��ٶ�


		        int  ADDTIM2 = 1;
		        int  VVVSSS2 = 1000;
		        long  int  VVVCCC2 = maxspeed *rate;
		        int  VADDTIMES2 = 1000;  //���ٲ���
		        double VVVAAA2= ((double)(VVVCCC2 - VVVSSS2) / (VADDTIMES2 - 1) / ADDTIM2);	//���ٶ�
		        double MINPU2	=	((VVVSSS2 + VVVCCC2) / 2 * ADDTIM2);
		        long int vddTable2[VADDTIMES2];
		        for(int i=0;i<VADDTIMES;i++)
		        	{
		        	vddTable2[i] = (long int)(6250000.0 / (VVVSSS2 + VVVAAA2 * ADDTIM2 * i));//��¼��ͬ���ٶ�
		        	}
		MOT->J1step = j1pu;
		MOT->J2step = j2pu;
		MOT->J3step = j3pu;
		MOT->J4step = j4pu;
		MOT->J5step = j5pu;
		MOT->J6step = j6pu;
		MOT->J7step = j7pu;
		MOT->J8step = j8pu;
		MOT->contia = contia;
		MOT->Tt = vddTable2[0];
		MOT->field.D3 = 'R';

		while(MOT->field.D3 == 'N')
		{
			 if(Variable::IsStop)
			 {
			  i = i - 1;
			  if(i < 1)
			  {
			    cout<<"���ƶ���ͣ"<<endl;
			    robot_stop();
			    return false;
			  }
			 }
			long long int j1has = MOT->J1step - oldJ1step;
			long long int j2has = MOT->J2step - oldJ2step;
			long long int j3has = MOT->J3step - oldJ3step;
			long long int j4has = MOT->J4step - oldJ4step;
			long long int j5has = MOT->J5step - oldJ5step;
			long long int j6has = MOT->J6step - oldJ6step;
			long long int j7has = MOT->J7step - oldJ7step;
			long long int j8has = MOT->J8step - oldJ8step;

			isContia = (unsigned long int)(sqrtf(j1has * j1has + j2has * j2has + \
												 j3has * j3has + j4has * j4has + \
												 j5has * j5has + j6has * j6has + \
												 j7has * j7has + j8has * j8has));


			if(contia <= 2 * MINPU2){	//���߶γ��Ȳ�����Ӽ��ٱ�Ҫ����ʱ

				if(isContia < contia / 2){
					a += VVVAAA2;	//���ٶ�
				}else{
					a -= VVVAAA2;
					if(a <= 0){
						a = 0;
					}
				}
				MOT->Tt = (unsigned long int)(6250000.0 / (VVVSSS2 + a));

			}else{						//��ȫ�Ӽ���

				if(isContia < MINPU2)
				{											//����
					MOT->Tt = vddTable2[i];
					i++;
					if(i >= VADDTIMES2)
					{
						i = VADDTIMES2 - 1;
					}
				}else if(isContia >= MINPU2 && isContia < (contia - MINPU2)){		//����
					MOT->Tt = vddTable2[i];
				}else{
					MOT->Tt = vddTable2[i];										//����
					i--;
					if(i <= 0){
						i = 0;
					}
				}
			}
			delay1ms();
		}
    return true;
}

void moto_runJoyAbs(Joint j, Coint c)
{
	int i = 0;
	unsigned long int isContia = 0;
	MOT->field.D3 = 'S';
	long int oldJ1step = MOT->J1step;
	long int oldJ2step = MOT->J2step;
	long int oldJ3step = MOT->J3step;
	long int oldJ4step = MOT->J4step;
	long int oldJ5step = MOT->J5step;
	long int oldJ6step = MOT->J6step;
	long int oldJ7step = MOT->J7step;
	long int oldJ8step = MOT->J8step;
	long long int j1pu;
	long long int j2pu;
	if(robotType == CoordRobot)
	{
	 j1pu = j.j1  * J1PUPR - oldJ1step;
	 j2pu = j.j2  * J2PUPR - oldJ2step;
	}
	else
	{
	  j1pu = j.j1 / pi*180 * J1PUPR - oldJ1step;
	  j2pu = j.j2 / pi*180 * J2PUPR - oldJ2step;
	}
	long long int j3pu = j.j3          * J3PUPR - oldJ3step;
	long long int j4pu = j.j4 / pi*180 * J4PUPR - oldJ4step;
	long long int j5pu = j.j5 / pi*180 * J5PUPR - oldJ5step;
	long long int j6pu = j.j6 / pi*180 * J6PUPR - oldJ6step;
	long long int j7pu = c.c1 / pi*180 * J7PUPR - oldJ7step;
	long long int j8pu = c.c2 / pi*180 * J8PUPR - oldJ8step;

	unsigned long int contia = (unsigned long int)(sqrtf(j1pu * j1pu + j2pu * j2pu + \
														 j3pu * j3pu + j4pu * j4pu + \
														 j5pu * j5pu + j6pu * j6pu + \
														 j7pu * j7pu + j8pu * j8pu));

	MOT->J1step = j1pu;
	MOT->J2step = j2pu;
	MOT->J3step = j3pu;
	MOT->J4step = j4pu;
	MOT->J5step = j5pu;
	MOT->J6step = j6pu;
	MOT->J7step = j7pu;
	MOT->J8step = j8pu;
	MOT->contia = contia;
	MOT->Tt = vddTable[0];
	MOT->field.D3 = 'R';
	while(MOT->field.D3 == 'N')
	{
		 if(Variable::IsStop)
		 break;
		long long int j1has = MOT->J1step - oldJ1step;
		long long int j2has = MOT->J2step - oldJ2step;
		long long int j3has = MOT->J3step - oldJ3step;
		long long int j4has = MOT->J4step - oldJ4step;
		long long int j5has = MOT->J5step - oldJ5step;
		long long int j6has = MOT->J6step - oldJ6step;
		long long int j7has = MOT->J7step - oldJ7step;
		long long int j8has = MOT->J8step - oldJ8step;

		isContia = (unsigned long int)(sqrtf(j1has * j1has + j2has * j2has + \
											 j3has * j3has + j4has * j4has + \
											 j5has * j5has + j6has * j6has + \
											 j7has * j7has + j8has * j8has));


		if(contia <= 2 * MINPU){	//���߶γ��Ȳ�����Ӽ��ٱ�Ҫ����ʱ

			if(isContia < contia / 2)
			{
				a1 += VVVAAA;	//���ٶ�
			}else
			{
				a1 -= VVVAAA;
				if(a1 <= 0)
				{
					a1 = 0;
				}
			}
			MOT->Tt = (unsigned long int)(6250000.0 / (VVVSSS + a1));

		}else{						//��ȫ�Ӽ���

			if(isContia < MINPU)
			{											//����
				MOT->Tt = vddTable[i];
				i++;
				if(i >= VADDTIMES)
				{
					i = VADDTIMES  - 1;
				}
			}else if(isContia >= MINPU && isContia < (contia - MINPU)){		//����
				MOT->Tt = vddTable[i];
			}else{
				MOT->Tt = vddTable[i];										//����
				i--;
				if(i <= 0)
				{
					i = 0;
				}
			}
		}
		delay1ms();
	}
}


//��ʱ�Ӽ���ͨ���߶��������ƣ��������Ǿ�����,�ǻ���ֵ�������Ǿ��ԵĻ���ֵ
void moto_runInterpolationAbs(Joint j, Coint c,float speed)
{
	//��ʼ׼��״̬
	MOT->field.D3 = 'S';
	//�Ȱѵ�ǰ�����嵱���������������ǵ�ǰ�����嵱��
	long int oldJ1step = MOT->J1step;
	long int oldJ2step = MOT->J2step;
	long int oldJ3step = MOT->J3step;
	long int oldJ4step = MOT->J4step;
	long int oldJ5step = MOT->J5step;
	long int oldJ6step = MOT->J6step;
	long int oldJ7step = MOT->J7step;
	long int oldJ8step = MOT->J8step;

    //������Ҫ�ߵ����嵱����
	long long int j1pu;
	long long int j2pu;
	if(robotType == CoordRobot)
	{
	 j1pu = j.j1  * J1PUPR - oldJ1step;
	 j2pu = j.j2  * J2PUPR - oldJ2step;
	}
	else
	{
	 j1pu = j.j1 / pi*180 * J1PUPR - oldJ1step;
	 j2pu = j.j2 / pi*180 * J2PUPR - oldJ2step;
	}
	long long int j3pu = j.j3          * J3PUPR - oldJ3step;
	long long int j4pu = j.j4 / pi*180 * J4PUPR - oldJ4step;
	long long int j5pu = j.j5 / pi*180 * J5PUPR - oldJ5step;
	long long int j6pu = j.j6 / pi*180 * J6PUPR - oldJ6step;
	long long int j7pu = c.c1 / pi*180 * J7PUPR - oldJ7step;
	long long int j8pu = c.c2 / pi*180 * J8PUPR - oldJ8step;


    //�ܵ���Ҫ�ܵ����嵱���ĺ�ֵ
	unsigned long int contia = (unsigned long int)(sqrtf(j1pu * j1pu + j2pu * j2pu + \
														 j3pu * j3pu + j4pu * j4pu + \
														 j5pu * j5pu + j6pu * j6pu + \
														 j7pu * j7pu + j8pu * j8pu));
    //���Ƹ�����ʾҪ�������������
	MOT->J1step = j1pu;
	MOT->J2step = j2pu;
	MOT->J3step = j3pu;
	MOT->J4step = j4pu;
	MOT->J5step = j5pu;
	MOT->J6step = j6pu;
	MOT->J7step = j7pu;
	MOT->J8step = j8pu;
	MOT->contia = contia;
	//Tt��ʾ����ʱ����Ҫ�����巢�꣬һ���ܵ�ʱ����
	MOT->Tt=(unsigned long int)(37500000/(speed*contia));
	MOT->field.D3 = 'R';
	while(MOT->field.D3 == 'N')
	{
		if(Variable::IsStop)
		break;
	}
}



void moto_runJoy(Joint j, Coint c)
{

	MOT->field.D3 = 'S';

	long int oldJ1step = MOT->J1step;
	long int oldJ2step = MOT->J2step;
	long int oldJ3step = MOT->J3step;
	long int oldJ4step = MOT->J4step;
	long int oldJ5step = MOT->J5step;
	long int oldJ6step = MOT->J6step;
	long int oldJ7step = MOT->J7step;
	long int oldJ8step = MOT->J8step;
	long long int j1pu;
	long long int j2pu;
	if(robotType == CoordRobot)
	{
	 j1pu = j.j1  * J1PUPR - oldJ1step;
	 j2pu = j.j2  * J2PUPR - oldJ2step;
	}
	else
	{
	 j1pu = j.j1 / pi*180  * J1PUPR - oldJ1step;
	 j2pu = j.j2 / pi*180  * J2PUPR - oldJ2step;
	}
	long long int j3pu = j.j3           * J3PUPR - oldJ3step;
	long long int j4pu = j.j4 / pi*180  * J4PUPR - oldJ4step;
	long long int j5pu = j.j5 / pi*180  * J5PUPR - oldJ5step;
	long long int j6pu = j.j6 / pi*180  * J6PUPR - oldJ6step;
	long long int j7pu = c.c1 / pi*180  * J7PUPR - oldJ7step;
	long long int j8pu = c.c2 / pi*180  * J8PUPR - oldJ8step;



	unsigned long int contia = (unsigned long int)(sqrtf(j1pu * j1pu + j2pu * j2pu + \
														 j3pu * j3pu + j4pu * j4pu + \
														 j5pu * j5pu + j6pu * j6pu + \
														 j7pu * j7pu + j8pu * j8pu));


	MOT->J1step = j1pu;
	MOT->J2step = j2pu;
	MOT->J3step = j3pu;
	MOT->J4step = j4pu;
	MOT->J5step = j5pu;
	MOT->J6step = j6pu;
	MOT->J7step = j7pu;
	MOT->J8step = j8pu;
	MOT->contia = contia;
	MOT->field.D3 = 'R';

	while(MOT->field.D3 == 'N'&&!Variable::IsStop)
	{

		MOT->Tt = (unsigned long int)(6250000.0 / (nowSpeed));

	}

}


//ָ�����ƶ�ָ���Ƕ�
//��ڲ�����	ch		ָ���ᣬȡֵ��Χ1,2,3,4,5,6,7,8
//			angle	��ת�Ƕ�,��λ��(�����ᵥλΪmm)
void moto_SettingJ(char ch, double angle,double speed)
{

	Joint j;
	Coint c;

	j.j1 = 0; j.j2 = 0; j.j3 = 0; j.j4 = 0;
	j.j5 = 0; j.j6 = 0; c.c1 = 0; c.c2 = 0;

//���������л�
	switch(ch)
{
		case 1:
		{
			if(robotType == CoordRobot)
			j.j1 = angle;
			else
			j.j1 = angle / 180.0 * pi;
			break;
		}
		case 2:
		{
			if(robotType == CoordRobot)
			j.j2 = angle ;
			else
			j.j2 = angle / 180.0 * pi;
			break;
		}
		case 3: j.j3 = angle ; break;
		case 4: j.j4 = angle / 180.0 * pi; break;
		case 5: j.j5 = angle / 180.0 * pi; break;
		case 6: j.j6 = angle / 180.0 * pi; break;
		case 7: c.c1 = angle / 180.0 * pi; break;
		case 8: c.c2 = angle / 180.0 * pi; break;
		default:break;
	}

	moto_runJ(j, c,speed);//��תһ���Ƕ���
}

//ָ�������е���λλ��
//    ch:ָ����
//speedL:�ٶȵȼ���[0,VADDTIMES]���ٶȵȼ�ԽС������λ�ٽ��Խ��
void J1RunToLimit(char ch, int speedL)        //��    �ٶ�
{

	int i = 0;
	bool lastLMT, nowLMT;
	int speedDownFlag = 0;

	long int j1pu = 0,j2pu = 0,j3pu = 0,j4pu = 0;
	long int j5pu = 0,j6pu = 0,j7pu = 0,j8pu = 0;      //8��������

	if(speedL >= VADDTIMES)
	{     //���ܳ��������ٶ�
		speedL = VADDTIMES;//��������������Ķ���
	}

	MOT->field.D3 = 'S';         //������mot����һ����ַ�����ƵĹ���
	switch(ch)
	{     //���������ж�          �Ǽ���λ�þͳ���180   ���Ǽ���
		case 1:
			    if(Variable::BigArmDirectionChange)
			    j1pu = (J1LMT) ? -180 * J1PUPR :  180 * J1PUPR;
			    else
			    j1pu = (J1LMT) ? 180 * J1PUPR : - 180 * J1PUPR;//��ת180��  ��ô������
				lastLMT = (J1LMT) ? true : false;    //
				MOT->contia = abs(j1pu);   //ȡģȥ�˶�
				break;
		case 2:
			    if(Variable::SmallArmDirectionChange)
			    j2pu = (J2LMT) ? -180 * J2PUPR : 180 * J2PUPR;
			    else
			    j2pu = (J2LMT) ? 180 * J2PUPR : - 180 * J2PUPR;  //���Ǽ���λ��
				lastLMT = (J2LMT) ? true : false;
				MOT->contia = abs(j2pu);
				break;
		case 3:  if(Variable::UpDownAxisDirectionChange)
			     j3pu = (J3LMT) ? -2000 * J3PUPR :  2000 * J3PUPR;
		         else
			     j3pu = (J3LMT) ? 2000 * J3PUPR : - 2000 * J3PUPR;
				lastLMT = (J3LMT) ? true : false;
				MOT->contia = abs(j3pu);
				break;
		case 4:
			    if(Variable::RotorAxisDirectionChange)
			    j4pu = (J4LMT) ? -180 * J4PUPR :  180 * J4PUPR;
			    else
			    j4pu = (J4LMT) ? 180 * J4PUPR :  -180 * J4PUPR;
				lastLMT = (J4LMT) ? true : false;
				MOT->contia = abs(j4pu);
				//¥������ʽ����ı䷽��
				break;
		case 5:
			    if(Variable::SwingAxisDirectionChange)
			    j5pu = (J5LMT) ? 180 * J5PUPR : -180 * J5PUPR;
			    else
			    j5pu = (J5LMT) ? -180 * J5PUPR : 180 * J5PUPR;
				lastLMT = (J5LMT) ? true : false;
				MOT->contia = abs(j5pu);
				break;
		case 6:
				break;
		case 7:
			if(Variable::ModifiedGear1DirectionChange)
			j7pu = (J7LMT) ? 180 * J7PUPR : -180 * J7PUPR;
			else
			j7pu = (J7LMT) ? -180 * J7PUPR : 180 * J7PUPR;
			lastLMT = (J7LMT) ? true : false;
			MOT->contia = abs(j7pu);
			break;

		case 8:
			if(Variable::ModifiedGear2DirectionChange)
			j8pu = (J8LMT) ? 180 * J8PUPR : -180 * J8PUPR;
			else
			j8pu = (J8LMT) ? -180 * J8PUPR : 180 * J8PUPR;
			lastLMT = (J8LMT) ? true : false;
			MOT->contia = abs(j8pu);
			break;
		default:break;
	}
	MOT->J1step = j1pu;
	MOT->J2step = j2pu;
	MOT->J3step = j3pu;
	MOT->J4step = j4pu;
	MOT->J5step = j5pu;
	MOT->J6step = j6pu;
	MOT->J7step = j7pu;
	MOT->J8step = j8pu;
	MOT->field.D3 = 'R';

	while(MOT->field.D3 == 'N'){
		 if(Variable::IsStop)    //��ͣ
		 {
		  robot_stop();
		  return;
		 }
		delay1ms();
		switch(ch){
			case 1: nowLMT = (J1LMT) ? true : false; break;
			case 2: nowLMT = (J2LMT) ? true : false; break;
			case 3: nowLMT = (J3LMT) ? true : false; break;
			case 4: nowLMT = (J4LMT) ? true : false; break;
			case 5: nowLMT = (J5LMT) ? true : false; break;
			case 6: nowLMT = (J6LMT) ? true : false; break;
			case 7: nowLMT = (J7LMT) ? true : false; break;
			case 8: nowLMT = (J8LMT) ? true : false; break;
			default:break;
		}

		//����״̬��һ�£���ʾ��������λ������λ �� ��������λ����λ      ״̬����һ�µ�
		if(lastLMT ^ nowLMT){
			speedDownFlag ++;
		}

		if(speedDownFlag > 10){
			i--;                                                            //1��������򲻶�
			if(i <= 0){
				MOT->field.D3 = 'S';
				return;
			}
		}else{
			i++;                   //��1000���������ѭ����ȥ
			if(i >= speedL){
				i = speedL - 1;
			}
		}
		MOT->Tt = vddTable[i];
		if(Variable::IsStop)    //��ͣ
		{
		 robot_stop();
         return;
		}
	}
}


void moto_init(void)
{

	for(int i=1;i<9;i++)
	{
		clcPUs(i);
	}

	robot_stop();
}

void moto_XYZclear(void)
{

	for(int i=1;i<4;i++)
	{
		clcPUs(i);
	}

	robot_stop();


}

void robot_stop(void)
{
	MOT->field.D3 = 'S';
	usleep(50000);
	MOT->J1step = 0;
	MOT->J2step = 0;
	MOT->J3step = 0;
	MOT->J4step = 0;
	MOT->J5step = 0;
	MOT->J6step = 0;
	MOT->J7step = 0;
	MOT->J8step = 0;
	MOT->contia = 0;
	MOT->Tt = 0xFFFF;
    usleep(50000);
	MOT->field.D3 = 'R';
}


/*
 * RobotBack.h
 *
 *  Created on: 2016��11��16��
 *      Author: Administrator
 */

#ifndef SRC_ROBOTBACK_H_
#define SRC_ROBOTBACK_H_
#include "common.h"
class Robot_Back
{
public:
	Robot_Back();
	virtual ~Robot_Back();
private:
	double speed;//�����ٶ�
	int AxisNumber;//�������
	double distance;//����ƫ��
	bool runing;    //��־λ���Ƿ��˶���
    Joint j={0,0,0,0,0,0};
   	Coint c={0,0};

public:

	//��������
    void setData(int AxisNumber,double speed,double distance);
    //�������
    void SingleAxisBack();
    //�����˶�
    bool run();

};

#endif /* SRC_ROBOTBACK_H_ */

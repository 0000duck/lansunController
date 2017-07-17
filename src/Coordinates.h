/*
 * Coordinates.h
 *
 *  Created on: 2016��11��17��
 *      Author: Administrator
 */

#ifndef SRC_COORDINATES_H_
#define SRC_COORDINATES_H_
//XYZRPW1W2
#include "moto.h"
#include "common.h"
//������
class Coordinates
{
public:
	Coordinates();
	virtual ~Coordinates();//���û���ָ��ָ��̳���Ķ���ʱ����������麯������ɾ������ָ����ֻ������������󣬴Ӷ�����ڴ��й¶


private:
    Joint curJ;   //Ŀǰ��
    Coint curC;
    Joint lastcurJ;   //��һ�ε�
    Coint lastcurC;
    ArrayXd xyzrpw;   //��һ����������
    void getPosition();
public:
    void run();
};

#endif /* SRC_COORDINATES_H_ */

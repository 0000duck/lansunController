/*
 * Parameter.h
 *
 *  Created on: 2016��11��17��
 *      Author: Administrator
 */

#ifndef SRC_PARAMETER_H_
#define SRC_PARAMETER_H_
class Parameter         //�ṹ����
	{
public:
	    Parameter();
		virtual ~Parameter();
		static   long int  SingleAxisMaxJspeed[8];  //�����ٶ�  pu/s   ����ÿ��
		static   long int  JoyMaxJspeed[8];
private:
		double bigArmLength;       //��۳���
		double smallArmLength;     //С�۳���
		double weldingTorchLength;   //��ǹ����
		double reservedLength;      //Ԥ������
public:
	    void setData(double bigArmLength,double smallArmLength,double weldingTorchLength,double reservedLength);
	};
#endif /* SRC_PARAMETER_H_ */

/*
 * IOparameter.h
 *
 *  Created on: 2016��11��26��
 *      Author: ������
 */

#ifndef LANSUNV2_0_SRC_IOPARAMETER_H_
#define LANSUNV2_0_SRC_IOPARAMETER_H_
class IOparameter {
public:
	IOparameter();
	virtual ~IOparameter();
	void run();
	int IOdata;
	int lastIOdata;
private:
public:
	void setData(int outputIO); //��������

};

#endif /* LANSUNV2_0_SRC_IOPARAMETER_H_ */

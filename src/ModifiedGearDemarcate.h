/*
 * ModifiedGearDemarcate.h
 *
 *  Created on: 2017��6��16��
 *      Author: deng
 */

#ifndef MODIFIEDGEARDEMARCATE_H_
#define MODIFIEDGEARDEMARCATE_H_
#include "common.h"
#include "DlyCommon.h"
//#define 	ACCURACY		0.0000001
#define 	ACCURACY		0.01
class ModifiedGearDemarcate {
public:
	ModifiedGearDemarcate();
	virtual ~ModifiedGearDemarcate();
	Matrix4d m_POS = Matrix4d::Identity();
	MJCoint	CurrentMJCoint;

	// ��ȡ��ǰJC����
	MJCoint getCurrentJCoint();
	void implementGetPOSMatrix();
	void setData(PointType);
	void setData(const Matrix4d &pos);
	bool run();
	char POS_BOOL;

private:
	void Iscoplane();
	PointType pointType;
	bool running;
	Matrix4d P1, P2, P3, P4, P5, P6;
};

#endif /* MODIFIEDGEARDEMARCATE_H_ */

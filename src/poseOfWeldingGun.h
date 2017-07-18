/*
 *  poseOfWeldingGun.h
 *
 *  Created on: 2016��11��21��
 *      Author: Bing_yao
 */
/*
 * ���ļ�  ���  �ɵײ��v1.0�汾 ����Ϊ�ײ�� V2.0�汾
 * ��ǹ �궨ģ�� ���� �����Ĵ���
 * ע���ײ��v1.0 �������λ�� Ϊ��ǹĩ��
 * 	  �ײ��v2.0     �������λ�� Ϊ����ؽ���ĩ��
 * */

#ifndef LANSUNV2_0_SRC_POSEOFWELDINGGUN_H_
#define LANSUNV2_0_SRC_POSEOFWELDINGGUN_H_

#include "scaralib.h"
#include "Torchcalibration.h"

extern Matrix4d CHPA;
// ���⣺ ���� ��ǹ�궨ֵ
Matrix4d fksolution(Joint j);
// ��⣺ ���� ��ǹ�궨ֵ
Matrix4d fksolutionPose(Joint j);//�궨���
Joint iksolution(Matrix4d Matrix, bool choose_j2);
// ��װ��⣺ ���� ��ǹ�궨ֵ
Joint NewPositionJointssolution(Matrix4d Matrix);
// ���⣺ ���� ��ǹ�궨ֵ
Matrix4d fksolutionBar(Joint j);
// ��⣺ ���� ��ǹ�궨ֵ
Joint iksolutionBar(Matrix4d Matrix, bool choose_j2);
// ��װ��⣺ ���� ��ǹ�궨ֵ
Joint NewPositionJointssolutionBar(Matrix4d Matrix);
// λ�� ��ת����
Matrix4d transl(Vector3d p);
// ��λ��ԭ��
void getCHPosAttitudesolution(Matrix4d Moe1, Matrix4d Moe2, Matrix4d Moe3, Matrix4d Moe4, Matrix4d Moe5, Matrix4d Moe6);

list<MJCoint> PositionAttitudeLinearInterpolationsolution(Matrix4d A, Matrix4d B);

#endif /* LANSUNV2_0_SRC_POSEOFWELDINGGUN_H_ */

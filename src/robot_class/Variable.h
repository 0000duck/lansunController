/*
 * Variable.h
 *
 *  Created on: 2017��5��3��
 *      Author: deng
 */

#ifndef ROBOT_CLASS_VARIABLE_H_
#define ROBOT_CLASS_VARIABLE_H_
class Variable
{
public:
	static bool IsStop;                         //��ͣ
	static bool IsSingleAxisStop;
	static bool RotorAxisDirectionChange;       //���㷽��ı�
	static bool SwingAxisDirectionChange;       //��ת�᷽��ı�
	static bool ModifiedGear1DirectionChange;
	static bool ModifiedGear2DirectionChange;
	static bool BigArmDirectionChange;
	static bool SmallArmDirectionChange;
	static bool UpDownAxisDirectionChange;       //��ת�᷽��ı�
	static bool Islimit;                        //�Ƿ�����λλ��

};

#endif /* ROBOT_CLASS_VARIABLE_H_ */

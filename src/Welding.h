/*

 3 * Welding.h
 *
 *  Created on: 2016��11��21��
 *      Author: Administrator
 */

#ifndef SRC_WELDING_H_
#define SRC_WELDING_H_
#include "common.h"
#include "moto.h"
#include "Element.h"
#include "DlyCommon.h"
#include "robot_class/ArcParser.h"
#include <list>
#include <vector>
class Welding
{
public:
	Welding();
	virtual ~Welding();
	bool runing;
	bool backruning;
private:
	bool arcStrick(int currentLineNum, double arcStrictime);
	void arcQuench(int currentLineNum);  //���й�����Ϩ��

public:
	void move();
	void run();
	void stopStaticChange();
	double rate;  //��ǰ����
	bool IsPreheatingFished;
	bool IsFireMode;    //�����״̬
	bool IsFireFinishedTime;  //�Ƿ�������ʱʱ��
	/*************************************�ؽ�ʽ�����˸���*******************************************/
	vector<Element> graph;   //ͼ������
	int graphInex;                          //�ߵ����
	long int interpolationIndex;            //�岹���
	bool IsParserSuccess;    //�Ƿ�����ɹ�
	bool moveFinished;
	bool backFinished;
	bool IsFireLinecPause;
	bool IsModeChangeArcStrick;
	arcStrickObject lastArcStrickObject;
	//ֱ�߽��յ�
	void receiveLinePoints(const ArrayXd &p1, const Coint& c1,const ArrayXd &p2, const Coint& c2, double speed, int num,bool IsFire);
	//Բ�����յ�
	void receiveArcPoints(const ArrayXd &p1, const Coint& c1, const ArrayXd &p,const Coint& c, const ArrayXd &p2, const Coint& c2, double speed,int num);
	//Բ��������
	void receiveCirclePoints(const ArrayXd &p1, const Coint& c1,const ArrayXd &p, const Coint& c, const ArrayXd &p2, const Coint& c2,double speed, int num);
	void receiveCircle1Points(const ArrayXd &p1, const Coint& c1,const ArrayXd &p, const Coint& c, const ArrayXd &p2, const Coint& c2,double speed, int num);
	void receiveArcStrickData(double delay, int num);   //����������
	void receiveArcQuenchData(int num);                //����Ϩ������
	bool getInterpolations();                          //�õ��岹��
	void init();
	void back();  //����
	double lastSpeed;

};

#endif /* SRC_WELDING_H_ */

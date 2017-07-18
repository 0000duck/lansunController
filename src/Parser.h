/*
 * Parser.h
 *
 *  Created on: 2016��11��15��
 *      Author: Administrator
 */

#ifndef SRC_PARSER_H_
#define SRC_PARSER_H_
#include <iostream>
using namespace std;
#include <vector>
#include "FixedLengthMoving.h"
#include "RobotBack.h"
#include "Coordinates.h"
#include "RoctorBar.h"
#include "Welding.h"
#include "Parameter.h"
#include "IOparameter.h"
#include "Torchcalibration.h"
#include "ModifiedGearDemarcate.h"
#include "MD5.h"
#include "moto.h"
#include <string>    //�����ֿ��Ե����ٶȿ���
class Parser
{
public:
	Parser();
	virtual ~Parser();
	//�ܵĽ�������
	void DateParser(const string&);
	void run();
	bool  back_finished;
	bool  IsConnnect;
	bool runing;//��־λ
private:
	//���ݽ���
    void Datereceive();
    //У�麯��
    bool check();
    //cmd��������
    void cmdparser();

    string sdata;

    bool IsClear;
    bool IsCutting;//�Ƿ��ܹ��и�
    vector<double> parserdata;        //����
    string cmd;
    string checkValue;
    string checkstr;
    ArrayXd initXyzrpw;   //����������ֵ

    /***********�ؽ�ʽ��������***************/
    void parserReceviedCoordPoints();      //���ս�����λ���´��ĵ�λ����

public:
    FixedLengthMoving  fixposmove;    //�����ƶ�
    Robot_Back       robotback;          //����
    Coordinates      coord;  //����
    IOparameter   ioparameter;//IO����
    RoctorBar        roctorbar;   //ҡ��
    Welding          welding;     //����
    Torchcalibration torchcalibration;
    ModifiedGearDemarcate modifiedGearDemarcate; //��λ����
    bool  IsSend;    //�Ƿ��������´���
    void changeRPW(ArrayXd &xyzrpw);
};

#endif /* SRC_PARSER_H_ */

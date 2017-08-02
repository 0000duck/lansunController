/*
 * common.h
 *
 *  Created on: 2016��5��17��
 *      Author: Yin
 */

#ifndef LANSUNV2_0_SRC_COMMON_H_
#define LANSUNV2_0_SRC_COMMON_H_


using namespace std;

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sstream>

#include <time.h>

#include <math.h>

#include <list>
#include <string>
#include "hwlib.h"
#include "hps_0.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include <stdbool.h>

#include "scaralib.h"
#include "poseOfWeldingGun.h"
#include "Torchcalibration.h"
#include "md5.h"
#include "ProjectUpdate.h"
#include "robot_class/Variable.h"      //������

#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )
enum RobotType
{
    JointRobot = 0,
	CoordRobot = 1
};
//
typedef struct
{
    unsigned long int DATA;
    unsigned long int DIRECTION;
    unsigned long int INTERRUPT_MASK;
    unsigned long int EDGE_CAPTURE;
}PIO_STR;


typedef struct
{   //��� ռ4���ֽ�  ������    �ܹ������̷߳���
    volatile long int J1step : 32;		// W : �����Ӧ��������			R : J1PUCOUNT
    volatile long int J2step : 32;		// W : �����Ӧ��������			R : J2PUCOUNT
    volatile long int J3step : 32;		// W : �����Ӧ��������			R : J3PUCOUNT
    volatile long int J4step : 32;		// W : �����Ӧ��������			R : J4PUCOUNT
    volatile long int J5step : 32;		// W : �����Ӧ��������			R : J5PUCOUNT
    volatile long int J6step : 32;		// W : �����Ӧ��������			R : J6PUCOUNT
    volatile long int J7step : 32;		// W : �����Ӧ��������			R : J7PUCOUNT
    volatile long int J8step : 32;		// W : �����Ӧ��������			R : J8PUCOUNT

    volatile unsigned long int contia : 32;		// W : ����					R : Message
    volatile unsigned long int     Tt : 32;		// W : ��ʱ������ı��ٶ�	R : Message

    union{
        volatile unsigned long int CMD : 32;		// W : ����ָ��  ��C��:�������ֵ����  ��R��:��ʼ����	R : ��N��:�������У���Y��:�������
    	struct{
    		volatile unsigned char D0 : 8;
    		volatile unsigned char D1 : 8;
    		volatile unsigned char D2 : 8;
    		volatile unsigned char D3 : 8;
    	}field;//��ʵ���൱����һ�������ֵ
    };

}MOT_SIR;    //MOT  ���ö��ٸ��ֽ���


typedef union{
    unsigned long int CMD;
	struct{
    	volatile unsigned short int data 		: 10;	//ģ��������0-0x3FF ��Ӧ 0-10V
    	volatile unsigned short int spd 		: 1;	//speed control bit:	1--fast mode  0--slow mode
    	volatile unsigned short int pwr 		: 1;	//power control bit: 	1--power down 0--power operation
    	volatile unsigned short int ch 			: 2;	//AVO channel selection:0-3
    	volatile unsigned short int start 		: 2;	//start control: 		2'b11--start  other--stop
	}field;
}AVO_SIR;


#define ZERO_THRESH 	(0.000001)

////==========================������嵱�� pu/�� �� pu/mm ��������汾
//#define J1PUPR		(3746.0317460317460317460317460317)
//#define J2PUPR		( 666.6666666666666666666666666667)
//#define J3PUPR		( 954.9296585513720146133025802351)
//#define J4PUPR		(  83.3333333333333333333333333333)
//#define J5PUPR		(1777.7777777777777777777777777778)
//#define J6PUPR		( 500.0000000000000000000000000000)
//#define J7PUPR		( 500.0000000000000000000000000000)
//#define J8PUPR		( 500.0000000000000000000000000000)


//==========================������嵱�� pu/�� �� pu/mm �ŷ��汾      //������嵱��������
//#define J1PUPR			(3746.0317460317460317460317460317)
//#define J2PUPR			(2666.6666666666666666666666666667)
//#define J3PUPR			(954.92965855137201461330258023509)
//#define J4PUPR			(177.77777777777777777777777777778)
//#define J5PUPR			(1777.7777777777777777777777777778)
//#define J6PUPR			( 500.0000000000000000000000000000)
//#define J7PUPR			(7111.1111111111111111111111111111)
//#define J8PUPR			(3333.3333333333333333333333333333)


////==========================������嵱�� pu/�� �� pu/mm ���԰汾
//#define J1PUPR			(374.60317460317460317460317460317)
//#define J2PUPR			(266.66666666666666666666666666667)
//#define J3PUPR			(95.492965855137201461330258023509)
//#define J4PUPR			(17.777777777777777777777777777778)
//#define J5PUPR			(177.77777777777777777777777777778)
//#define J6PUPR			( 50.00000000000000000000000000000)
//#define J7PUPR			(711.11111111111111111111111111111)
//#define J8PUPR			(333.33333333333333333333333333333)

//�и����嵱��
//#define J1PUPR			(2547.7707006369426751592356687898)
//#define J2PUPR			(1819.0913638637500568466051207422)
//#define J3PUPR			(1591.0797703753675394269567099016)
//#define J4PUPR			(1428.5714285714285714285714285714)
//#define J5PUPR			(1123.5955056179775280898876404494)
//#define J6PUPR			( 500.0000000000000000000000000000)
//#define J7PUPR			(7111.1111111111111111111111111111)
//#define J8PUPR			(3333.3333333333333333333333333333)
////С������
//#define J1PUPR			(200.78431372549019607843137254902*5*100/105)
//#define J2PUPR			(89.875732569540895493016713433891)
//#define J3PUPR			(954.92965855137201461330258023509)
//#define J4PUPR			(133.33333333333333333333333333333)
//#define J5PUPR			(133.33333333333333333333333333333)
//#define J6PUPR			( 500.0000000000000000000000000000)
//#define J7PUPR			(7111.1111111111111111111111111111)
//#define J8PUPR			(3333.3333333333333333333333333333)

//#define J1PUPR			(3746.0317460317460317460317460317)
//#define J2PUPR			(2666.6666666666666666666666666667*18/21)
//#define J3PUPR			(10000*10/20/pi)
//#define J4PUPR			(277.7777777777778*4)
//#define J5PUPR			(277.7777777777778*4)
//#define J6PUPR			( 500.0000000000000000000000000000)
//#define J7PUPR			(7111.1111111111111111111111111111)
//#define J8PUPR			(3333.3333333333333333333333333333)

#define PORT 1000
#define BUFFER_SIZE 	1024
extern sockaddr_in udpaddr;




//==========================================�ؽ����λ�������Ͻṹ��
typedef struct
{
	float j1; float j2; double j3; float j4;
	float j5; float j6; float j7; float j8;

}getJoint;

extern double J1PUPR;
extern double J2PUPR;
extern double J3PUPR;
extern double J4PUPR;
extern double J5PUPR;
extern double J6PUPR;
extern double J7PUPR;
extern double J8PUPR;
extern Joint lastJ;
extern bool lastJIsinit;
extern RobotType robotType;
extern volatile unsigned long h2p_lw_led_addr;
extern volatile unsigned long h2p_lw_key_addr;
extern volatile unsigned long h2p_lw_mot_addr;
extern volatile unsigned long h2p_lw_IOM_addr;
extern volatile unsigned long h2p_lw_AVO_addr;

#define MOT 	((MOT_SIR *)h2p_lw_mot_addr)  //��ʵ����һ����ַ  ����SOC�ĵ�ַ
#define IOM		((PIO_STR *)h2p_lw_IOM_addr)
#define AVO		((AVO_SIR *)h2p_lw_AVO_addr)

//��λ�ӿ�
#define J1LMT	((~(IOM->DATA) & 0x00000100))             //��λ�ṹ     soc io�ڵ������� 8λ  16����    32λ 4�ֽ�
#define J2LMT	((~(IOM->DATA) & 0x00000200))
#define J3LMT	((~(IOM->DATA) & 0x00000400))
#define J4LMT	((~(IOM->DATA) & 0x00000800))
#define J5LMT	((~(IOM->DATA) & 0x00001000))
#define J6LMT	((~(IOM->DATA) & 0x00002000))
#define J7LMT	((~(IOM->DATA) & 0x00004000))
#define J8LMT	((~(IOM->DATA) & 0x00008000))

#define XLMT	((~(IOM->DATA) & 0X00010000))
#define YLMT	((~(IOM->DATA) & 0X00020000))
#define ZLMT	((~(IOM->DATA) & 0X00040000))
#define EmergencyIO	((~(IOM->DATA) & 0x00080000))

//===========================�Ӽ��ٶ���
//===========================�Ӽ��ٶ���
#define VVVSSS		( 1000)			//���ٶ�
#define VVVCCC		(24000*3)			//ĩ�ٶ�
#define ADDTIM		(    1)			//����ʱ��
#define VADDTIMES 	( 1000)			//���ٲ���                    ���ٵĲ���
//#define VVVCCC1		(24000*3)			//ĩ�ٶ�       ҡ�˺����ٶ�
#define VVVAAA		((double)(VVVCCC - VVVSSS) / (VADDTIMES - 1) / ADDTIM)	//���ٶ�
#define MINPU		((VVVSSS + VVVCCC) / 2 * ADDTIM)
//���ٳ���
//#define MINPU1		((VVVSSS + VVVCCC1) / 2 * ADDTIM)
//#define VADDTIMES1 	( 1000)			//���ٲ���                    ���ٵĲ���
//#define VVVAAA1		((double)(VVVCCC1 - VVVSSS) / (VADDTIMES1 - 1) / ADDTIM)	//���ٶ�



//�����ǿ��Ƶ�����
/**********************************/
//#define ADDTIM2		(    1)
//#define VVVSSS2		( 3000)
//#define VVVCCC2		(24000 * 5)
//#define VADDTIMES2 	( 1000)	  //���ٲ���
//#define VVVAAA2	((double)(VVVCCC2 - VVVSSS2) / (VADDTIMES2 - 1) / ADDTIM2)	//���ٶ�
//#define MINPU2		((VVVSSS2 + VVVCCC2) / 2 * ADDTIM2)
//extern unsigned long int vddTable2[VADDTIMES2];	//���ٸı��б�

/************************************/


extern unsigned long int vddTable[VADDTIMES];	//���ٸı��б�
extern unsigned long int nowSpeed;

//===========================�ֱ����ƶ���
//A B X Y LB RB BACK START JLB JRB left right down up
#define JOY_A				( 0)
#define JOY_B				( 1)
#define JOY_X				( 2)
#define JOY_Y				( 3)
#define JOY_LB				( 4)
#define JOY_RB				( 5)
#define JOY_BACK			( 6)
#define JOY_START			( 7)
#define JOY_JLB				( 8)
#define JOY_JRB				( 9)
#define JOY_left			(10)
#define JOY_right			(11)
#define JOY_down			(12)
#define JOY_up				(13)

extern  bool joyButtons[14];        //14����ť
//LT RT LUDT LLRT RUDT RLRT;
extern double  joySticks[ 5];

extern Joint currentJ, targetJ;

extern Coint targetC, currentC;

extern double interpolationSpeed;

// debug
extern Matrix4d TCFmatrix;

extern bool TESKVOID;
extern ofstream writefile;


#endif /* LANSUNV2_0_SRC_COMMON_H_ */

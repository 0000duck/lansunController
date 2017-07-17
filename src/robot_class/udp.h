/*
 * udp.h
 *
 *  Created on: 2017��5��9��
 *      Author: deng
 */

#ifndef ROBOT_CLASS_UDP_H_
#define ROBOT_CLASS_UDP_H_
//udp��Ķ���
#define UdpPort 8080
#include <string>
#include <string.h>
#include <iostream>
#include <netinet/in.h>
using namespace std;
class udp
{
public:
	udp();
	virtual ~udp();
	static string udpAddress;
	static int udpsocket;          //�׽���
	static int addr_len;           //����
	static bool IsOpenUdp;         //�Ƿ��udpͨ��
	static bool IsBackCancle;      //�Ƿ����ȡ�� ���ڷ���
	static sockaddr_in udpaddr;    //udp���׽���
	static void initUdpSocket();   //��ʼ��udp
	static void sendProgramData(const string &str);//���ͱ���
    static void closeUdp();       //�ر�udp;
};

#endif /* ROBOT_CLASS_UDP_H_ */

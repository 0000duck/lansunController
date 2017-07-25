/*
 * udp.cpp
 *
 *  Created on: 2017��5��9��
 *      Author: deng
 */

#include "udp.h"
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
 string udp::udpAddress = "192.168.1.101";
 int  udp::udpsocket = 0;              //�׽���
 int  udp::addr_len = sizeof(sockaddr_in);  //����
 bool udp::IsOpenUdp = false;       //�Ƿ��udpͨ��
 bool udp::IsBackCancle = false;    //�Ƿ����ȡ�� ���ڷ���
 sockaddr_in udp::udpaddr;    //udp���׽���
udp::udp()
{
  // TODO Auto-generated constructor stub
}

udp::~udp()
{
	// TODO Auto-generated destructor stub
}

void udp::initUdpSocket()
 {
	 if(!IsOpenUdp)
		 return;
	 /* ����socket*/
	 if((udpsocket = socket(AF_INET,SOCK_DGRAM,0))<0)   //��ʼ��socket
	 {
		 perror("socket");
		 exit(1);
	 }
	 else
	 {
		 cout << "udp��ʼ���ɹ�" << endl;
	 }
	 bzero(&udpaddr,sizeof(udpaddr));
	 udpaddr.sin_family = AF_INET;
	 udpaddr.sin_port = htons(UdpPort);
	 udpaddr.sin_addr.s_addr = inet_addr(udpAddress.c_str());
//	 udpaddr.sin_addr.s_addr = htonl(INADDR_ANY);
//	 if (bind(udp, (struct sockaddr *)&udpaddr, sizeof(udpaddr)) < 0)
//	 {
//		 perror("bind");
//		 exit(1);
//	 }
//	 else
//	 {
//		 cout << "udp�󶨳ɹ�" << endl;
//	 }
//	     char buff[512];
//	     struct sockaddr_in clientAddr;
//
//	     unsigned len = sizeof(clientAddr);
//	     int n = recvfrom(udp, buff, 511, 0, (struct sockaddr*)&clientAddr, &len);
//		if (n>0)
//		{
//			buff[n] = 0;
//			printf("upd���յ������ǣ�%s %u says: %s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buff);
//			n = sendto(udp, buff, n, 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
//			if (n < 0)
//			{
//				perror("udp����ʧ��1");
//
//			}
//		}
//		else
//		{
//			    perror("udp����ʧ��2");
//		}
 }

 void udp::sendProgramData(const string &str)
 {
	 if(IsOpenUdp)
	 sendto(udpsocket,str.c_str(),str.size(),0,(sockaddr *)&udpaddr,addr_len);
 }
 void udp::closeUdp()
 {
	 if(udp::IsOpenUdp)
     close(udpsocket);
 }

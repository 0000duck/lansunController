/*
 *  main.cpp
 *
 *  Created on: 2016��5��16��
 *      Author: Yin
 */

//AVO->field.ch=0;  ָ��ͨ��0-3
//AVO->field.data=0x123      0-0x3FF 1-10v��ѹ����
#include "hps_0.h"                    //hps_0.h
#include "common.h"                   //
#include "moto.h"                      //
#include "DlyCommon.h"
using namespace std;
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include "Parser.h"
#include "Parameter.h"
#include "robot_class/udp.h"
// debug
#include "FixedLengthMoving.h"
volatile unsigned long h2p_lw_led_addr;
volatile unsigned long h2p_lw_key_addr;      //��
volatile unsigned long h2p_lw_mot_addr;
volatile unsigned long h2p_lw_IOM_addr;     //IO��
volatile unsigned long h2p_lw_AVO_addr;    //������ѹ��
//==========================ÿ1mm��ɢ�����
//extern float RESOLUTION;
extern float RESOLUTION;
double J1PUPR = 1000;
double J2PUPR = 1000;
double J3PUPR = 1000;
double J4PUPR = 1000;
double J5PUPR = 1000;
double J6PUPR = 1000;
double  J7PUPR = 1000;
double  J8PUPR = 1000;
Matrix4d TCFmatrix;
pthread_mutex_t mutex;
Parser parser;  //������
Parameter structParameter;      //�ṹ����
string lastcmd;   //������һ��ָ��
Coint currentC = { 0, 0 };
int conn;   //�˿ں�
sem_t sem;                //�ź���
char sendbuf[BUFFER_SIZE];        //���ͻ���
char recvbuf[BUFFER_SIZE];        //���ܻ���
int recvlen;
int socket_descriptor;

unsigned long int vddTable[VADDTIMES];     //�ٶ��б�
string joinstr = "";
unsigned long int nowSpeed = VVVSSS;                   //1000��
//���ݲ��
void splitStr(char *s, list<string> &strArray);
//����ƴ��
void joinStr(vector<string>& strArray);
int server_sockfd;
//�õ����ݲ�����
void getDateParser();
//��������
void run();
//���̺߳���
void *thread_function0(void *arg);
void *thread_function1(void *arg);
void *thread_function2(void *arg);

int main()
{
	// �汾��
	version = "V2017 07 yingjian ceshi";	//�汾����Ҫ�Լ��趨
	// ��ʼ��ϵͳ��Ϣ
	initSystemState();
	dh =  0;
	TCFmatrix << 1, 0, 0,0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1;
	POSITIONER << 1, 0, 0,0,
			      0, 1, 0, 0,
			      0, 0, 1, 0,
			      0, 0, 0, 1;
	RESOLUTION = 10; 	//��ʾ��ɢ����Ϊ0.1mm
	RESOLUTION_ATT = 50;  //��ʾ��̬��ɢ����Ϊ1/30
	void *virtual_base;
	int fd;
// ���ڴ�ӳ���豸����
	if ((fd = open("/dev/mem", ( O_RDWR | O_SYNC))) == -1) //��ӳ������
	{
		printf("ERROR: could not open \"/dev/mem\"...\n");
		return (1);
	}

//ӳ �������ַ���û��ռ�
	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE),
			MAP_SHARED, fd, HW_REGS_BASE); //ӳ�䵽�ڴ�
	if (virtual_base == MAP_FAILED) {
		printf("ERROR: mmap() failed...\n");
		close(fd);
		return (1);
	}

//socӲ��
	h2p_lw_led_addr = (unsigned long) virtual_base
			+ ((unsigned long) ( ALT_LWFPGASLVS_OFST + CUSTOM_LEDS_0_BASE)
					& (unsigned long) ( HW_REGS_MASK));
	h2p_lw_key_addr = (unsigned long) virtual_base
			+ ((unsigned long) ( ALT_LWFPGASLVS_OFST + BUTTON_PIO_BASE)
					& (unsigned long) ( HW_REGS_MASK));
	h2p_lw_mot_addr = (unsigned long) virtual_base
			+ ((unsigned long) ( ALT_LWFPGASLVS_OFST + ROBOT_0_BASE)
					& (unsigned long) ( HW_REGS_MASK));
	h2p_lw_IOM_addr = (unsigned long) virtual_base
			+ ((unsigned long) ( ALT_LWFPGASLVS_OFST + IO_MOD_V2_0_BASE)
					& (unsigned long) ( HW_REGS_MASK));
	h2p_lw_AVO_addr = (unsigned long) virtual_base
			+ ((unsigned long) ( ALT_LWFPGASLVS_OFST + AVO_MOD_0_BASE)
					& (unsigned long) ( HW_REGS_MASK));


//***********************************************
	IOM->DATA=0xFFFFFFFF;//�رյ��������
	moto_init();//
	getVList(vddTable);//
	udp::initUdpSocket();
//***********************************************


//	allAxisInit();
//	J1RunToLimit(1,100);
//	 AVO->field.start = 3;
//	 AVO->field.spd = 0;
//	 AVO->field.pwr = 0;
//
//	 for(int n=0;n<2;n++){
//		 for(float i=0;i<2*pi;i=i+0.01*pi){
//
//				AVO->field.ch = 0;
//				AVO->field.data = (float)0xFF * sinf(i+0*pi/8.0) + 0xFF;
//				usleep(5000);
//
//				AVO->field.ch = 1;
//				AVO->field.data = (float)0xFF * sinf(i+1*pi/8.0) + 0xFF;
//				usleep(5000);
//
//				AVO->field.ch = 2;
//				AVO->field.data = (float)0xFF * sinf(i+2*pi/8.0) + 0xFF;
//				usleep(5000);
//
//				AVO->field.ch = 3;
//				AVO->field.data = (float)0xFF * sinf(i+3*pi/8.0) + 0xFF;
//				usleep(5000);
//		 }
//	 }
//
//	//===============================����������Դ���
//
//		while(1){
//			if(((IOM->DATA ) & 0X00100000))
//				IOM->DATA =0xFFFFFFFe;
//			else IOM->DATA=0xFFFFFFFF;
//			if (((IOM->DATA ) & 0X00200000))
//				IOM->DATA =0xFFFFFFFd;
//			if (((IOM->DATA ) & 0X00400000))
//				IOM->DATA =0xFFFFFFFb;
//			if (((IOM->DATA ) & 0X00800000))
//				IOM->DATA =0xFFFFFFF7;
//			if (((IOM->DATA ) & 0X01000000))
//				IOM->DATA =0xFFFFFFeF;
//			if (((IOM->DATA ) & 0X02000000))
//				IOM->DATA =0xFFFFFFdF;
//			if (((IOM->DATA ) & 0X04000000))
//				IOM->DATA =0xFFFFFFbF;
//			if (((IOM->DATA ) & 0X08000000))
//				IOM->DATA =0xFFFFFF7F;
//			if (((IOM->DATA ) & 0X10000000))
//				IOM->DATA =0xFFFFFeFF;
//			if (((IOM->DATA ) & 0X20000000))
//				IOM->DATA =0xFFFFFdFF;
//			if (((IOM->DATA ) & 0X40000000))
//				IOM->DATA =0xFFFFFbFF;
//			if (((IOM->DATA ) & 0X80000000))
//				IOM->DATA =0xFFFFF7FF;
//			usleep(100000);
//		}
//
//	cout<<"...test end here..."<<endl;
//	while(1);






	//=============================Socket_init
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_sockaddr;
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(PORT);
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(server_sockfd, (struct sockaddr *) &server_sockaddr,
			sizeof(server_sockaddr)) < 0)
	{
		perror("bind");
		exit(1);
	}

	if (listen(server_sockfd, 10) < 0)
	{
		perror("listen");
		exit(1);
	}

	struct sockaddr_in ckient_addr;
	socklen_t length = sizeof(ckient_addr);
	cout << "�ȴ������С������Ǻ�" << endl;

	conn = accept(server_sockfd, (struct sockaddr *) &ckient_addr, &length);
	if (conn < 0)
	{
		perror("connect");
		exit(1);
	} else
		cout << "�밲׿ϵͳ���ӳɹ�" << endl;
	DylCommon::setConn(conn);
	int res;
	pthread_t a_thread[3];
	void *thread_result;

// �����ź���
	res = pthread_mutex_init(&mutex, NULL);
	if (res != 0)
	{
		perror("Mutex init failed!");
		exit(EXIT_FAILURE);
	}

// �����ź���
	res = sem_init(&sem, 0, 0);
	if (res != 0)
	{
		perror("Sem init failed!");
		exit(EXIT_FAILURE);
	}

//�����߳�
	res = pthread_create(&a_thread[0], NULL, thread_function0, (void *) 0);
	res = pthread_create(&a_thread[1], NULL, thread_function1, (void *) 1);
	res = pthread_create(&a_thread[2], NULL, thread_function2, (void *) 2);
	if (res != 0)
	{
		perror("Thread creation failed!");
		exit(EXIT_FAILURE);
	}

//�߳�
	while (1)   //���߳��˶�
	{
	  getDateParser();
	}
    sem_post(&sem);
    sleep(1);
	printf("\nWaiting for thread to finish...\n");
	close(conn);
	close(server_sockfd);
	res = pthread_join(a_thread[0], &thread_result);
	res = pthread_join(a_thread[1], &thread_result);
	res = pthread_join(a_thread[2], &thread_result);
	if (res != 0)
	{
		perror("Thread join failed!\n");
		exit(EXIT_FAILURE);
	}

	printf("Thread joined\n");

	res = pthread_mutex_destroy(&mutex);
	if (res != 0) {
		perror("Thread destroy failed!\n");
		exit(EXIT_FAILURE);
	}


// ����ڴ�ӳ��
	if (munmap(virtual_base, HW_REGS_SPAN) != 0)
	{
		printf("ERROR: munmap() failed...\n");
		close(fd);
		return (1);
	}

// �ر��豸����
	close(fd);
	udp::closeUdp();
	return 0;
}


//�ָ��׽ӵ�����
void splitStr(char *s, list<string> &strArray)
{
	const char *d1 = "\n";
	char *p = strtok(s, d1);
	while (p)
	{
		strArray.push_back(p);
		p = strtok(NULL, d1);
	}

}



//ƴ������
void joinStr(vector<string>& strArray)
{
	//ƴ��   ���ݷ��Ͷ���ܳ��ֲ�����
	if (joinstr != "")
	{
		string begin = *strArray.begin();
		if (begin[0] != '[')
			*strArray.begin() = joinstr + *(strArray.begin());
		joinstr = "";
	}
	if (strArray.size() > 0)
	{
		string end = *(--strArray.end());
		if (*(--end.end()) != ']')
		{
			joinstr = end;
			strArray.erase(--strArray.end());
		}
	}
}


//*****************������������*******************//
void getDateParser()  //
{
	while (1)
	{
		memset(recvbuf, 0, sizeof(recvbuf));
		recvlen = recv(conn, recvbuf, sizeof(recvbuf), 0); //���յ��ĺ���
		if (recvlen <= 0)
		{
		  if (errno == EINTR)
		  {

		  }
		  else
			  exit(1);
		}
		string str(recvbuf);
		vector<string> strArray = DylCommon::split(str, "\n");
		joinStr(strArray); //ƴ��
		for (vector<string>::iterator it = strArray.begin();it != strArray.end(); it++)
		{
			if (*it == "\n" || *it == "")
				continue;
			parser.DateParser(*it); //���ݽ���
		}

	}

}
//�˶����������
void run()
{
	while (1)
	{
		parser.run();
		usleep(10000);
	}

}

//****************���˶�***************//
void *thread_function0(void *arg)
{
	    run();
		return 0;

}
//*********ң���˶�********************/
void *thread_function1(void *arg)
{
	while (1)
		{
			parser.roctorbar.RoctorRun();
			usleep(10000);
		}
		return 0;
}
//���߳̾�һֱ����λ���ش�����
void *thread_function2(void *arg)
{

    double count = 0;
	while (1)
	{
		//������ɲ��ϴ�����λ������
		if (parser.back_finished)
		{
			if (!parser.welding.runing || udp::IsOpenUdp)   //���ں��ӹ������ǲ�ִ�������
			{
				parser.coord.run();   //�ϴ��������
				if(!parser.IsSend )
				count += 2;          //˵����������
			}

		}
		if(parser.runing)
		parser.ioparameter.run();   //�ϴ�IO������
		if(!udp::IsOpenUdp)
		{
		usleep(100000);
		if(!parser.IsSend)
		count ++;
		if(count >= 50)  //5s֮�����Ƿ������ӵ�״̬
		{
			pthread_mutex_lock (&mutex);
			if(parser.IsConnnect)  //��ʾ��������״̬
				parser.IsConnnect = false;
			else
			{
				if(!Variable::IsStop)
				{
					Variable::IsStop = true;
				  IOM->DATA=0xFFFFFFFF;
				  cout <<"�����ж�   �˶�ֹͣ" << endl;
				}
			}
			pthread_mutex_unlock(&mutex);
			count = 0; //��������
		}
		}
	}
	return 0;
}



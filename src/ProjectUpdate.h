/*
 * ProjectUpdate.h
 *
 *  Created on: 2017��2��4��
 *      Author: Bing_yao
 */

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <sstream>
#include <fstream>

#include <list>
#include <string>
#include <stdbool.h>
#include <vector>


#ifndef SRC_PROJECTUPDATE_H_
#define SRC_PROJECTUPDATE_H_

#define FILE_NAME_MAX_SIZE 512
using namespace std;
enum SystemState {update, recovery};
extern SystemState systemState;
extern string version;
// �������ǻָ� ��ϵͳ��
void updateRun();
// ��ʼ�� ϵͳ״̬  ����汾����Ϣ
void initSystemState();


#endif /* SRC_PROJECTUPDATE_H_ */

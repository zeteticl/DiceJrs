#pragma once
#ifndef JRS_GLOBAL_VAR
#define JRS_GLOBAL_VAR
#include "CQLogger.h"
#include <map>

// Ӧ���Ƿ�����
extern bool Enabled;

// ��Ϣ�����߳��Ƿ���������
extern bool msgSendThreadRunning;

// ȫ�ֿ�QLogger
extern CQ::logger DiceLogger;

extern std::map<std::string, std::string> GlobalMsg;
#endif /*JRS_GLOBAL_VAR*/

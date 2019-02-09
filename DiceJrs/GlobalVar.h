#pragma once
#ifndef JRS_GLOBAL_VAR
#define JRS_GLOBAL_VAR
#include "CQLogger.h"
#include <map>

// 应用是否被启用
extern bool Enabled;

// 消息发送线程是否正在运行
extern bool msgSendThreadRunning;

// 全局酷QLogger
extern CQ::logger DiceLogger;

extern std::map<std::string, std::string> GlobalMsg;
#endif /*JRS_GLOBAL_VAR*/

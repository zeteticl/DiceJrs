#pragma once
#ifndef JRS_MSG_SEND
#define JRS_MSG_SEND
#include <string>

/*
 *  加锁并将消息存入消息发送队列
 *  Param:
 *  const std::string& msg 消息内容字符串
 *  long long target_id 目标ID(QQ,群号或讨论组uin)
 *  bool is_private = true 是否为私聊 默认为真
 */
void AddMsgToQueue(const std::string& msg, long long target_id, bool is_private = true);

/*
 * 消息发送线程函数
 * 注意: 切勿在主线程中调用此函数, 此函数仅用于初始化消息发送线程
 */
void SendMsg();
#endif /*JRS_MSG_SEND*/

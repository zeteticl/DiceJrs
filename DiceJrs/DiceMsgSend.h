#pragma once
#ifndef JRS_MSG_SEND
#define JRS_MSG_SEND
#include <string>

/*
 *  ����������Ϣ������Ϣ���Ͷ���
 *  Param:
 *  const std::string& msg ��Ϣ�����ַ���
 *  long long target_id Ŀ��ID(QQ,Ⱥ�Ż�������uin)
 *  bool is_private = true �Ƿ�Ϊ˽�� Ĭ��Ϊ��
 */
void AddMsgToQueue(const std::string& msg, long long target_id, bool is_private = true);

void SendMsg();
#endif /*JRS_MSG_SEND*/

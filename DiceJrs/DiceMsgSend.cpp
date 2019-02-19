#include <queue>
#include <mutex>
#include <string>
#include <thread>
#include "CQAPI_EX.h"
#include "DiceMsgSend.h"
#include "GlobalVar.h"
using namespace std;

// 消息发送存储结构体
struct msg_t
{
	string msg;
	long long target_id = 0;
	bool is_private = true;
	msg_t() = default;

	msg_t(string msg, long long target_id = 0, bool is_private = true) : msg(move(msg)), target_id(target_id),
	                                                                     is_private(is_private)
	{
	}
};

// 消息发送队列
std::queue<msg_t> msgQueue;

// 消息发送队列锁
mutex msgQueueMutex;

void AddMsgToQueue(const string& msg, long long target_id, bool is_private)
{
	lock_guard<std::mutex> lock_queue(msgQueueMutex);
	msgQueue.emplace(msg_t(msg, target_id, is_private));
}


void SendMsg()
{
	Enabled = true;
	msgSendThreadRunning = true;
	while (Enabled)
	{
		msg_t msg;
		msgQueueMutex.lock();
		if (!msgQueue.empty())
		{
			msg = msgQueue.front();
			msgQueue.pop();
		}
		msgQueueMutex.unlock();
		if (!msg.msg.empty())
		{
			if (msg.is_private)
			{
				CQ::sendPrivateMsg(msg.target_id, msg.msg);
			}
			else if (msg.target_id < 1000000000)
			{
				CQ::sendGroupMsg(msg.target_id, msg.msg);
			}
			else
			{
				CQ::sendDiscussMsg(msg.target_id, msg.msg);
			}
		}
		else
		{
			this_thread::sleep_for(chrono::milliseconds(20));
		}
	}
	msgSendThreadRunning = false;
}
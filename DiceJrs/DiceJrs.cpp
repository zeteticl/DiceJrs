#include <Windows.h>
#include <string>
#include <iostream>
#include <map>
#include <set>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <thread>
#include <chrono>
#include <mutex>

#include "APPINFO.h"
#include "RandomGenerator.h"
#include "RD.h"
#include "CQEVE_ALL.h"
#include "InitList.h"
#include "GlobalVar.h"
#include "NameStorage.h"
#include "DiceMsgSend.h"
#include "CustomMsg.h"
#include "MsgFormat.h"
#include "Jrs.h"

using namespace std;
using namespace CQ;

unique_ptr<NameStorage> Name;

std::string strip(std::string origin)
{
	bool flag = true;
	while (flag)
	{
		flag = false;
		if (origin[0] == '!' || origin[0] == '.')
		{
			origin.erase(origin.begin());
			flag = true;
		}
		else if (origin.substr(0, 2) == "��" || origin.substr(0, 2) == "��")
		{
			origin.erase(origin.begin());
			origin.erase(origin.begin());
			flag = true;
		}
	}
	return origin;
}

std::string getName(long long QQ, long long GroupID = 0)
{
	if (GroupID)
	{
		if (GroupID < 1000000000)
		{
			return strip(Name->get(GroupID, QQ).empty()
				             ? (getGroupMemberInfo(GroupID, QQ).GroupNick.empty()
					                ? getStrangerInfo(QQ).nick
					                : getGroupMemberInfo(GroupID, QQ).GroupNick)
				             : Name->get(GroupID, QQ));
		}
		return strip(Name->get(GroupID, QQ).empty() ? getStrangerInfo(QQ).nick : Name->get(GroupID, QQ));
	}
	return strip(getStrangerInfo(QQ).nick);
}

map<long long, FATE> JRFATE;
map<long long, int> DefaultDice;
map<long long, string> WelcomeMsg;
map<long long, string> LogMsg;
set<long long> EnableLogGroup;
set<long long> DisabledGroup;
set<long long> DisabledDiscuss;
set<long long> DisabledJRFATEGroup;
set<long long> DisabledJRFATEDiscuss;
set<long long> DisabledMEGroup;
set<long long> DisabledMEDiscuss;
set<long long> DisabledHELPGroup;
set<long long> DisabledHELPDiscuss;
set<long long> DisabledOBGroup;
set<long long> DisabledOBDiscuss;
set<long long> BanFriendList;
set<long long> BanGroupList;
unique_ptr<Initlist> ilInitList;

struct SourceType
{
	SourceType(long long a, int b, long long c) : QQ(a), Type(b), GrouporDiscussID(c)
	{
	};
	long long QQ = 0;
	int Type = 0;
	long long GrouporDiscussID = 0;

	bool operator<(SourceType b) const
	{
		return this->QQ < b.QQ;
	}
};

using PropType = map<string, int>;
map<SourceType, PropType> CharacterProp;
multimap<long long, long long> ObserveGroup;
multimap<long long, long long> ObserveDiscuss;
string strFileLoc;
EVE_Enable(eventEnable)
{
	//Wait until the thread terminates
	while (msgSendThreadRunning)
		Sleep(10);

	thread msgSendThread(SendMsg);
	msgSendThread.detach();
	strFileLoc = getAppDirectory();
	/*
	* ���ƴ洢-�������ȡ
	*/
	Name = make_unique<NameStorage>(strFileLoc + "Name.dicedb");
	ifstream ifstreamCharacterProp(strFileLoc + "CharacterProp.RDconf");
	if (ifstreamCharacterProp)
	{
		long long QQ, GrouporDiscussID;
		int Type, Value;
		string SkillName;
		while (ifstreamCharacterProp >> QQ >> Type >> GrouporDiscussID >> SkillName >> Value)
		{
			CharacterProp[SourceType(QQ, Type, GrouporDiscussID)][SkillName] = Value;
		}
	}
	ifstreamCharacterProp.close();

	ifstream ifstreamDisabledGroup(strFileLoc + "DisabledGroup.RDconf");
	if (ifstreamDisabledGroup)
	{
		long long Group;
		while (ifstreamDisabledGroup >> Group)
		{
			DisabledGroup.insert(Group);
		}
	}
	ifstreamDisabledGroup.close();
	ifstream ifstreamDisabledDiscuss(strFileLoc + "DisabledDiscuss.RDconf");
	if (ifstreamDisabledDiscuss)
	{
		long long Discuss;
		while (ifstreamDisabledDiscuss >> Discuss)
		{
			DisabledDiscuss.insert(Discuss);
		}
	}
	ifstreamDisabledDiscuss.close();

	//Fate
	ifstream ifstreamDisabledFATEG(strFileLoc + "DisabledJRFATEGroup.RDconf");
	if (ifstreamDisabledFATEG)
	{
		long long Group;
		while (ifstreamDisabledFATEG >> Group)
		{
			DisabledJRFATEGroup.insert(Group);
		}
	}
	ifstreamDisabledFATEG.close();
	ifstream ifstreamDisabledFATED(strFileLoc + "DisabledJRFATEDiscuss.RDconf");
	if (ifstreamDisabledFATED)
	{
		long long Discuss;
		while (ifstreamDisabledFATED >> Discuss)
		{
			DisabledJRFATEDiscuss.insert(Discuss);
		}
	}
	ifstreamDisabledFATED.close();
	//Fate
	ifstream ifstreamDisabledMEGroup(strFileLoc + "DisabledMEGroup.RDconf");
	if (ifstreamDisabledMEGroup)
	{
		long long Group;
		while (ifstreamDisabledMEGroup >> Group)
		{
			DisabledMEGroup.insert(Group);
		}
	}
	ifstreamDisabledMEGroup.close();
	ifstream ifstreamDisabledMEDiscuss(strFileLoc + "DisabledMEDiscuss.RDconf");
	if (ifstreamDisabledMEDiscuss)
	{
		long long Discuss;
		while (ifstreamDisabledMEDiscuss >> Discuss)
		{
			DisabledMEDiscuss.insert(Discuss);
		}
	}
	ifstreamDisabledMEDiscuss.close();
	ifstream ifstreamDisabledHELPGroup(strFileLoc + "DisabledHELPGroup.RDconf");
	if (ifstreamDisabledHELPGroup)
	{
		long long Group;
		while (ifstreamDisabledHELPGroup >> Group)
		{
			DisabledHELPGroup.insert(Group);
		}
	}
	ifstreamDisabledHELPGroup.close();
	ifstream ifstreamDisabledHELPDiscuss(strFileLoc + "DisabledHELPDiscuss.RDconf");
	if (ifstreamDisabledHELPDiscuss)
	{
		long long Discuss;
		while (ifstreamDisabledHELPDiscuss >> Discuss)
		{
			DisabledHELPDiscuss.insert(Discuss);
		}
	}
	ifstreamDisabledHELPDiscuss.close();
	ifstream ifstreamDisabledOBGroup(strFileLoc + "DisabledOBGroup.RDconf");
	if (ifstreamDisabledOBGroup)
	{
		long long Group;
		while (ifstreamDisabledOBGroup >> Group)
		{
			DisabledOBGroup.insert(Group);
		}
	}
	ifstreamDisabledOBGroup.close();
	ifstream ifstreamDisabledOBDiscuss(strFileLoc + "DisabledOBDiscuss.RDconf");
	if (ifstreamDisabledOBDiscuss)
	{
		long long Discuss;
		while (ifstreamDisabledOBDiscuss >> Discuss)
		{
			DisabledOBDiscuss.insert(Discuss);
		}
	}
	ifstreamDisabledOBDiscuss.close();
	ifstream ifstreamObserveGroup(strFileLoc + "ObserveGroup.RDconf");
	if (ifstreamObserveGroup)
	{
		long long Group, QQ;
		while (ifstreamObserveGroup >> Group >> QQ)
		{
			ObserveGroup.insert(make_pair(Group, QQ));
		}
	}
	ifstreamObserveGroup.close();

	ifstream ifstreamObserveDiscuss(strFileLoc + "ObserveDiscuss.RDconf");
	if (ifstreamObserveDiscuss)
	{
		long long Discuss, QQ;
		while (ifstreamObserveDiscuss >> Discuss >> QQ)
		{
			ObserveDiscuss.insert(make_pair(Discuss, QQ));
		}
	}
	ifstreamObserveDiscuss.close();
	//Fate
	ifstream ifstreamJRFATE(strFileLoc + "JRFATE.RDconf");
	if (ifstreamJRFATE)
	{
		long long QQ;
		int Val;
		string strDate;
		while (ifstreamJRFATE >> QQ >> strDate >> Val)
		{
			JRFATE[QQ].Date = strDate;
			JRFATE[QQ].FATEVal = Val;
		}
	}
	ifstreamJRFATE.close();
	//Fate
	//Ban
	ifstream ifstreamBanFriendList(strFileLoc + "BanFriendList.RDconf");
	if (ifstreamBanFriendList)
	{
		long long ban;
		while (ifstreamBanFriendList >> ban)
		{
			BanFriendList.insert(ban);
		}
	}
	ifstreamBanFriendList.close();
	//
	ifstream ifstreamBanGroupList(strFileLoc + "BanGroupList.RDconf");
	if (ifstreamBanGroupList)
	{
		long long ban;
		while (ifstreamBanGroupList >> ban)
		{
			BanGroupList.insert(ban);
		}
	}
	ifstreamBanGroupList.close();
	//
	ifstream ifstreamDefault(strFileLoc + "Default.RDconf");
	if (ifstreamDefault)
	{
		long long QQ;
		int DefVal;
		while (ifstreamDefault >> QQ >> DefVal)
		{
			DefaultDice[QQ] = DefVal;
		}
	}
	ifstreamDefault.close();
	ifstream ifstreamWelcomeMsg(strFileLoc + "WelcomeMsg.RDconf");
	if (ifstreamWelcomeMsg)
	{
		long long GroupID;
		string Msg;
		while (ifstreamWelcomeMsg >> GroupID >> Msg)
		{
			while (Msg.find("{space}") != string::npos)Msg.replace(Msg.find("{space}"), 7, " ");
			while (Msg.find("{tab}") != string::npos)Msg.replace(Msg.find("{tab}"), 5, "\t");
			while (Msg.find("{endl}") != string::npos)Msg.replace(Msg.find("{endl}"), 6, "\n");
			while (Msg.find("{enter}") != string::npos)Msg.replace(Msg.find("{enter}"), 7, "\r");
			WelcomeMsg[GroupID] = Msg;
		}
	}
	ifstreamWelcomeMsg.close();




	/*Log
	ifstream ifstreamLogMsg(strFileLoc + "LogMsg.RDconf");
	{
		long long GroupID;
		string LogMsgs;
		while (ifstreamLogMsg >> GroupID >> LogMsgs)
		{
			while (Msg.find("{space}") != string::npos)Msg.replace(Msg.find("{space}"), 7, " ");
			while (Msg.find("{enter}") != string::npos)Msg.replace(Msg.find("{enter}"), 7, "\r");
			LogMsg = LogMsgs;
		}
	}
	*/




	ilInitList = make_unique<Initlist>(strFileLoc + "INIT.DiceDB");
	ifstream ifstreamCustomMsg(strFileLoc + "CustomMsg.json");
	if (ifstreamCustomMsg)
	{
		ReadCustomMsg(ifstreamCustomMsg);
	}
	ifstreamCustomMsg.close();
	return 0;
}


EVE_PrivateMsg_EX(eventPrivateMsg)
{
	if (eve.isSystem())return;
	init(eve.message);
	init2(eve.message);
	if (eve.message[0] != '.')return;
	int intMsgCnt = 1;
	while (isspace(static_cast<unsigned char>(eve.message[intMsgCnt])))intMsgCnt++;
	eve.message_block();
	const string strNickName = getName(eve.fromQQ);
	string strLowerMessage = eve.message;
	transform(strLowerMessage.begin(), strLowerMessage.end(), strLowerMessage.begin(), [](unsigned char c) { return tolower(c); });
	if (BanFriendList.count(eve.fromQQ) && !strLowerMessage.empty())
	{ 
		const string strReply = "<" + eve.fromQQ + GlobalMsg["strBanned"];
		AddMsgToQueue(strReply, eve.fromQQ);
		return;
	}
	if (strLowerMessage.substr(intMsgCnt, 4) == "help")
		{
			intMsgCnt += 4;
			while (strLowerMessage[intMsgCnt] == ' ')
				intMsgCnt++;
			const string strAction = strLowerMessage.substr(intMsgCnt);
			if (strAction == "list")
			{
				AddMsgToQueue(GlobalMsg["strHlpMsgList"], eve.fromQQ);
				return;
			}
			AddMsgToQueue(GlobalMsg["strHlpMsg"] + "\n�˰汾����ʱ�䣺"+ __DATE__ + " " + __TIME__, eve.fromQQ);
		}
	else if (strLowerMessage.substr(intMsgCnt, 3) == "bot")
		{
			AddMsgToQueue(Dice_Bot_Ver, eve.fromQQ);
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "me")
		{
			intMsgCnt += 2;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strGroupID;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strGroupID += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strAction = strip(eve.message.substr(intMsgCnt));

			for (auto i : strGroupID)
			{
				if (!isdigit(static_cast<unsigned char>(i)))
				{
					AddMsgToQueue(GlobalMsg["strGroupIDInvalid"], eve.fromQQ);
					return;
				}
			}
			if (strGroupID.empty())
			{
				AddMsgToQueue("Ⱥ�Ų���Ϊ��!", eve.fromQQ);
				return;
			}
			if (strAction.empty())
			{
				AddMsgToQueue("��������Ϊ��!", eve.fromQQ);
				return;
			}
			const long long llGroupID = stoll(strGroupID);
			if (DisabledGroup.count(llGroupID))
			{
				AddMsgToQueue(GlobalMsg["strDisabledErr"], eve.fromQQ);
				return;
			}
			if (DisabledMEGroup.count(llGroupID))
			{
				AddMsgToQueue(GlobalMsg["strMEDisabledErr"], eve.fromQQ);
				return;
			}
			string strReply = getName(eve.fromQQ, llGroupID) + strAction;
			const int intSendRes = sendGroupMsg(llGroupID, strReply);
			if (intSendRes < 0)
			{
				AddMsgToQueue(GlobalMsg["strSendErr"], eve.fromQQ);
			}
			else
			{
				AddMsgToQueue("����ִ�гɹ�!", eve.fromQQ);
			}
		}
	else if (strLowerMessage.substr(intMsgCnt, 4) == "coc6")
		{
			intMsgCnt += 4;
			if (strLowerMessage[intMsgCnt] == 's')
				intMsgCnt++;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strNum;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strNum += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (strNum.length() > 2)
			{
				AddMsgToQueue(GlobalMsg["strCharacterTooBig"], eve.fromQQ);
				return;
			}
			const int intNum = stoi(strNum.empty() ? "1" : strNum);
			if (intNum > 10)
			{
				AddMsgToQueue(GlobalMsg["strCharacterTooBig"], eve.fromQQ);
				return;
			}
			if (intNum == 0)
			{
				AddMsgToQueue(GlobalMsg["strCharacterCannotBeZero"], eve.fromQQ);
				return;
			}
			string strReply = strNickName;
			COC6(strReply, intNum);
			AddMsgToQueue(strReply, eve.fromQQ);
		}
	else if (strLowerMessage.substr(intMsgCnt, 3) == "dnd")
		{
			intMsgCnt += 3;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strNum;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strNum += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (strNum.length() > 2)
			{
				AddMsgToQueue(GlobalMsg["strCharacterTooBig"], eve.fromQQ);
				return;
			}
			const int intNum = stoi(strNum.empty() ? "1" : strNum);
			if (intNum > 10)
			{
				AddMsgToQueue(GlobalMsg["strCharacterTooBig"], eve.fromQQ);
				return;
			}
			if (intNum == 0)
			{
				AddMsgToQueue(GlobalMsg["strCharacterCannotBeZero"], eve.fromQQ);
				return;
			}
			string strReply = strNickName;
			DND(strReply, intNum);
			AddMsgToQueue(strReply, eve.fromQQ);
		}
	else if (strLowerMessage.substr(intMsgCnt, 3) == "coc")
		{
			intMsgCnt += 3;
			if (strLowerMessage[intMsgCnt] == '7')
				intMsgCnt++;
			if (strLowerMessage[intMsgCnt] == 's')
				intMsgCnt++;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strNum;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strNum += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (strNum.length() > 2)
			{
				AddMsgToQueue(GlobalMsg["strCharacterTooBig"], eve.fromQQ);
				return;
			}
			const int intNum = stoi(strNum.empty() ? "1" : strNum);
			if (intNum > 10)
			{
				AddMsgToQueue(GlobalMsg["strCharacterTooBig"], eve.fromQQ);
				return;
			}
			if (intNum == 0)
			{
				AddMsgToQueue(GlobalMsg["strCharacterCannotBeZero"], eve.fromQQ);
				return;
			}
			string strReply = strNickName;
			COC7(strReply, intNum);
			AddMsgToQueue(strReply, eve.fromQQ);
		}
	else if (strLowerMessage.substr(intMsgCnt, 4) == "exit")
	{
	if (eve.fromQQ != MASTER) return;
	intMsgCnt += 4;
	while (isspace(strLowerMessage[intMsgCnt]))
		intMsgCnt++;
	string strgroup;
	while (isdigit(strLowerMessage[intMsgCnt]))
	{
		strgroup += strLowerMessage[intMsgCnt];
		intMsgCnt++;
	}
	for (auto i : strgroup)
	{
		if (!isdigit(i))
		{
			return;
		}
	}
	const long long llgroup = stoll(strgroup);
	AddMsgToQueue("������⵽�Ǻ������أ����𽫾��յĶ��������������;��Ⱥ�У������������ϵ���ս���2558272027��", llgroup, false);
	if (setGroupLeave(llgroup) == 0)
		AddMsgToQueue("�Ѵ�" + strgroup + "���˳�", eve.fromQQ);
	eve.message_block();
	return;
	}
	else if (strLowerMessage.substr(intMsgCnt, 3) == "ban")
	{
		if (eve.fromQQ == MASTER)
		{
			intMsgCnt += 3;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strBan;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strBan += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strClassify = strip(eve.message.substr(intMsgCnt));

			if (strClassify == "friend" || strClassify == "f") {
				for (auto i : strBan)
				{
					if (!isdigit(i))
					{
						AddMsgToQueue("���� ��ʽΪ.ban [�˺�] [���]", eve.fromQQ);
						return;
					}
				}
				const long long ban = stoll(strBan);
				if (ban == MASTER)
				{
					AddMsgToQueue(GlobalMsg["strBanMaster"], eve.fromQQ);
				}
				else if (BanFriendList.count(ban))
				{
					AddMsgToQueue(GlobalMsg["strAlreadyBanned"], eve.fromQQ);
				}
				else
				{
					BanFriendList.insert(ban);
					AddMsgToQueue(GlobalMsg["strSuccessfullyBanned"], eve.fromQQ);
				}
			}
			if (strClassify == "group" || strClassify == "g") {
				for (auto i : strBan)
				{
					if (!isdigit(static_cast<unsigned char>(i)))
					{
						AddMsgToQueue("���� ��ʽΪ.ban [�˺�] [���]", eve.fromQQ);
						return;
					}
				}
				const long long ban = stoll(strBan);
				if (BanGroupList.count(ban))
				{
					AddMsgToQueue(GlobalMsg["strAlreadyBanned"], eve.fromQQ);
				}
				else
				{
					BanGroupList.insert(ban);
					AddMsgToQueue(GlobalMsg["strSuccessfullyBanned"], eve.fromQQ);
				}
			}
			if (strClassify == "list" || strClassify == "l")
			{
				string ReplyF = "<�û�����б�>\n";
				auto itera = BanFriendList.begin();
				while (itera != BanFriendList.end())
				{
					ReplyF += to_string(*itera) + '\n';
					itera++;
				}
				string ReplyG = "<Ⱥ����б�>\n";
				auto iterb = BanGroupList.begin();
				while (iterb != BanGroupList.end())
				{
					ReplyG += to_string(*iterb) + '\n';
					iterb++;
				}
				AddMsgToQueue(ReplyF + "\n" + ReplyG, eve.fromQQ);
			}
		}
		else
		{
			AddMsgToQueue(GlobalMsg["strNoAuth"], eve.fromQQ);
			AddMsgToQueue("�û�<" + to_string(eve.fromQQ) + ">���Բ��Ϸ�����", MASTER);
		}
	}
	else if (strLowerMessage.substr(intMsgCnt, 5) == "unban")
	{
	if (eve.fromQQ == MASTER)
	{
		intMsgCnt += 5;
		while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			intMsgCnt++;
		string strBan;
		while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
		{
			strBan += strLowerMessage[intMsgCnt];
			intMsgCnt++;
		}
		while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			intMsgCnt++;
		string strClassify = strip(eve.message.substr(intMsgCnt));

		if (strClassify == "friend" || strClassify == "f")
		{
			for (auto i : strBan)
			{
				if (!isdigit(static_cast<unsigned char>(i)))
				{
					AddMsgToQueue("���� ��ʽΪ.ban [�˺�] [���]", eve.fromQQ);
					return;
				}
			}
			const long long ban = stoll(strBan);
			if (BanFriendList.count(ban))
			{
				BanFriendList.erase(ban);
				AddMsgToQueue(GlobalMsg["strUnBan"], eve.fromQQ);
			}
			else
			{
				AddMsgToQueue(GlobalMsg["strNotBanned"], eve.fromQQ);
			}
		}
		if (strClassify == "group" || strClassify == "g")
		{
			for (auto i : strBan)
			{
				if (!isdigit(static_cast<unsigned char>(i)))
				{
					AddMsgToQueue("���� ��ʽΪ.ban [�˺�] [���]", eve.fromQQ);
					return;
				}
			}
			const long long ban = stoll(strBan);
			if (BanGroupList.count(ban))
			{
				BanGroupList.erase(ban);
				AddMsgToQueue(GlobalMsg["strUnBan"], eve.fromQQ);
			}
			else
			{
				AddMsgToQueue(GlobalMsg["strNotBanned"], eve.fromQQ);
			}
		}
	}
	else
	{
		AddMsgToQueue(GlobalMsg["strNoAuth"], eve.fromQQ);
		AddMsgToQueue("�û�<" + to_string(eve.fromQQ) + ">���Խ�����", MASTER);
	}
	}
	else if (strLowerMessage[intMsgCnt] == 'r')
		{
			intMsgCnt += 1;
			bool boolDetail = true;
			if (eve.message[intMsgCnt] == 's')
			{
				boolDetail = false;
				intMsgCnt++;
			}
			while (isspace(static_cast<unsigned char>(eve.message[intMsgCnt])))intMsgCnt++;
			string strMainDice;
			string strReason;
			bool tmpContainD = false;
			int intTmpMsgCnt;
			for (intTmpMsgCnt = intMsgCnt; intTmpMsgCnt != eve.message.length() && eve.message[intTmpMsgCnt] != ' '; intTmpMsgCnt++)
			{
				if (strLowerMessage[intTmpMsgCnt] == 'd' || strLowerMessage[intTmpMsgCnt] == 'p' || strLowerMessage[intTmpMsgCnt] == 'b' || strLowerMessage[intTmpMsgCnt] == '#' || strLowerMessage[intTmpMsgCnt] == 'f' || strLowerMessage[intTmpMsgCnt] == 'a')
					tmpContainD = true;
				if (!isdigit(static_cast<unsigned char>(strLowerMessage[intTmpMsgCnt])) && strLowerMessage[intTmpMsgCnt] != 'd' && strLowerMessage[intTmpMsgCnt] != 'k' && strLowerMessage[intTmpMsgCnt] != 'p' && strLowerMessage[intTmpMsgCnt] != 'b'&&strLowerMessage[intTmpMsgCnt] != 'f' && strLowerMessage[intTmpMsgCnt] != '+' && strLowerMessage[intTmpMsgCnt] != '-' && strLowerMessage[intTmpMsgCnt] != '#' && strLowerMessage[intTmpMsgCnt] != 'a')
				{
					break;
				}
			}
			if (tmpContainD)
			{
				strMainDice = strLowerMessage.substr(intMsgCnt, intTmpMsgCnt - intMsgCnt);
				while (isspace(static_cast<unsigned char>(eve.message[intTmpMsgCnt])))
					intTmpMsgCnt++;
				strReason = eve.message.substr(intTmpMsgCnt);
			}
			else
				strReason = eve.message.substr(intMsgCnt);

			int intTurnCnt = 1;
			if (strMainDice.find("#") != string::npos)
			{
				string strTurnCnt = strMainDice.substr(0, strMainDice.find("#"));
				if (strTurnCnt.empty())
					strTurnCnt = "1";
				strMainDice = strMainDice.substr(strMainDice.find("#") + 1);
				RD rdTurnCnt(strTurnCnt, eve.fromQQ);
				const int intRdTurnCntRes = rdTurnCnt.Roll();
				if (intRdTurnCntRes == Value_Err)
				{
					AddMsgToQueue(GlobalMsg["strValueErr"], eve.fromQQ);
					return;
				}
				if (intRdTurnCntRes == Input_Err)
				{
					AddMsgToQueue(GlobalMsg["strInputErr"], eve.fromQQ);
					return;
				}
				if (intRdTurnCntRes == ZeroDice_Err)
				{
					AddMsgToQueue(GlobalMsg["strZeroDiceErr"], eve.fromQQ);
					return;
				}
				if (intRdTurnCntRes == ZeroType_Err)
				{
					AddMsgToQueue(GlobalMsg["strZeroTypeErr"], eve.fromQQ);
					return;
				}
				if (intRdTurnCntRes == DiceTooBig_Err)
				{
					AddMsgToQueue(GlobalMsg["strDiceTooBigErr"], eve.fromQQ);
					return;
				}
				if (intRdTurnCntRes == TypeTooBig_Err)
				{
					AddMsgToQueue(GlobalMsg["strTypeTooBigErr"], eve.fromQQ);
					return;
				}
				if (intRdTurnCntRes == AddDiceVal_Err)
				{
					AddMsgToQueue(GlobalMsg["strAddDiceValErr"], eve.fromQQ);
					return;
				}
				if (intRdTurnCntRes != 0)
				{
					AddMsgToQueue(GlobalMsg["strUnknownErr"], eve.fromQQ);
					return;
				}
				if (rdTurnCnt.intTotal > 10)
				{
					AddMsgToQueue(GlobalMsg["strRollTimeExceeded"], eve.fromQQ);
					return;
				}
				if (rdTurnCnt.intTotal <= 0)
				{
					AddMsgToQueue(GlobalMsg["strRollTimeErr"], eve.fromQQ);
					return;
				}
				intTurnCnt = rdTurnCnt.intTotal;
				if (strTurnCnt.find("d") != string::npos)
				{
					const string strTurnNotice = strNickName + "����������: " + rdTurnCnt.FormShortString() + "��";

					AddMsgToQueue(strTurnNotice, eve.fromQQ);
				}
			}
			RD rdMainDice(strMainDice, eve.fromQQ);

			const int intFirstTimeRes = rdMainDice.Roll();
			if (intFirstTimeRes == Value_Err)
			{
				AddMsgToQueue(GlobalMsg["strValueErr"], eve.fromQQ);
				return;
			}
			if (intFirstTimeRes == Input_Err)
			{
				AddMsgToQueue(GlobalMsg["strInputErr"], eve.fromQQ);
				return;
			}
			if (intFirstTimeRes == ZeroDice_Err)
			{
				AddMsgToQueue(GlobalMsg["strZeroDiceErr"], eve.fromQQ);
				return;
			}
			if (intFirstTimeRes == ZeroType_Err)
			{
				AddMsgToQueue(GlobalMsg["strZeroTypeErr"], eve.fromQQ);
				return;
			}
			if (intFirstTimeRes == DiceTooBig_Err)
			{
				AddMsgToQueue(GlobalMsg["strDiceTooBigErr"], eve.fromQQ);
				return;
			}
			if (intFirstTimeRes == TypeTooBig_Err)
			{
				AddMsgToQueue(GlobalMsg["strTypeTooBigErr"], eve.fromQQ);
				return;
			}
			if (intFirstTimeRes == AddDiceVal_Err)
			{
				AddMsgToQueue(GlobalMsg["strAddDiceValErr"], eve.fromQQ);
				return;
			}
			if (intFirstTimeRes != 0)
			{
				AddMsgToQueue(GlobalMsg["strUnknownErr"], eve.fromQQ);
				return;
			}
			if (!boolDetail && intTurnCnt != 1)
			{
				string strAns = strNickName + "������: " + to_string(intTurnCnt) + "��" + rdMainDice.strDice + ": { ";
				if (!strReason.empty())
					strAns.insert(0, "����" + strReason + " ");
				vector<int> vintExVal;
				while (intTurnCnt--)
				{
					// �˴�����ֵ����
					// ReSharper disable once CppExpressionWithoutSideEffects
					rdMainDice.Roll();
					strAns += to_string(rdMainDice.intTotal);
					if (intTurnCnt != 0)
						strAns += ",";
					if ((rdMainDice.strDice == "D100" || rdMainDice.strDice == "1D100") && (rdMainDice.intTotal <= 5 ||
						rdMainDice.intTotal >= 96))
						vintExVal.push_back(rdMainDice.intTotal);
				}
				strAns += " }";
				if (!vintExVal.empty())
				{
					strAns += ",��ֵ: ";
					for (auto it = vintExVal.cbegin(); it != vintExVal.cend(); ++it)
					{
						strAns += to_string(*it);
						if (it != vintExVal.cend() - 1)
							strAns += ",";
					}
				}
				AddMsgToQueue(strAns, eve.fromQQ);
			}
			else
			{
				while (intTurnCnt--)
				{
					// �˴�����ֵ����
					// ReSharper disable once CppExpressionWithoutSideEffects
					rdMainDice.Roll();
					string strAns = strNickName + "������: " + (boolDetail
						? rdMainDice.FormCompleteString()
						: rdMainDice.FormShortString());
					if (!strReason.empty())
						strAns.insert(0, "����" + strReason + " ");
					AddMsgToQueue(strAns, eve.fromQQ);
				}
			}
		}
}
EVE_GroupMsg_EX(eventGroupMsg)
{
	if (eve.isSystem() || eve.isAnonymous())return;
	init(eve.message);
	while (isspace(static_cast<unsigned char>(eve.message[0])))eve.message.erase(eve.message.begin());
	string strAt = "[CQ:at,qq=" + to_string(getLoginQQ()) + "]";
	if (eve.message.substr(0, 6) == "[CQ:at")
	{
		if (eve.message.substr(0, strAt.length()) == strAt)
		{
			eve.message = eve.message.substr(strAt.length());
		}
		else
		{
			return;
		}
	}
	init2(eve.message);
	if (eve.message[0] != '.')return;
	int intMsgCnt = 1;
	while (isspace(static_cast<unsigned char>(eve.message[intMsgCnt])))intMsgCnt++;
	eve.message_block();
	const string strNickName = getName(eve.fromQQ, eve.fromGroup);
	string strLowerMessage = eve.message;
	transform(strLowerMessage.begin(), strLowerMessage.end(), strLowerMessage.begin(), [](unsigned char c) { return tolower(c); });
	if (BanFriendList.count(eve.fromQQ) && !strLowerMessage.empty())
	{
		const string strReply = "<" + eve.fromQQ + GlobalMsg["strBanned"];
		AddMsgToQueue(strReply, eve.fromGroup, false);
		return;
	}
	if (BanFriendList.count(eve.fromGroup) && !strLowerMessage.empty())
	{
		const string strReply = GlobalMsg["strGroupBanned"];
		AddMsgToQueue(strReply, eve.fromGroup, false);
		return;
	}

	if (strLowerMessage.substr(intMsgCnt, 3) == "bot")
		{
			intMsgCnt += 3;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string Command;
			while (intMsgCnt != strLowerMessage.length() && !isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !isspace(
				static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				Command += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string QQNum = strLowerMessage.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) - intMsgCnt);
			if (Command == "on")
			{
				if (eve.fromQQ == MASTER || QQNum.empty() || QQNum == to_string(getLoginQQ()) || (QQNum.length() == 4 && QQNum == to_string(getLoginQQ() % 10000)))
				{
					if (eve.fromQQ == MASTER || getGroupMemberInfo(eve.fromGroup, eve.fromQQ).permissions >= 2)
					{
						if (DisabledGroup.count(eve.fromGroup))
						{
							DisabledGroup.erase(eve.fromGroup);
							AddMsgToQueue("�ɹ�������������!", eve.fromGroup, false);
						}
						else
						{
							AddMsgToQueue("���������Ѿ����ڿ���״̬!", eve.fromGroup, false);
						}
					}
					else
					{
						AddMsgToQueue(GlobalMsg["strPermissionDeniedErr"], eve.fromGroup, false);
					}
				}
			}
			else if (Command == "off")
			{
				if (eve.fromQQ == MASTER || QQNum.empty() || QQNum == to_string(getLoginQQ()) || (QQNum.length() == 4 && QQNum == to_string(getLoginQQ() % 10000)))
				{
					if (eve.fromQQ == MASTER || getGroupMemberInfo(eve.fromGroup, eve.fromQQ).permissions >= 2)
					{
						if (!DisabledGroup.count(eve.fromGroup))
						{
							DisabledGroup.insert(eve.fromGroup);
							AddMsgToQueue("�ɹ��رձ�������!", eve.fromGroup, false);
						}
						else
						{
							AddMsgToQueue("���������Ѿ����ڹر�״̬!", eve.fromGroup, false);
						}
					}
					else
					{
						AddMsgToQueue(GlobalMsg["strPermissionDeniedErr"], eve.fromGroup, false);
					}
				}
			}
			else
			{
				if (eve.fromQQ == MASTER || QQNum.empty() || QQNum == to_string(getLoginQQ()) || (QQNum.length() == 4 && QQNum == to_string(getLoginQQ() % 10000)))
				{
					AddMsgToQueue(Dice_Bot_Ver, eve.fromGroup, false);
				}
			}
			return;
		}
	if (strLowerMessage.substr(intMsgCnt, 4) == "quit")
		{
			intMsgCnt += 4;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string QQNum;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				QQNum += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (eve.fromQQ == MASTER || QQNum.empty() || (QQNum.length() == 4 && QQNum == to_string(getLoginQQ() % 10000)) || QQNum == to_string(getLoginQQ()))
			{
				if (eve.fromQQ == MASTER || getGroupMemberInfo(eve.fromGroup, eve.fromQQ).permissions >= 2)
				{
					setGroupLeave(eve.fromGroup);
				}
				else
				{
					AddMsgToQueue(GlobalMsg["strPermissionDeniedErr"], eve.fromGroup, false);
				}
			}
			return;
		}
	if (strLowerMessage.substr(intMsgCnt, 4) == "help")
		{
			intMsgCnt += 4;
			while (strLowerMessage[intMsgCnt] == ' ')
				intMsgCnt++;
			const string strAction = strLowerMessage.substr(intMsgCnt);
			if (strAction == "on")
			{
				if (eve.fromQQ == MASTER || getGroupMemberInfo(eve.fromGroup, eve.fromQQ).permissions >= 2)
				{
					if (DisabledHELPGroup.count(eve.fromGroup))
					{
						DisabledHELPGroup.erase(eve.fromGroup);
						AddMsgToQueue("�ɹ��ڱ�Ⱥ������.help����!", eve.fromGroup, false);
					}
					else
					{
						AddMsgToQueue("�ڱ�Ⱥ��.help����û�б�����!", eve.fromGroup, false);
					}
				}
				else
				{
					AddMsgToQueue(GlobalMsg["strPermissionDeniedErr"], eve.fromGroup, false);
				}
				return;
			}
			if (strAction == "off")
			{
				if (eve.fromQQ == MASTER || getGroupMemberInfo(eve.fromGroup, eve.fromQQ).permissions >= 2)
				{
					if (!DisabledHELPGroup.count(eve.fromGroup))
					{
						DisabledHELPGroup.insert(eve.fromGroup);
						AddMsgToQueue("�ɹ��ڱ�Ⱥ�н���.help����!", eve.fromGroup, false);
					}
					else
					{
						AddMsgToQueue("�ڱ�Ⱥ��.help����û�б�����!", eve.fromGroup, false);
					}
				}
				else
				{
					AddMsgToQueue(GlobalMsg["strPermissionDeniedErr"], eve.fromGroup, false);
				}
				return;
			}
			if (DisabledHELPGroup.count(eve.fromGroup))
			{
				const string strMag = "���յĶ�����Ϣ����";
				AddMsgToQueue(strMag, eve.fromGroup, false);
				return;
			}
			if (strAction == "list" && !DisabledHELPGroup.count(eve.fromGroup))
			{
				AddMsgToQueue(GlobalMsg["strHlpMsgList"], eve.fromGroup, false);
				return;
			}
			AddMsgToQueue(GlobalMsg["strHlpMsg"] + "\n�˰汾����ʱ�䣺" + __DATE__ + " " + __TIME__, eve.fromGroup, false);
		}
	if (DisabledGroup.count(eve.fromGroup))return;
	else if (strLowerMessage.substr(intMsgCnt, 7) == "welcome")
		{
			intMsgCnt += 7;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			if (eve.fromQQ == MASTER || getGroupMemberInfo(eve.fromGroup, eve.fromQQ).permissions >= 2)
			{
				string strWelcomeMsg = eve.message.substr(intMsgCnt);
				if (strWelcomeMsg.empty())
				{
					if (WelcomeMsg.count(eve.fromGroup))
					{
						WelcomeMsg.erase(eve.fromGroup);
						AddMsgToQueue(GlobalMsg["strWelcomeMsgClearNotice"], eve.fromGroup, false);
					}
					else
					{
						AddMsgToQueue(GlobalMsg["strWelcomeMsgClearErr"], eve.fromGroup, false);
					}
				}
				else
				{
					WelcomeMsg[eve.fromGroup] = strWelcomeMsg;
					AddMsgToQueue(GlobalMsg["strWelcomeMsgUpdateNotice"], eve.fromGroup, false);
				}
			}
			else
			{
				AddMsgToQueue(GlobalMsg["strPermissionDeniedErr"], eve.fromGroup, false);
			}
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "st")
		{
			intMsgCnt += 2;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			if (intMsgCnt == strLowerMessage.length())
			{
				AddMsgToQueue(GlobalMsg["strStErr"], eve.fromGroup, false);
				return;
			}
			if (strLowerMessage.substr(intMsgCnt, 3) == "clr")
			{
				if (CharacterProp.count(SourceType(eve.fromQQ, GroupT, eve.fromGroup)))
				{
					CharacterProp.erase(SourceType(eve.fromQQ, GroupT, eve.fromGroup));
				}
				AddMsgToQueue(GlobalMsg["strPropCleared"], eve.fromGroup, false);
				return;
			}
			if (strLowerMessage.substr(intMsgCnt, 3) == "del")
			{
				intMsgCnt += 3;
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
					intMsgCnt++;
				string strSkillName;
				while (intMsgCnt != strLowerMessage.length() && !isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !(strLowerMessage[
					intMsgCnt] == '|'))
				{
					strSkillName += strLowerMessage[intMsgCnt];
					intMsgCnt++;
				}
					if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
					if (CharacterProp.count(SourceType(eve.fromQQ, GroupT, eve.fromGroup)) && CharacterProp[SourceType(
						eve.fromQQ, GroupT, eve.fromGroup)].count(strSkillName))
					{
						CharacterProp[SourceType(eve.fromQQ, GroupT, eve.fromGroup)].erase(strSkillName);
						AddMsgToQueue(GlobalMsg["strPropDeleted"], eve.fromGroup, false);
					}
					else
					{
						AddMsgToQueue(GlobalMsg["strPropNotFound"], eve.fromGroup, false);
					}
					return;
			}
			if (strLowerMessage.substr(intMsgCnt, 4) == "show")
			{
				intMsgCnt += 4;
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
					intMsgCnt++;
				string strSkillName;
				while (intMsgCnt != strLowerMessage.length() && !isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !(strLowerMessage[
					intMsgCnt] == '|'))
				{
					strSkillName += strLowerMessage[intMsgCnt];
					intMsgCnt++;
				}
					if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
					if (CharacterProp.count(SourceType(eve.fromQQ, GroupT, eve.fromGroup)) && CharacterProp[SourceType(
						eve.fromQQ, GroupT, eve.fromGroup)].count(strSkillName))
					{
						AddMsgToQueue(format(GlobalMsg["strProp"], {
							strNickName, strSkillName,
							to_string(CharacterProp[SourceType(eve.fromQQ, GroupT, eve.fromGroup)][strSkillName])
							}), eve.fromGroup, false);
					}
					else if (SkillDefaultVal.count(strSkillName))
					{
						AddMsgToQueue(format(GlobalMsg["strProp"], { strNickName, strSkillName, to_string(SkillDefaultVal[strSkillName]) }),
							eve.fromGroup, false);
					}
					else
					{
						AddMsgToQueue(GlobalMsg["strPropNotFound"], eve.fromGroup, false);
					}
					return;
			}
			bool boolError = false;
			while (intMsgCnt != strLowerMessage.length())
			{
				string strSkillName;
				while (intMsgCnt != strLowerMessage.length() && !isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !
					isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && strLowerMessage[intMsgCnt] != '=' && strLowerMessage[intMsgCnt]
					!= ':')
				{
					strSkillName += strLowerMessage[intMsgCnt];
					intMsgCnt++;
				}
				if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) || strLowerMessage[intMsgCnt] == '=' || strLowerMessage[intMsgCnt
				] == ':')intMsgCnt++;
				string strSkillVal;
				while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				{
					strSkillVal += strLowerMessage[intMsgCnt];
					intMsgCnt++;
				}
				if (strSkillName.empty() || strSkillVal.empty() || strSkillVal.length() > 3)
				{
					boolError = true;
					break;
				}
				CharacterProp[SourceType(eve.fromQQ, GroupT, eve.fromGroup)][strSkillName] = stoi(strSkillVal);
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) || strLowerMessage[intMsgCnt] == '|')intMsgCnt++;
			}
			if (boolError)
			{
				AddMsgToQueue(GlobalMsg["strPropErr"], eve.fromGroup, false);
			}
			else
			{
				AddMsgToQueue(GlobalMsg["strSetPropSuccess"], eve.fromGroup, false);
			}
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "ri")
		{
			intMsgCnt += 2;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))intMsgCnt++;
			string strinit = "D20";
			if (strLowerMessage[intMsgCnt] == '+' || strLowerMessage[intMsgCnt] == '-')
			{
				strinit += strLowerMessage[intMsgCnt];
				intMsgCnt++;
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))intMsgCnt++;
			}
			else if (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				strinit += '+';
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strinit += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				intMsgCnt++;
			}
			string strname = eve.message.substr(intMsgCnt);
			if (strname.empty())
				strname = strNickName;
			else
				strname = strip(strname);
			RD initdice(strinit);
			const int intFirstTimeRes = initdice.Roll();
			if (intFirstTimeRes == Value_Err)
			{
				AddMsgToQueue(GlobalMsg["strValueErr"], eve.fromGroup, false);
				return;
			}
			if (intFirstTimeRes == Input_Err)
			{
				AddMsgToQueue(GlobalMsg["strInputErr"], eve.fromGroup, false);
				return;
			}
			if (intFirstTimeRes == ZeroDice_Err)
			{
				AddMsgToQueue(GlobalMsg["strZeroDiceErr"], eve.fromGroup, false);
				return;
			}
			if (intFirstTimeRes == ZeroType_Err)
			{
				AddMsgToQueue(GlobalMsg["strZeroTypeErr"], eve.fromGroup, false);
				return;
			}
			if (intFirstTimeRes == DiceTooBig_Err)
			{
				AddMsgToQueue(GlobalMsg["strDiceTooBigErr"], eve.fromGroup, false);
				return;
			}
			if (intFirstTimeRes == TypeTooBig_Err)
			{
				AddMsgToQueue(GlobalMsg["strTypeTooBigErr"], eve.fromGroup, false);
				return;
			}
			if (intFirstTimeRes == AddDiceVal_Err)
			{
				AddMsgToQueue(GlobalMsg["strAddDiceValErr"], eve.fromGroup, false);
				return;
			}
			if (intFirstTimeRes != 0)
			{
				AddMsgToQueue(GlobalMsg["strUnknownErr"], eve.fromGroup, false);
				return;
			}
			ilInitList->insert(eve.fromGroup, initdice.intTotal, strname);
			const string strReply = strname + "���ȹ����㣺" + strinit + '=' + to_string(initdice.intTotal);
			AddMsgToQueue(strReply, eve.fromGroup, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 4) == "init")
		{
			intMsgCnt += 4;
			string strReply;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))intMsgCnt++;
			if (strLowerMessage.substr(intMsgCnt, 3) == "clr")
			{
				if (ilInitList->clear(eve.fromGroup))
					strReply = "�ɹ�����ȹ���¼��";
				else
					strReply = "�б�Ϊ�գ�";
				AddMsgToQueue(strReply, eve.fromGroup, false);
				return;
			}
			ilInitList->show(eve.fromGroup, strReply);
			AddMsgToQueue(strReply, eve.fromGroup, false);
		}
	else if (strLowerMessage[intMsgCnt] == 'w')
		{
			intMsgCnt++;
			bool boolDetail = false;
			if (strLowerMessage[intMsgCnt] == 'w')
			{
				intMsgCnt++;
				boolDetail = true;
			}
			bool isHidden = false;
			if (strLowerMessage[intMsgCnt] == 'h')
			{
				isHidden = true;
				intMsgCnt += 1;
			}
			while (isspace(static_cast<unsigned char>(eve.message[intMsgCnt])))
				intMsgCnt++;

			int intTmpMsgCnt;
			for (intTmpMsgCnt = intMsgCnt; intTmpMsgCnt != eve.message.length() && eve.message[intTmpMsgCnt] != ' ';
				intTmpMsgCnt++)
			{
				if (!isdigit(static_cast<unsigned char>(strLowerMessage[intTmpMsgCnt])) && strLowerMessage[intTmpMsgCnt] != 'd' && strLowerMessage[
					intTmpMsgCnt] != 'k' && strLowerMessage[intTmpMsgCnt] != 'p' && strLowerMessage[intTmpMsgCnt] != 'b'
						&&
						strLowerMessage[intTmpMsgCnt] != 'f' && strLowerMessage[intTmpMsgCnt] != '+' && strLowerMessage[
							intTmpMsgCnt
						] != '-' && strLowerMessage[intTmpMsgCnt] != '#' && strLowerMessage[intTmpMsgCnt] != 'a')
				{
					break;
				}
			}
			string strMainDice = strLowerMessage.substr(intMsgCnt, intTmpMsgCnt - intMsgCnt);
			while (isspace(static_cast<unsigned char>(eve.message[intTmpMsgCnt])))
				intTmpMsgCnt++;
			string strReason = eve.message.substr(intTmpMsgCnt);


			int intTurnCnt = 1;
			if (strMainDice.find("#") != string::npos)
			{
				string strTurnCnt = strMainDice.substr(0, strMainDice.find("#"));
				if (strTurnCnt.empty())
					strTurnCnt = "1";
				strMainDice = strMainDice.substr(strMainDice.find("#") + 1);
				RD rdTurnCnt(strTurnCnt, eve.fromQQ);
				const int intRdTurnCntRes = rdTurnCnt.Roll();
				if (intRdTurnCntRes == Value_Err)
				{
					AddMsgToQueue(GlobalMsg["strValueErr"], eve.fromGroup, false);
					return;
				}
				if (intRdTurnCntRes == Input_Err)
				{
					AddMsgToQueue(GlobalMsg["strInputErr"], eve.fromGroup, false);
					return;
				}
				if (intRdTurnCntRes == ZeroDice_Err)
				{
					AddMsgToQueue(GlobalMsg["strZeroDiceErr"], eve.fromGroup, false);
					return;
				}
				if (intRdTurnCntRes == ZeroType_Err)
				{
					AddMsgToQueue(GlobalMsg["strZeroTypeErr"], eve.fromGroup, false);
					return;
				}
				if (intRdTurnCntRes == DiceTooBig_Err)
				{
					AddMsgToQueue(GlobalMsg["strDiceTooBigErr"], eve.fromGroup, false);
					return;
				}
				if (intRdTurnCntRes == TypeTooBig_Err)
				{
					AddMsgToQueue(GlobalMsg["strTypeTooBigErr"], eve.fromGroup, false);
					return;
				}
				if (intRdTurnCntRes == AddDiceVal_Err)
				{
					AddMsgToQueue(GlobalMsg["strAddDiceValErr"], eve.fromGroup, false);
					return;
				}
				if (intRdTurnCntRes != 0)
				{
					AddMsgToQueue(GlobalMsg["strUnknownErr"], eve.fromGroup, false);
					return;
				}
				if (rdTurnCnt.intTotal > 10)
				{
					AddMsgToQueue(GlobalMsg["strRollTimeExceeded"], eve.fromGroup, false);
					return;
				}
				if (rdTurnCnt.intTotal <= 0)
				{
					AddMsgToQueue(GlobalMsg["strRollTimeErr"], eve.fromGroup, false);
					return;
				}
				intTurnCnt = rdTurnCnt.intTotal;
				if (strTurnCnt.find("d") != string::npos)
				{
					string strTurnNotice = strNickName + "����������: " + rdTurnCnt.FormShortString() + "��";
					if (!isHidden)
					{
						AddMsgToQueue(strTurnNotice, eve.fromGroup, false);
					}
					else
					{
						strTurnNotice = "��Ⱥ\"" + getGroupList()[eve.fromGroup] + "\"�� " + strTurnNotice;
						AddMsgToQueue(strTurnNotice, eve.fromQQ);
						const auto range = ObserveGroup.equal_range(eve.fromGroup);
						for (auto it = range.first; it != range.second; ++it)
						{
							if (it->second != eve.fromQQ)
							{
								AddMsgToQueue(strTurnNotice, it->second);
							}
						}
					}
				}
			}
			if (strMainDice.empty())
			{
				AddMsgToQueue(GlobalMsg["strEmptyWWDiceErr"], eve.fromGroup, false);
				return;
			}
			string strFirstDice = strMainDice.substr(0, strMainDice.find('+') < strMainDice.find('-')
				? strMainDice.find('+')
				: strMainDice.find('-'));
			bool boolAdda10 = true;
			for (auto i : strFirstDice)
			{
				if (!isdigit(static_cast<unsigned char>(i)))
				{
					boolAdda10 = false;
					break;
				}
			}
			if (boolAdda10)
				strMainDice.insert(strFirstDice.length(), "a10");
			RD rdMainDice(strMainDice, eve.fromQQ);

			const int intFirstTimeRes = rdMainDice.Roll();
			if (intFirstTimeRes == Value_Err)
			{
				AddMsgToQueue(GlobalMsg["strValueErr"], eve.fromGroup, false);
				return;
			}
			if (intFirstTimeRes == Input_Err)
			{
				AddMsgToQueue(GlobalMsg["strInputErr"], eve.fromGroup, false);
				return;
			}
			if (intFirstTimeRes == ZeroDice_Err)
			{
				AddMsgToQueue(GlobalMsg["strZeroDiceErr"], eve.fromGroup, false);
				return;
			}
			else
			{
				if (intFirstTimeRes == ZeroType_Err)
				{
					AddMsgToQueue(GlobalMsg["strZeroTypeErr"], eve.fromGroup, false);
					return;
				}
				if (intFirstTimeRes == DiceTooBig_Err)
				{
					AddMsgToQueue(GlobalMsg["strDiceTooBigErr"], eve.fromGroup, false);
					return;
				}
				if (intFirstTimeRes == TypeTooBig_Err)
				{
					AddMsgToQueue(GlobalMsg["strTypeTooBigErr"], eve.fromGroup, false);
					return;
				}
				if (intFirstTimeRes == AddDiceVal_Err)
				{
					AddMsgToQueue(GlobalMsg["strAddDiceValErr"], eve.fromGroup, false);
					return;
				}
				if (intFirstTimeRes != 0)
				{
					AddMsgToQueue(GlobalMsg["strUnknownErr"], eve.fromGroup, false);
					return;
				}
			}
			if (!boolDetail && intTurnCnt != 1)
			{
				string strAns = strNickName + "������: " + to_string(intTurnCnt) + "��" + rdMainDice.strDice + ": { ";
				if (!strReason.empty())
					strAns.insert(0, "����" + strReason + " ");
				vector<int> vintExVal;
				while (intTurnCnt--)
				{
					// �˴�����ֵ����
					// ReSharper disable once CppExpressionWithoutSideEffects
					rdMainDice.Roll();
					strAns += to_string(rdMainDice.intTotal);
					if (intTurnCnt != 0)
						strAns += ",";
					if ((rdMainDice.strDice == "D100" || rdMainDice.strDice == "1D100") && (rdMainDice.intTotal <= 5 ||
						rdMainDice.intTotal >= 96))
						vintExVal.push_back(rdMainDice.intTotal);
				}
				strAns += " }";
				if (!vintExVal.empty())
				{
					strAns += ",��ֵ: ";
					for (auto it = vintExVal.cbegin(); it != vintExVal.cend(); ++it)
					{
						strAns += to_string(*it);
						if (it != vintExVal.cend() - 1)
							strAns += ",";
					}
				}
				if (!isHidden)
				{
					AddMsgToQueue(strAns, eve.fromGroup, false);
				}
				else
				{
					strAns = "��Ⱥ\"" + getGroupList()[eve.fromGroup] + "\"�� " + strAns;
					AddMsgToQueue(strAns, eve.fromQQ);
					const auto range = ObserveGroup.equal_range(eve.fromGroup);
					for (auto it = range.first; it != range.second; ++it)
					{
						if (it->second != eve.fromQQ)
						{
							AddMsgToQueue(strAns, it->second);
						}
					}
				}
			}
			else
			{
				while (intTurnCnt--)
				{
					// �˴�����ֵ����
					// ReSharper disable once CppExpressionWithoutSideEffects
					rdMainDice.Roll();
					string strAns = strNickName + "������: " + (boolDetail
						? rdMainDice.FormCompleteString()
						: rdMainDice.FormShortString());
					if (!strReason.empty())
						strAns.insert(0, "����" + strReason + " ");
					if (!isHidden)
					{
						AddMsgToQueue(strAns, eve.fromGroup, false);
					}
					else
					{
						strAns = "��Ⱥ\"" + getGroupList()[eve.fromGroup] + "\"�� " + strAns;
						AddMsgToQueue(strAns, eve.fromQQ);
						const auto range = ObserveGroup.equal_range(eve.fromGroup);
						for (auto it = range.first; it != range.second; ++it)
						{
							if (it->second != eve.fromQQ)
							{
								AddMsgToQueue(strAns, it->second);
							}
						}
					}
				}
			}
			if (isHidden)
			{
				const string strReply = strNickName + "������һ�ΰ���";
				AddMsgToQueue(strReply, eve.fromGroup, false);
			}
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "ob")
		{
			intMsgCnt += 2;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			const string Command = strLowerMessage.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) - intMsgCnt);
			if (Command == "on")
			{
				if (eve.fromQQ == MASTER || getGroupMemberInfo(eve.fromGroup, eve.fromQQ).permissions >= 2)
				{
					if (DisabledOBGroup.count(eve.fromGroup))
					{
						DisabledOBGroup.erase(eve.fromGroup);
						AddMsgToQueue("�ɹ��ڱ�Ⱥ�������Թ�ģʽ!", eve.fromGroup, false);
					}
					else
					{
						AddMsgToQueue("��Ⱥ���Թ�ģʽû�б�����!", eve.fromGroup, false);
					}
				}
				else
				{
					AddMsgToQueue("��û��Ȩ��ִ�д�����!", eve.fromGroup, false);
				}
				return;
			}
			if (Command == "off")
			{
				if (eve.fromQQ == MASTER || getGroupMemberInfo(eve.fromGroup, eve.fromQQ).permissions >= 2)
				{
					if (!DisabledOBGroup.count(eve.fromGroup))
					{
						DisabledOBGroup.insert(eve.fromGroup);
						ObserveGroup.clear();
						AddMsgToQueue("�ɹ��ڱ�Ⱥ�н����Թ�ģʽ!", eve.fromGroup, false);
					}
					else
					{
						AddMsgToQueue("��Ⱥ���Թ�ģʽû�б�����!", eve.fromGroup, false);
					}
				}
				else
				{
					AddMsgToQueue("��û��Ȩ��ִ�д�����!", eve.fromGroup, false);
				}
				return;
			}
			if (DisabledOBGroup.count(eve.fromGroup))
			{
				AddMsgToQueue("�ڱ�Ⱥ���Թ�ģʽ�ѱ�����!", eve.fromGroup, false);
				return;
			}
			if (Command == "list")
			{
				string Msg = "��ǰ���Թ�����:";
				const auto Range = ObserveGroup.equal_range(eve.fromGroup);
				for (auto it = Range.first; it != Range.second; ++it)
				{
					Msg += "\n" + getName(it->second, eve.fromGroup) + "(" + to_string(it->second) + ")";
				}
				const string strReply = Msg == "��ǰ���Թ�����:" ? "��ǰ�����Թ���" : Msg;
				AddMsgToQueue(strReply, eve.fromGroup, false);
			}
			else if (Command == "clr")
			{
				if (eve.fromQQ == MASTER || getGroupMemberInfo(eve.fromGroup, eve.fromQQ).permissions >= 2)
				{
					ObserveGroup.erase(eve.fromGroup);
					AddMsgToQueue("�ɹ�ɾ�������Թ���!", eve.fromGroup, false);
				}
				else
				{
					AddMsgToQueue("��û��Ȩ��ִ�д�����!", eve.fromGroup, false);
				}
			}
			else if (Command == "exit")
			{
				const auto Range = ObserveGroup.equal_range(eve.fromGroup);
				for (auto it = Range.first; it != Range.second; ++it)
				{
					if (it->second == eve.fromQQ)
					{
						ObserveGroup.erase(it);
						const string strReply = strNickName + "�ɹ��˳��Թ�ģʽ!";
						AddMsgToQueue(strReply, eve.fromGroup, false);
						eve.message_block();
						return;
					}
				}
				const string strReply = strNickName + "û�м����Թ�ģʽ!";
				AddMsgToQueue(strReply, eve.fromGroup, false);
			}
			else
			{
				const auto Range = ObserveGroup.equal_range(eve.fromGroup);
				for (auto it = Range.first; it != Range.second; ++it)
				{
					if (it->second == eve.fromQQ)
					{
						const string strReply = strNickName + "�Ѿ������Թ�ģʽ!";
						AddMsgToQueue(strReply, eve.fromGroup, false);
						eve.message_block();
						return;
					}
				}
				ObserveGroup.insert(make_pair(eve.fromGroup, eve.fromQQ));
				const string strReply = strNickName + "�ɹ������Թ�ģʽ!";
				AddMsgToQueue(strReply, eve.fromGroup, false);
			}
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "ti")
		{
			string strAns = strNickName + "�ķ����-��ʱ֢״:\n";
			TempInsane(strAns);
			AddMsgToQueue(strAns, eve.fromGroup, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "li")
		{
			string strAns = strNickName + "�ķ����-�ܽ�֢״:\n";
			LongInsane(strAns);
			AddMsgToQueue(strAns, eve.fromGroup, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "sc" && strLowerMessage.substr(intMsgCnt, 3) != "scp")
	{
		intMsgCnt += 2;
		while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			intMsgCnt++;
		string SanCost = strLowerMessage.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) - intMsgCnt);
		intMsgCnt += SanCost.length();
		while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			intMsgCnt++;
		string San;
		while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
		{
			San += strLowerMessage[intMsgCnt];
			intMsgCnt++;
		}
		if (SanCost.empty() || SanCost.find("/") == string::npos)
		{
			AddMsgToQueue(GlobalMsg["strSCInvalid"], eve.fromGroup, false);
			return;
		}
		if (San.empty() && !(CharacterProp.count(SourceType(eve.fromQQ, GroupT, eve.fromGroup)) && CharacterProp[
			SourceType(eve.fromQQ, GroupT, eve.fromGroup)].count("����")))
		{
			AddMsgToQueue(GlobalMsg["strSanInvalid"], eve.fromGroup, false);
			return;
		}
			for (const auto& character : SanCost.substr(0, SanCost.find("/")))
			{
				if (!isdigit(static_cast<unsigned char>(character)) && character != 'D' && character != 'd' && character != '+' && character != '-')
				{
					AddMsgToQueue(GlobalMsg["strSCInvalid"], eve.fromQQ, false);
					return;
				}
			}
			for (const auto& character : SanCost.substr(SanCost.find("/") + 1))
			{
				if (!isdigit(static_cast<unsigned char>(character)) && character != 'D' && character != 'd' && character != '+' && character != '-')
				{
					AddMsgToQueue(GlobalMsg["strSCInvalid"], eve.fromQQ, false);
					return;
				}
			}
			RD rdSuc(SanCost.substr(0, SanCost.find("/")));
			RD rdFail(SanCost.substr(SanCost.find("/") + 1));
			if (rdSuc.Roll() != 0 || rdFail.Roll() != 0)
			{
				AddMsgToQueue(GlobalMsg["strSCInvalid"], eve.fromGroup, false);
				return;
			}
			if (San.length() >= 3)
			{
				AddMsgToQueue(GlobalMsg["strSanInvalid"], eve.fromGroup, false);
				return;
			}
			const int intSan = San.empty() ? CharacterProp[SourceType(eve.fromQQ, GroupT, eve.fromGroup)]["����"] : stoi(San);
			if (intSan == 0)
			{
				AddMsgToQueue(GlobalMsg["strSanInvalid"], eve.fromGroup, false);
				return;
			}
			string strAns = strNickName + "��Sancheck:\n1D100=";
			const int intTmpRollRes = RandomGenerator::Randint(1, 100);
			strAns += to_string(intTmpRollRes);

			if (intTmpRollRes <= intSan)
			{
				strAns += " �ɹ�\n���Sanֵ����" + SanCost.substr(0, SanCost.find("/"));
				if (SanCost.substr(0, SanCost.find("/")).find("d") != string::npos)
					strAns += "=" + to_string(rdSuc.intTotal);
				strAns += +"��,��ǰʣ��" + to_string(max(0, intSan - rdSuc.intTotal)) + "��";
				if (San.empty())
				{
					CharacterProp[SourceType(eve.fromQQ, GroupT, eve.fromGroup)]["����"] = max(0, intSan - rdSuc.intTotal);
				}
			}
			else if (intTmpRollRes == 100 || (intSan < 50 && intTmpRollRes > 95))
			{
				strAns += " ��ʧ��\n���Sanֵ����" + SanCost.substr(SanCost.find("/") + 1);
				// ReSharper disable once CppExpressionWithoutSideEffects
				rdFail.Max();
				if (SanCost.substr(SanCost.find("/") + 1).find("d") != string::npos)
					strAns += "���ֵ=" + to_string(rdFail.intTotal);
				strAns += +"��,��ǰʣ��" + to_string(max(0, intSan - rdFail.intTotal)) + "��";
				if (San.empty())
				{
					CharacterProp[SourceType(eve.fromQQ, GroupT, eve.fromGroup)]["����"] = max(0, intSan - rdFail.intTotal);
				}
			}
			else
			{
				strAns += " ʧ��\n���Sanֵ����" + SanCost.substr(SanCost.find("/") + 1);
				if (SanCost.substr(SanCost.find("/") + 1).find("d") != string::npos)
					strAns += "=" + to_string(rdFail.intTotal);
				strAns += +"��,��ǰʣ��" + to_string(max(0, intSan - rdFail.intTotal)) + "��";
				if (San.empty())
				{
					CharacterProp[SourceType(eve.fromQQ, GroupT, eve.fromGroup)]["����"] = max(0, intSan - rdFail.intTotal);
				}
			}
			AddMsgToQueue(strAns, eve.fromGroup, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "en")
		{
			intMsgCnt += 2;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strSkillName;
			while (intMsgCnt != eve.message.length() && !isdigit(static_cast<unsigned char>(eve.message[intMsgCnt])) && !isspace(static_cast<unsigned char>(eve.message[intMsgCnt]))
				)
			{
				strSkillName += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
			while (isspace(static_cast<unsigned char>(eve.message[intMsgCnt])))
				intMsgCnt++;
			string strCurrentValue;
			while (isdigit(static_cast<unsigned char>(eve.message[intMsgCnt])))
			{
				strCurrentValue += eve.message[intMsgCnt];
				intMsgCnt++;
			}
			int intCurrentVal;
			if (strCurrentValue.empty())
			{
				if (CharacterProp.count(SourceType(eve.fromQQ, GroupT, eve.fromGroup)) && CharacterProp[SourceType(
					eve.fromQQ, GroupT, eve.fromGroup)].count(strSkillName))
				{
					intCurrentVal = CharacterProp[SourceType(eve.fromQQ, GroupT, eve.fromGroup)][strSkillName];
				}
				else if (SkillDefaultVal.count(strSkillName))
				{
					intCurrentVal = SkillDefaultVal[strSkillName];
				}
				else
				{
					AddMsgToQueue(GlobalMsg["strEnValInvalid"], eve.fromGroup, false);
					return;
				}
			}
			else
			{
				if (strCurrentValue.length() > 3)
				{
					AddMsgToQueue(GlobalMsg["strEnValInvalid"], eve.fromGroup, false);

					return;
				}
				intCurrentVal = stoi(strCurrentValue);
			}

			string strAns = strNickName + "��" + strSkillName + "��ǿ��ɳ��춨:\n1D100=";
			const int intTmpRollRes = RandomGenerator::Randint(1, 100);
			strAns += to_string(intTmpRollRes) + "/" + to_string(intCurrentVal);

			if (intTmpRollRes <= intCurrentVal && intTmpRollRes <= 95)
			{
				strAns += " ʧ��!\n���" + (strSkillName.empty() ? "���Ի���ֵ" : strSkillName) + "û�б仯!";
			}
			else
			{
				strAns += " �ɹ�!\n���" + (strSkillName.empty() ? "���Ի���ֵ" : strSkillName) + "����1D10=";
				const int intTmpRollD10 = RandomGenerator::Randint(1, 10);
				strAns += to_string(intTmpRollD10) + "��,��ǰΪ" + to_string(intCurrentVal + intTmpRollD10) + "��";
				if (strCurrentValue.empty())
				{
					CharacterProp[SourceType(eve.fromQQ, GroupT, eve.fromGroup)][strSkillName] = intCurrentVal +
						intTmpRollD10;
				}
			}
			AddMsgToQueue(strAns, eve.fromGroup, false);
		}
	//Fate
	else if (strLowerMessage.substr(intMsgCnt, 4) == "fate")
		{
			intMsgCnt += 4;
			while (isspace(strLowerMessage[intMsgCnt]))
				intMsgCnt++;
			const string Command = strLowerMessage.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) - intMsgCnt);
			if (Command == "on")
			{
				if (eve.fromQQ == MASTER || getGroupMemberInfo(eve.fromGroup, eve.fromQQ).permissions >= 2)
				{
					if (DisabledJRFATEGroup.count(eve.fromGroup))
					{
						DisabledJRFATEGroup.erase(eve.fromGroup);
						AddMsgToQueue("�ɹ��ڱ�Ⱥ������FATE!", eve.fromGroup, false);
					}
					else
					{
						AddMsgToQueue("�ڱ�Ⱥ��FATEû�б�����!", eve.fromGroup, false);
					}
				}
				else
				{
					AddMsgToQueue(GlobalMsg["strPermissionDeniedErr"], eve.fromGroup, false);
				}
				return;
			}
			if (Command == "off")
			{
				if (eve.fromQQ == MASTER || getGroupMemberInfo(eve.fromGroup, eve.fromQQ).permissions >= 2)
				{
					if (!DisabledJRFATEGroup.count(eve.fromGroup))
					{
						DisabledJRFATEGroup.insert(eve.fromGroup);
						AddMsgToQueue("�ɹ��ڱ�Ⱥ�н���FATE!", eve.fromGroup, false);
					}
					else
					{
						AddMsgToQueue("�ڱ�Ⱥ��FATEû�б�����!", eve.fromGroup, false);
					}
				}
				else
				{
					AddMsgToQueue(GlobalMsg["strPermissionDeniedErr"], eve.fromGroup, false);
				}
				return;
			}
			if (DisabledJRFATEGroup.count(eve.fromGroup))
			{
				AddMsgToQueue("�ڱ�Ⱥ��FATE�����ѱ�����", eve.fromGroup, false);
				return;
			}
			char cstrDate[100] = {};
			time_t time_tTime = 0;
			time(&time_tTime);
			tm tmTime{};
			localtime_s(&tmTime, &time_tTime);
			strftime(cstrDate, 100, "%F", &tmTime);
			if (JRFATE.count(eve.fromQQ) && JRFATE[eve.fromQQ].Date == cstrDate)
			{
				const string strReply = strNickName + " ���շ������� \n" + tarotCard[JRFATE[eve.fromQQ].FATEVal];
				AddMsgToQueue(strReply, eve.fromGroup, false);
			}
			else
			{
				int iFATE = RandomGenerator::Randint(1, 44);
				JRFATE[eve.fromQQ].Date = cstrDate;
				JRFATE[eve.fromQQ].FATEVal = iFATE;
				const string strReply(strNickName + " ���շ������� \n" + tarotCard[JRFATE[eve.fromQQ].FATEVal]);
				AddMsgToQueue(strReply, eve.fromGroup, false);
			}
		}
	else if (strLowerMessage.substr(intMsgCnt, 5) == "tarot")
		{
			const string strReply(strNickName + " �е������� \n" + tarotCard[RandomGenerator::Randint(1, 44)]);
			AddMsgToQueue(strReply, eve.fromGroup, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 3) == "scp")
	{
		intMsgCnt += 3;
		while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			intMsgCnt++;
		string strSCP = eve.message.substr(intMsgCnt);
		for (auto i : strSCP)
		{
			if (!isdigit(i))
			{
				AddMsgToQueue(GlobalMsg["strSCPErr"], eve.fromGroup, false);
				return;
			}
		}
		if (strSCP > "3999") 
		{
			AddMsgToQueue(GlobalMsg["strSCPErr"], eve.fromGroup, false);
			return;
		}
		if (strSCP.length() == 4) 
		{
			if (strSCP >= "1000" && strSCP <= "1999")
			{
				const string scpa = GlobalMsg["strSCP"] + strSCP;
				const string scpb = GlobalMsg["strSCPWeb"] + "-ii/scp-" + strSCP;
				AddMsgToQueue(scpa + "\n" + scpb, eve.fromGroup, false);
			}
			else if (strSCP >= "2000" && strSCP <= "2999")
			{
				const string scpa = GlobalMsg["strSCP"] + strSCP;
				const string scpb = GlobalMsg["strSCPWeb"] + "-iii/scp-" + strSCP;
				AddMsgToQueue(scpa + "\n" + scpb, eve.fromGroup, false);
			}
			else if (strSCP >= "3000" && strSCP <= "3999")
			{
				const string scpa = GlobalMsg["strSCP"] + strSCP;
				const string scpb = GlobalMsg["strSCPWeb"] + "-iv/scp-" + strSCP;
				AddMsgToQueue(scpa + "\n" + scpb, eve.fromGroup, false);
			}
		}
		else if (strSCP.length() == 3)
		{
			const string scpa = GlobalMsg["strSCP"] + strSCP;
			const string scpb = GlobalMsg["strSCPWeb"] + "-" + strSCP;
			AddMsgToQueue(scpa + "\n" + scpb, eve.fromGroup, false);
		}
		else if (strSCP.length() == 2)
		{
			const string scpa = GlobalMsg["strSCP"] + "0" + strSCP;
			const string scpb = GlobalMsg["strSCPWeb"] + "-i/scp-0" + strSCP;
			AddMsgToQueue(scpa + "\n" + scpb, eve.fromGroup, false);
		}
		else if (strSCP.length() == 1)
		{
			const string scpa = GlobalMsg["strSCP"] + "00" + strSCP;
			const string scpb = GlobalMsg["strSCPWeb"] + "-i/scp-00" + strSCP;
			AddMsgToQueue(scpa + "\n" + scpb, eve.fromGroup, false);
		}
	}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "nn")
		{
			intMsgCnt += 2;
			while (isspace(static_cast<unsigned char>(eve.message[intMsgCnt])))
				intMsgCnt++;
			string name = eve.message.substr(intMsgCnt);
			if (name.length() > 50)
			{
				AddMsgToQueue(GlobalMsg["strNameTooLongErr"], eve.fromGroup, false);
				return;
			}
			if (!name.empty())
			{
				Name->set(eve.fromGroup, eve.fromQQ, name);
				const string strReply = "�ѽ�" + strNickName + "�����Ƹ���Ϊ" + strip(name);
				AddMsgToQueue(strReply, eve.fromGroup, false);
			}
			else
			{
				if (Name->del(eve.fromGroup, eve.fromQQ))
				{
					const string strReply = "�ѽ�" + strNickName + "������ɾ��";
					AddMsgToQueue(strReply, eve.fromGroup, false);
				}
				else
				{
					const string strReply = strNickName + GlobalMsg["strNameDelErr"];
					AddMsgToQueue(strReply, eve.fromGroup, false);
				}
			}
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "me")
		{
			intMsgCnt += 2;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strAction = strLowerMessage.substr(intMsgCnt);
			if (strAction == "on")
			{
				if (eve.fromQQ == MASTER || getGroupMemberInfo(eve.fromGroup, eve.fromQQ).permissions >= 2)
				{
					if (DisabledMEGroup.count(eve.fromGroup))
					{
						DisabledMEGroup.erase(eve.fromGroup);
						AddMsgToQueue("�ɹ��ڱ�Ⱥ������.me����!", eve.fromGroup, false);
					}
					else
					{
						AddMsgToQueue("�ڱ�Ⱥ��.me����û�б�����!", eve.fromGroup, false);
					}
				}
				else
				{
					AddMsgToQueue(GlobalMsg["strPermissionDeniedErr"], eve.fromGroup, false);
				}
				return;
			}
			if (strAction == "off")
			{
				if (eve.fromQQ == MASTER || getGroupMemberInfo(eve.fromGroup, eve.fromQQ).permissions >= 2)
				{
					if (!DisabledMEGroup.count(eve.fromGroup))
					{
						DisabledMEGroup.insert(eve.fromGroup);
						AddMsgToQueue("�ɹ��ڱ�Ⱥ�н���.me����!", eve.fromGroup, false);
					}
					else
					{
						AddMsgToQueue("�ڱ�Ⱥ��.me����û�б�����!", eve.fromGroup, false);
					}
				}
				else
				{
					AddMsgToQueue(GlobalMsg["strPermissionDeniedErr"], eve.fromGroup, false);
				}
				return;
			}
			if (DisabledMEGroup.count(eve.fromGroup))
			{
				AddMsgToQueue("�ڱ�Ⱥ��.me�����ѱ�����!", eve.fromGroup, false);
				return;
			}
			if (DisabledMEGroup.count(eve.fromGroup))
			{
				AddMsgToQueue(GlobalMsg["strMEDisabledErr"], eve.fromGroup, false);
				return;
			}
			strAction = strip(eve.message.substr(intMsgCnt));
			if (strAction.empty())
			{
				AddMsgToQueue("��������Ϊ��!", eve.fromGroup, false);
				return;
			}
			const string strReply = strNickName + strAction;
			AddMsgToQueue(strReply, eve.fromGroup, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 3) == "set")
		{
			intMsgCnt += 3;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strDefaultDice = strLowerMessage.substr(intMsgCnt, strLowerMessage.find(" ", intMsgCnt) - intMsgCnt);
			while (strDefaultDice[0] == '0')
				strDefaultDice.erase(strDefaultDice.begin());
			if (strDefaultDice.empty())
				strDefaultDice = "100";
			for (auto charNumElement : strDefaultDice)
				if (!isdigit(static_cast<unsigned char>(charNumElement)))
				{
					AddMsgToQueue(GlobalMsg["strSetInvalid"], eve.fromGroup, false);
					return;
				}
			if (strDefaultDice.length() > 5)
			{
				AddMsgToQueue(GlobalMsg["strSetTooBig"], eve.fromGroup, false);
				return;
			}
			const int intDefaultDice = stoi(strDefaultDice);
			if (intDefaultDice == 100)
				DefaultDice.erase(eve.fromQQ);
			else
				DefaultDice[eve.fromQQ] = intDefaultDice;
			const string strSetSuccessReply = "�ѽ�" + strNickName + "��Ĭ�������͸���ΪD" + strDefaultDice;
			AddMsgToQueue(strSetSuccessReply, eve.fromGroup, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 4) == "coc6")
		{
			intMsgCnt += 4;
			if (strLowerMessage[intMsgCnt] == 's')
				intMsgCnt++;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strNum;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strNum += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (strNum.length() > 2)
			{
				AddMsgToQueue(GlobalMsg["strCharacterTooBig"], eve.fromQQ, false);
				return;
			}
			const int intNum = stoi(strNum.empty() ? "1" : strNum);
			if (intNum > 10)
			{
				AddMsgToQueue(GlobalMsg["strCharacterTooBig"], eve.fromGroup, false);
				return;
			}
			if (intNum == 0)
			{
				AddMsgToQueue(GlobalMsg["strCharacterCannotBeZero"], eve.fromGroup, false);
				return;
			}
			string strReply = strNickName;
			COC6(strReply, intNum);
			AddMsgToQueue(strReply, eve.fromGroup, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 3) == "dnd")
		{
			intMsgCnt += 3;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strNum;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strNum += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (strNum.length() > 2)
			{
				AddMsgToQueue(GlobalMsg["strCharacterTooBig"], eve.fromQQ, false);
				return;
			}
			const int intNum = stoi(strNum.empty() ? "1" : strNum);
			if (intNum > 10)
			{
				AddMsgToQueue(GlobalMsg["strCharacterTooBig"], eve.fromGroup, false);
				return;
			}
			if (intNum == 0)
			{
				AddMsgToQueue(GlobalMsg["strCharacterCannotBeZero"], eve.fromGroup, false);
				return;
			}
			string strReply = strNickName;
			DND(strReply, intNum);
			AddMsgToQueue(strReply, eve.fromGroup, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 3) == "coc")
		{
			intMsgCnt += 3;
			if (strLowerMessage[intMsgCnt] == '7')
				intMsgCnt++;
			if (strLowerMessage[intMsgCnt] == 's')
				intMsgCnt++;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strNum;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strNum += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (strNum.length() > 2)
			{
				AddMsgToQueue(GlobalMsg["strCharacterTooBig"], eve.fromQQ, false);
				return;
			}
			const int intNum = stoi(strNum.empty() ? "1" : strNum);
			if (intNum > 10)
			{
				AddMsgToQueue(GlobalMsg["strCharacterTooBig"], eve.fromGroup, false);
				return;
			}
			if (intNum == 0)
			{
				AddMsgToQueue(GlobalMsg["strCharacterCannotBeZero"], eve.fromGroup, false);
				return;
			}
			string strReply = strNickName;
			COC7(strReply, intNum);
			AddMsgToQueue(strReply, eve.fromGroup, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "ra")
		{
			intMsgCnt += 2;
			string strSkillName;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))intMsgCnt++;
			while (intMsgCnt != strLowerMessage.length() && !isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !
				isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && strLowerMessage[intMsgCnt] != '=' && strLowerMessage[intMsgCnt] !=
				':')
			{
				strSkillName += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) || strLowerMessage[intMsgCnt] == '=' || strLowerMessage[intMsgCnt] ==
				':')intMsgCnt++;
			string strSkillVal;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strSkillVal += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				intMsgCnt++;
			}
			string strReason = eve.message.substr(intMsgCnt);
			int intSkillVal;
			if (strSkillVal.empty())
			{
				if (CharacterProp.count(SourceType(eve.fromQQ, GroupT, eve.fromGroup)) && CharacterProp[SourceType(
					eve.fromQQ, GroupT, eve.fromGroup)].count(strSkillName))
				{
					intSkillVal = CharacterProp[SourceType(eve.fromQQ, GroupT, eve.fromGroup)][strSkillName];
				}
				else if (SkillDefaultVal.count(strSkillName))
				{
					intSkillVal = SkillDefaultVal[strSkillName];
				}
				else
				{
					AddMsgToQueue(GlobalMsg["strUnknownPropErr"], eve.fromGroup, false);
					return;
				}
			}
			else if (strSkillVal.length() > 3)
			{
				AddMsgToQueue(GlobalMsg["strPropErr"], eve.fromGroup, false);
				return;
			}
			else
			{
				intSkillVal = stoi(strSkillVal);
			}
			const int intD100Res = RandomGenerator::Randint(1, 100);
			string strReply = strNickName + "����" + strSkillName + "�춨: D100=" + to_string(intD100Res) + "/" +
				to_string(intSkillVal) + " ";
			if (intD100Res <= 5)strReply += "��ɹ�";
			else if (intD100Res <= intSkillVal / 5)strReply += "���ѳɹ�";
			else if (intD100Res <= intSkillVal / 2)strReply += "���ѳɹ�";
			else if (intD100Res <= intSkillVal)strReply += "�ɹ�";
			else if (intD100Res <= 95)strReply += "ʧ��";
			else strReply += "��ʧ��";
			if (!strReason.empty())
			{
				strReply = "����" + strReason + " " + strReply;
			}
			AddMsgToQueue(strReply, eve.fromGroup, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "rc")
		{
			intMsgCnt += 2;
			string strSkillName;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))intMsgCnt++;
			while (intMsgCnt != strLowerMessage.length() && !isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !
				isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && strLowerMessage[intMsgCnt] != '=' && strLowerMessage[intMsgCnt] !=
				':')
			{
				strSkillName += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) || strLowerMessage[intMsgCnt] == '=' || strLowerMessage[intMsgCnt] ==
				':')intMsgCnt++;
			string strSkillVal;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strSkillVal += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				intMsgCnt++;
			}
			string strReason = eve.message.substr(intMsgCnt);
			int intSkillVal;
			if (strSkillVal.empty())
			{
				if (CharacterProp.count(SourceType(eve.fromQQ, GroupT, eve.fromGroup)) && CharacterProp[SourceType(
					eve.fromQQ, GroupT, eve.fromGroup)].count(strSkillName))
				{
					intSkillVal = CharacterProp[SourceType(eve.fromQQ, GroupT, eve.fromGroup)][strSkillName];
				}
				else if (SkillDefaultVal.count(strSkillName))
				{
					intSkillVal = SkillDefaultVal[strSkillName];
				}
				else
				{
					AddMsgToQueue(GlobalMsg["strUnknownPropErr"], eve.fromGroup, false);
					return;
				}
			}
			else if (strSkillVal.length() > 3)
			{
				AddMsgToQueue(GlobalMsg["strPropErr"], eve.fromGroup, false);
				return;
			}
			else
			{
				intSkillVal = stoi(strSkillVal);
			}
			const int intD100Res = RandomGenerator::Randint(1, 100);
			string strReply = strNickName + "����" + strSkillName + "�춨: D100=" + to_string(intD100Res) + "/" +
				to_string(intSkillVal) + " ";
			if (intD100Res == 1)strReply += "��ɹ�";
			else if (intD100Res <= intSkillVal / 5)strReply += "���ѳɹ�";
			else if (intD100Res <= intSkillVal / 2)strReply += "���ѳɹ�";
			else if (intD100Res <= intSkillVal)strReply += "�ɹ�";
			else if (intD100Res <= 95 || (intSkillVal >= 50 && intD100Res != 100))strReply += "ʧ��";
			else strReply += "��ʧ��";
			if (!strReason.empty())
			{
				strReply = "����" + strReason + " " + strReply;
			}
			AddMsgToQueue(strReply, eve.fromGroup, false);
		}
	else if (strLowerMessage[intMsgCnt] == 'r' ||strLowerMessage[intMsgCnt] == 'h')
		{
			bool isHidden = false;
			if (strLowerMessage[intMsgCnt] == 'h')
				isHidden = true;
			intMsgCnt += 1;
			bool boolDetail = true;
			if (eve.message[intMsgCnt] == 's')
			{
				boolDetail = false;
				intMsgCnt++;
			}
			if (strLowerMessage[intMsgCnt] == 'h')
			{
				isHidden = true;
				intMsgCnt += 1;
			}
			while (isspace(static_cast<unsigned char>(eve.message[intMsgCnt])))
				intMsgCnt++;
			string strMainDice;
			string strReason;
			bool tmpContainD = false;
			int intTmpMsgCnt;
			for (intTmpMsgCnt = intMsgCnt; intTmpMsgCnt != eve.message.length() && eve.message[intTmpMsgCnt] != ' ';
				intTmpMsgCnt++)
			{
				if (strLowerMessage[intTmpMsgCnt] == 'd' || strLowerMessage[intTmpMsgCnt] == 'p' || strLowerMessage[
					intTmpMsgCnt] == 'b' || strLowerMessage[intTmpMsgCnt] == '#' || strLowerMessage[intTmpMsgCnt] == 'f'
						||
						strLowerMessage[intTmpMsgCnt] == 'a')
					tmpContainD = true;
					if (!isdigit(static_cast<unsigned char>(strLowerMessage[intTmpMsgCnt])) && strLowerMessage[intTmpMsgCnt] != 'd' && strLowerMessage[
						intTmpMsgCnt] != 'k' && strLowerMessage[intTmpMsgCnt] != 'p' && strLowerMessage[intTmpMsgCnt] != 'b'
							&&
							strLowerMessage[intTmpMsgCnt] != 'f' && strLowerMessage[intTmpMsgCnt] != '+' && strLowerMessage[
								intTmpMsgCnt
							] != '-' && strLowerMessage[intTmpMsgCnt] != '#' && strLowerMessage[intTmpMsgCnt] != 'a')
					{
						break;
					}
			}
			if (tmpContainD)
			{
				strMainDice = strLowerMessage.substr(intMsgCnt, intTmpMsgCnt - intMsgCnt);
				while (isspace(static_cast<unsigned char>(eve.message[intTmpMsgCnt])))
					intTmpMsgCnt++;
				strReason = eve.message.substr(intTmpMsgCnt);
			}
			else
				strReason = eve.message.substr(intMsgCnt);

			int intTurnCnt = 1;
			if (strMainDice.find("#") != string::npos)
			{
				string strTurnCnt = strMainDice.substr(0, strMainDice.find("#"));
				if (strTurnCnt.empty())
					strTurnCnt = "1";
				strMainDice = strMainDice.substr(strMainDice.find("#") + 1);
				RD rdTurnCnt(strTurnCnt, eve.fromQQ);
				const int intRdTurnCntRes = rdTurnCnt.Roll();
				if (intRdTurnCntRes == Value_Err)
				{
					AddMsgToQueue(GlobalMsg["strValueErr"], eve.fromGroup, false);
					return;
				}
				if (intRdTurnCntRes == Input_Err)
				{
					AddMsgToQueue(GlobalMsg["strInputErr"], eve.fromGroup, false);
					return;
				}
				if (intRdTurnCntRes == ZeroDice_Err)
				{
					AddMsgToQueue(GlobalMsg["strZeroDiceErr"], eve.fromGroup, false);
					return;
				}
				if (intRdTurnCntRes == ZeroType_Err)
				{
					AddMsgToQueue(GlobalMsg["strZeroTypeErr"], eve.fromGroup, false);
					return;
				}
				if (intRdTurnCntRes == DiceTooBig_Err)
				{
					AddMsgToQueue(GlobalMsg["strDiceTooBigErr"], eve.fromGroup, false);
					return;
				}
				if (intRdTurnCntRes == TypeTooBig_Err)
				{
					AddMsgToQueue(GlobalMsg["strTypeTooBigErr"], eve.fromGroup, false);
					return;
				}
				if (intRdTurnCntRes == AddDiceVal_Err)
				{
					AddMsgToQueue(GlobalMsg["strAddDiceValErr"], eve.fromGroup, false);
					return;
				}
				if (intRdTurnCntRes != 0)
				{
					AddMsgToQueue(GlobalMsg["strUnknownErr"], eve.fromGroup, false);
					return;
				}
				if (rdTurnCnt.intTotal > 10)
				{
					AddMsgToQueue(GlobalMsg["strRollTimeExceeded"], eve.fromGroup, false);
					return;
				}
				if (rdTurnCnt.intTotal <= 0)
				{
					AddMsgToQueue(GlobalMsg["strRollTimeErr"], eve.fromGroup, false);
					return;
				}
				intTurnCnt = rdTurnCnt.intTotal;
				if (strTurnCnt.find("d") != string::npos)
				{
					string strTurnNotice = strNickName + "����������: " + rdTurnCnt.FormShortString() + "��";
					if (!isHidden)
					{
						AddMsgToQueue(strTurnNotice, eve.fromGroup, false);
					}
					else
					{
						strTurnNotice = "��Ⱥ\"" + getGroupList()[eve.fromGroup] + "\"�� " + strTurnNotice;
						AddMsgToQueue(strTurnNotice, eve.fromQQ);
						const auto range = ObserveGroup.equal_range(eve.fromGroup);
						for (auto it = range.first; it != range.second; ++it)
						{
							if (it->second != eve.fromQQ)
							{
								AddMsgToQueue(strTurnNotice, it->second);
							}
						}
					}
				}
			}
			RD rdMainDice(strMainDice, eve.fromQQ);

			const int intFirstTimeRes = rdMainDice.Roll();
			if (intFirstTimeRes == Value_Err)
			{
				AddMsgToQueue(GlobalMsg["strValueErr"], eve.fromGroup, false);
				return;
			}
			if (intFirstTimeRes == Input_Err)
			{
				AddMsgToQueue(GlobalMsg["strInputErr"], eve.fromGroup, false);
				return;
			}
			if (intFirstTimeRes == ZeroDice_Err)
			{
				AddMsgToQueue(GlobalMsg["strZeroDiceErr"], eve.fromGroup, false);
				return;
			}
			if (intFirstTimeRes == ZeroType_Err)
			{
				AddMsgToQueue(GlobalMsg["strZeroTypeErr"], eve.fromGroup, false);
				return;
			}
			if (intFirstTimeRes == DiceTooBig_Err)
			{
				AddMsgToQueue(GlobalMsg["strDiceTooBigErr"], eve.fromGroup, false);
				return;
			}
			if (intFirstTimeRes == TypeTooBig_Err)
			{
				AddMsgToQueue(GlobalMsg["strTypeTooBigErr"], eve.fromGroup, false);
				return;
			}
			if (intFirstTimeRes == AddDiceVal_Err)
			{
				AddMsgToQueue(GlobalMsg["strAddDiceValErr"], eve.fromGroup, false);
				return;
			}
			if (intFirstTimeRes != 0)
			{
				AddMsgToQueue(GlobalMsg["strUnknownErr"], eve.fromGroup, false);
				return;
			}
			if (!boolDetail && intTurnCnt != 1)
			{
				string strAns = strNickName + "������: " + to_string(intTurnCnt) + "��" + rdMainDice.strDice + ": { ";
				if (!strReason.empty())
					strAns.insert(0, "����" + strReason + " ");
				vector<int> vintExVal;
				while (intTurnCnt--)
				{
					// �˴�����ֵ����
					// ReSharper disable once CppExpressionWithoutSideEffects
					rdMainDice.Roll();
					strAns += to_string(rdMainDice.intTotal);
					if (intTurnCnt != 0)
						strAns += ",";
					if ((rdMainDice.strDice == "D100" || rdMainDice.strDice == "1D100") && (rdMainDice.intTotal <= 5 ||
						rdMainDice.intTotal >= 96))
						vintExVal.push_back(rdMainDice.intTotal);
				}
				strAns += " }";
				if (!vintExVal.empty())
				{
					strAns += ",��ֵ: ";
					for (auto it = vintExVal.cbegin(); it != vintExVal.cend(); ++it)
					{
						strAns += to_string(*it);
						if (it != vintExVal.cend() - 1)
							strAns += ",";
					}
				}
				if (!isHidden)
				{
					AddMsgToQueue(strAns, eve.fromGroup, false);
				}
				else
				{
					strAns = "��Ⱥ\"" + getGroupList()[eve.fromGroup] + "\"�� " + strAns;
					AddMsgToQueue(strAns, eve.fromQQ);
					const auto range = ObserveGroup.equal_range(eve.fromGroup);
					for (auto it = range.first; it != range.second; ++it)
					{
						if (it->second != eve.fromQQ)
						{
							AddMsgToQueue(strAns, it->second);
						}
					}
				}
			}
			else
			{
				while (intTurnCnt--)
				{
					// �˴�����ֵ����
					// ReSharper disable once CppExpressionWithoutSideEffects
					rdMainDice.Roll();
					string strAns = strNickName + "������: " + (boolDetail
						? rdMainDice.FormCompleteString()
						: rdMainDice.FormShortString());
					if (!strReason.empty())
						strAns.insert(0, "����" + strReason + " ");
					if (!isHidden)
					{
						AddMsgToQueue(strAns, eve.fromGroup, false);
					}
					else
					{
						strAns = "��Ⱥ\"" + getGroupList()[eve.fromGroup] + "\"�� " + strAns;
						AddMsgToQueue(strAns, eve.fromQQ);
						const auto range = ObserveGroup.equal_range(eve.fromGroup);
						for (auto it = range.first; it != range.second; ++it)
						{
							if (it->second != eve.fromQQ)
							{
								AddMsgToQueue(strAns, it->second);
							}
						}
					}
				}
			}
			if (isHidden)
			{
				const string strReply = strNickName + "������һ�ΰ���";
				AddMsgToQueue(strReply, eve.fromGroup, false);
			}
		}
}
EVE_DiscussMsg_EX(eventDiscussMsg)
{
	if (eve.isSystem())return;
	init(eve.message);
	string strAt = "[CQ:at,qq=" + to_string(getLoginQQ()) + "]";
	if (eve.message.substr(0, 6) == "[CQ:at")
	{
		if (eve.message.substr(0, strAt.length()) == strAt)
		{
			eve.message = eve.message.substr(strAt.length() + 1);
		}
		else
		{
			return;
		}
	}
	init2(eve.message);
	if (eve.message[0] != '.')
		return;
	int intMsgCnt = 1;
	while (isspace(static_cast<unsigned char>(eve.message[intMsgCnt])))
		intMsgCnt++;
	eve.message_block();
	const string strNickName = getName(eve.fromQQ, eve.fromDiscuss);
	string strLowerMessage = eve.message;
	transform(strLowerMessage.begin(), strLowerMessage.end(), strLowerMessage.begin(), [](unsigned char c) { return tolower(c); });
	if (BanFriendList.count(eve.fromQQ) && !strLowerMessage.empty())
	{
		const string strReply = "<" + eve.fromQQ + GlobalMsg["strBanned"];
		AddMsgToQueue(strReply, eve.fromDiscuss, false);
		return;
	}
	if (strLowerMessage.substr(intMsgCnt, 3) == "bot")
		{
			intMsgCnt += 3;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string Command;
			while (intMsgCnt != strLowerMessage.length() && !isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !isspace(
				static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				Command += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string QQNum = strLowerMessage.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) - intMsgCnt);
			if (Command == "on")
			{
				if (eve.fromQQ == 2558272027 || QQNum.empty() || QQNum == to_string(getLoginQQ()) || (QQNum.length() == 4 && QQNum == to_string(getLoginQQ() % 10000)))
				{
					if (DisabledDiscuss.count(eve.fromDiscuss))
					{
						DisabledDiscuss.erase(eve.fromDiscuss);
						AddMsgToQueue("�ɹ�������������!", eve.fromDiscuss, false);
					}
					else
					{
						AddMsgToQueue("���������Ѿ����ڿ���״̬!", eve.fromDiscuss, false);
					}
				}
			}
			else if (Command == "off")
			{
				if (eve.fromQQ == 2558272027 || QQNum.empty() || QQNum == to_string(getLoginQQ()) || (QQNum.length() == 4 && QQNum == to_string(getLoginQQ() % 10000)))
				{
					if (!DisabledDiscuss.count(eve.fromDiscuss))
					{
						DisabledDiscuss.insert(eve.fromDiscuss);
						AddMsgToQueue("�ɹ��رձ�������!", eve.fromDiscuss, false);
					}
					else
					{
						AddMsgToQueue("���������Ѿ����ڹر�״̬!", eve.fromDiscuss, false);
					}
				}
			}
			else
			{
				if (QQNum.empty() || QQNum == to_string(getLoginQQ()) || (QQNum.length() == 4 && QQNum == to_string(
					getLoginQQ() % 10000)))
				{
					AddMsgToQueue(Dice_Bot_Ver, eve.fromDiscuss, false);
				}
			}
			return;
		}
	if (strLowerMessage.substr(intMsgCnt, 4) == "quit")
		{
			intMsgCnt += 4;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string QQNum;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				QQNum += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (eve.fromQQ == 2558272027 || QQNum.empty() || (QQNum.length() == 4 && QQNum == to_string(getLoginQQ() % 10000)) || QQNum == to_string(getLoginQQ()))
			{
				setDiscussLeave(eve.fromDiscuss);
			}
			return;
		}
	if (strLowerMessage.substr(intMsgCnt, 4) == "help")
		{
		intMsgCnt += 4;
		while (strLowerMessage[intMsgCnt] == ' ')
			intMsgCnt++;
		const string strAction = strLowerMessage.substr(intMsgCnt);
		if (strAction == "list")
		{
			AddMsgToQueue(GlobalMsg["strHlpMsgList"], eve.fromDiscuss, false);
			return;
		}
			AddMsgToQueue(GlobalMsg["strHlpMsg"] + "\n�˰汾����ʱ�䣺" + __DATE__ + " " + __TIME__, eve.fromDiscuss, false);
		}
	if (DisabledDiscuss.count(eve.fromDiscuss))return;
	else if (strLowerMessage.substr(intMsgCnt, 2) == "st")
		{
			intMsgCnt += 2;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			if (intMsgCnt == strLowerMessage.length())
			{
				AddMsgToQueue(GlobalMsg["strStErr"], eve.fromDiscuss, false);
				return;
			}
			if (strLowerMessage.substr(intMsgCnt, 3) == "clr")
			{
				if (CharacterProp.count(SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)))
				{
					CharacterProp.erase(SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss));
				}
				AddMsgToQueue(GlobalMsg["strPropCleared"], eve.fromDiscuss, false);
				return;
			}
			if (strLowerMessage.substr(intMsgCnt, 3) == "del")
			{
				intMsgCnt += 3;
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
					intMsgCnt++;
				string strSkillName;
				while (intMsgCnt != strLowerMessage.length() && !isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !(strLowerMessage[
					intMsgCnt] == '|'))
				{
					strSkillName += strLowerMessage[intMsgCnt];
					intMsgCnt++;
				}
					if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
					if (CharacterProp.count(SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)) && CharacterProp[SourceType(
						eve.fromQQ, DiscussT, eve.fromDiscuss)].count(strSkillName))
					{
						CharacterProp[SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)].erase(strSkillName);
						AddMsgToQueue(GlobalMsg["strPropDeleted"], eve.fromDiscuss, false);
					}
					else
					{
						AddMsgToQueue(GlobalMsg["strPropNotFound"], eve.fromDiscuss, false);
					}
					return;
			}
			if (strLowerMessage.substr(intMsgCnt, 4) == "show")
			{
				intMsgCnt += 4;
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
					intMsgCnt++;
				string strSkillName;
				while (intMsgCnt != strLowerMessage.length() && !isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !(strLowerMessage[
					intMsgCnt] == '|'))
				{
					strSkillName += strLowerMessage[intMsgCnt];
					intMsgCnt++;
				}
					if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
					if (CharacterProp.count(SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)) && CharacterProp[SourceType(
						eve.fromQQ, DiscussT, eve.fromDiscuss)].count(strSkillName))
					{
						AddMsgToQueue(format(GlobalMsg["strProp"], {
							strNickName, strSkillName,
							to_string(CharacterProp[SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)][strSkillName])
							}), eve.fromDiscuss, false);
					}
					else if (SkillDefaultVal.count(strSkillName))
					{
						AddMsgToQueue(format(GlobalMsg["strProp"], { strNickName, strSkillName, to_string(SkillDefaultVal[strSkillName]) }),
							eve.fromDiscuss, false);
					}
					else
					{
						AddMsgToQueue(GlobalMsg["strPropNotFound"], eve.fromDiscuss, false);
					}
					return;
			}
			bool boolError = false;
			while (intMsgCnt != strLowerMessage.length())
			{
				string strSkillName;
				while (intMsgCnt != strLowerMessage.length() && !isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !
					isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && strLowerMessage[intMsgCnt] != '=' && strLowerMessage[intMsgCnt]
					!= ':')
				{
					strSkillName += strLowerMessage[intMsgCnt];
					intMsgCnt++;
				}
				if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) || strLowerMessage[intMsgCnt] == '=' || strLowerMessage[intMsgCnt
				] == ':')intMsgCnt++;
				string strSkillVal;
				while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				{
					strSkillVal += strLowerMessage[intMsgCnt];
					intMsgCnt++;
				}
				if (strSkillName.empty() || strSkillVal.empty() || strSkillVal.length() > 3)
				{
					boolError = true;
					break;
				}
				CharacterProp[SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)][strSkillName] = stoi(strSkillVal);
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) || strLowerMessage[intMsgCnt] == '|')intMsgCnt++;
			}
			if (boolError)
			{
				AddMsgToQueue(GlobalMsg["strPropErr"], eve.fromDiscuss, false);
			}
			else
			{
				AddMsgToQueue(GlobalMsg["strSetPropSuccess"], eve.fromDiscuss, false);
			}
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "ri")
		{
			intMsgCnt += 2;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))intMsgCnt++;
			string strinit = "D20";
			if (strLowerMessage[intMsgCnt] == '+' || strLowerMessage[intMsgCnt] == '-')
			{
				strinit += strLowerMessage[intMsgCnt];
				intMsgCnt++;
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))intMsgCnt++;
			}
			else if (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				strinit += '+';
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strinit += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				intMsgCnt++;
			}
			string strname = eve.message.substr(intMsgCnt);
			if (strname.empty())
				strname = strNickName;
			else
				strname = strip(strname);
			RD initdice(strinit);
			const int intFirstTimeRes = initdice.Roll();
			if (intFirstTimeRes == Value_Err)
			{
				AddMsgToQueue(GlobalMsg["strValueErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes == Input_Err)
			{
				AddMsgToQueue(GlobalMsg["strInputErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes == ZeroDice_Err)
			{
				AddMsgToQueue(GlobalMsg["strZeroDiceErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes == ZeroType_Err)
			{
				AddMsgToQueue(GlobalMsg["strZeroTypeErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes == DiceTooBig_Err)
			{
				AddMsgToQueue(GlobalMsg["strDiceTooBigErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes == TypeTooBig_Err)
			{
				AddMsgToQueue(GlobalMsg["strTypeTooBigErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes == AddDiceVal_Err)
			{
				AddMsgToQueue(GlobalMsg["strAddDiceValErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes != 0)
			{
				AddMsgToQueue(GlobalMsg["strUnknownErr"], eve.fromDiscuss, false);
				return;
			}
			ilInitList->insert(eve.fromDiscuss, initdice.intTotal, strname);
			const string strReply = strname + "���ȹ����㣺" + strinit + '=' + to_string(initdice.intTotal);
			AddMsgToQueue(strReply, eve.fromDiscuss, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 4) == "init")
		{
			intMsgCnt += 4;
			string strReply;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))intMsgCnt++;
			if (strLowerMessage.substr(intMsgCnt, 3) == "clr")
			{
				if (ilInitList->clear(eve.fromDiscuss))
					strReply = "�ɹ�����ȹ���¼��";
				else
					strReply = "�б�Ϊ�գ�";
				AddMsgToQueue(strReply, eve.fromDiscuss, false);
				return;
			}
			ilInitList->show(eve.fromDiscuss, strReply);
			AddMsgToQueue(strReply, eve.fromDiscuss, false);
		}
	else if (strLowerMessage[intMsgCnt] == 'w')
		{
			intMsgCnt++;
			bool boolDetail = false;
			if (strLowerMessage[intMsgCnt] == 'w')
			{
				intMsgCnt++;
				boolDetail = true;
			}
			bool isHidden = false;
			if (strLowerMessage[intMsgCnt] == 'h')
			{
				isHidden = true;
				intMsgCnt += 1;
			}
			while (isspace(static_cast<unsigned char>(eve.message[intMsgCnt])))
				intMsgCnt++;

			int intTmpMsgCnt;
			for (intTmpMsgCnt = intMsgCnt; intTmpMsgCnt != eve.message.length() && eve.message[intTmpMsgCnt] != ' ';
				intTmpMsgCnt++)
			{
				if (!isdigit(static_cast<unsigned char>(strLowerMessage[intTmpMsgCnt])) && strLowerMessage[intTmpMsgCnt] != 'd' && strLowerMessage[
					intTmpMsgCnt] != 'k' && strLowerMessage[intTmpMsgCnt] != 'p' && strLowerMessage[intTmpMsgCnt] != 'b'
						&&
						strLowerMessage[intTmpMsgCnt] != 'f' && strLowerMessage[intTmpMsgCnt] != '+' && strLowerMessage[
							intTmpMsgCnt
						] != '-' && strLowerMessage[intTmpMsgCnt] != '#' && strLowerMessage[intTmpMsgCnt] != 'a')
				{
					break;
				}
			}
			string strMainDice = strLowerMessage.substr(intMsgCnt, intTmpMsgCnt - intMsgCnt);
			while (isspace(static_cast<unsigned char>(eve.message[intTmpMsgCnt])))
				intTmpMsgCnt++;
			string strReason = eve.message.substr(intTmpMsgCnt);


			int intTurnCnt = 1;
			if (strMainDice.find("#") != string::npos)
			{
				string strTurnCnt = strMainDice.substr(0, strMainDice.find("#"));
				if (strTurnCnt.empty())
					strTurnCnt = "1";
				strMainDice = strMainDice.substr(strMainDice.find("#") + 1);
				RD rdTurnCnt(strTurnCnt, eve.fromQQ);
				const int intRdTurnCntRes = rdTurnCnt.Roll();
				if (intRdTurnCntRes == Value_Err)
				{
					AddMsgToQueue(GlobalMsg["strValueErr"], eve.fromDiscuss, false);
					return;
				}
				if (intRdTurnCntRes == Input_Err)
				{
					AddMsgToQueue(GlobalMsg["strInputErr"], eve.fromDiscuss, false);
					return;
				}
				if (intRdTurnCntRes == ZeroDice_Err)
				{
					AddMsgToQueue(GlobalMsg["strZeroDiceErr"], eve.fromDiscuss, false);
					return;
				}
				if (intRdTurnCntRes == ZeroType_Err)
				{
					AddMsgToQueue(GlobalMsg["strZeroTypeErr"], eve.fromDiscuss, false);
					return;
				}
				if (intRdTurnCntRes == DiceTooBig_Err)
				{
					AddMsgToQueue(GlobalMsg["strDiceTooBigErr"], eve.fromDiscuss, false);
					return;
				}
				if (intRdTurnCntRes == TypeTooBig_Err)
				{
					AddMsgToQueue(GlobalMsg["strTypeTooBigErr"], eve.fromDiscuss, false);
					return;
				}
				if (intRdTurnCntRes == AddDiceVal_Err)
				{
					AddMsgToQueue(GlobalMsg["strAddDiceValErr"], eve.fromDiscuss, false);
					return;
				}
				if (intRdTurnCntRes != 0)
				{
					AddMsgToQueue(GlobalMsg["strUnknownErr"], eve.fromDiscuss, false);
					return;
				}
				if (rdTurnCnt.intTotal > 10)
				{
					AddMsgToQueue(GlobalMsg["strRollTimeExceeded"], eve.fromDiscuss, false);
					return;
				}
				if (rdTurnCnt.intTotal <= 0)
				{
					AddMsgToQueue(GlobalMsg["strRollTimeErr"], eve.fromDiscuss, false);
					return;
				}
				intTurnCnt = rdTurnCnt.intTotal;
				if (strTurnCnt.find("d") != string::npos)
				{
					string strTurnNotice = strNickName + "����������: " + rdTurnCnt.FormShortString() + "��";
					if (!isHidden)
					{
						AddMsgToQueue(strTurnNotice, eve.fromDiscuss, false);
					}
					else
					{
						strTurnNotice = "�ڶ��������� " + strTurnNotice;
						AddMsgToQueue(strTurnNotice, eve.fromQQ);
						const auto range = ObserveDiscuss.equal_range(eve.fromDiscuss);
						for (auto it = range.first; it != range.second; ++it)
						{
							if (it->second != eve.fromQQ)
							{
								AddMsgToQueue(strTurnNotice, it->second);
							}
						}
					}
				}
			}
			string strFirstDice = strMainDice.substr(0, strMainDice.find('+') < strMainDice.find('-')
				? strMainDice.find('+')
				: strMainDice.find('-'));
			bool boolAdda10 = true;
			for (auto i : strFirstDice)
			{
				if (!isdigit(static_cast<unsigned char>(i)))
				{
					boolAdda10 = false;
					break;
				}
			}
			if (boolAdda10)
				strMainDice.insert(strFirstDice.length(), "a10");
			RD rdMainDice(strMainDice, eve.fromQQ);

			const int intFirstTimeRes = rdMainDice.Roll();
			if (intFirstTimeRes == Value_Err)
			{
				AddMsgToQueue(GlobalMsg["strValueErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes == Input_Err)
			{
				AddMsgToQueue(GlobalMsg["strInputErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes == ZeroDice_Err)
			{
				AddMsgToQueue(GlobalMsg["strZeroDiceErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes == ZeroType_Err)
			{
				AddMsgToQueue(GlobalMsg["strZeroTypeErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes == DiceTooBig_Err)
			{
				AddMsgToQueue(GlobalMsg["strDiceTooBigErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes == TypeTooBig_Err)
			{
				AddMsgToQueue(GlobalMsg["strTypeTooBigErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes == AddDiceVal_Err)
			{
				AddMsgToQueue(GlobalMsg["strAddDiceValErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes != 0)
			{
				AddMsgToQueue(GlobalMsg["strUnknownErr"], eve.fromDiscuss, false);
				return;
			}
			if (!boolDetail && intTurnCnt != 1)
			{
				string strAns = strNickName + "������: " + to_string(intTurnCnt) + "��" + rdMainDice.strDice + ": { ";
				if (!strReason.empty())
					strAns.insert(0, "����" + strReason + " ");
				vector<int> vintExVal;
				while (intTurnCnt--)
				{
					// �˴�����ֵ����
					// ReSharper disable once CppExpressionWithoutSideEffects
					rdMainDice.Roll();
					strAns += to_string(rdMainDice.intTotal);
					if (intTurnCnt != 0)
						strAns += ",";
					if ((rdMainDice.strDice == "D100" || rdMainDice.strDice == "1D100") && (rdMainDice.intTotal <= 5 ||
						rdMainDice.intTotal >= 96))
						vintExVal.push_back(rdMainDice.intTotal);
				}
				strAns += " }";
				if (!vintExVal.empty())
				{
					strAns += ",��ֵ: ";
					for (auto it = vintExVal.cbegin(); it != vintExVal.cend(); ++it)
					{
						strAns += to_string(*it);
						if (it != vintExVal.cend() - 1)
							strAns += ",";
					}
				}
				if (!isHidden)
				{
					AddMsgToQueue(strAns, eve.fromDiscuss, false);
				}
				else
				{
					strAns = "�ڶ��������� " + strAns;
					AddMsgToQueue(strAns, eve.fromQQ);
					const auto range = ObserveDiscuss.equal_range(eve.fromDiscuss);
					for (auto it = range.first; it != range.second; ++it)
					{
						if (it->second != eve.fromQQ)
						{
							AddMsgToQueue(strAns, it->second);
						}
					}
				}
			}
			else
			{
				while (intTurnCnt--)
				{
					// �˴�����ֵ����
					// ReSharper disable once CppExpressionWithoutSideEffects
					rdMainDice.Roll();
					string strAns = strNickName + "������: " + (boolDetail
						? rdMainDice.FormCompleteString()
						: rdMainDice.FormShortString());
					if (!strReason.empty())
						strAns.insert(0, "����" + strReason + " ");
					if (!isHidden)
					{
						AddMsgToQueue(strAns, eve.fromDiscuss, false);
					}
					else
					{
						strAns = "�ڶ��������� " + strAns;
						AddMsgToQueue(strAns, eve.fromQQ);
						const auto range = ObserveDiscuss.equal_range(eve.fromDiscuss);
						for (auto it = range.first; it != range.second; ++it)
						{
							if (it->second != eve.fromQQ)
							{
								AddMsgToQueue(strAns, it->second);
							}
						}
					}
				}
			}
			if (isHidden)
			{
				const string strReply = strNickName + "������һ�ΰ���";
				AddMsgToQueue(strReply, eve.fromDiscuss, false);
			}
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "ob")
		{
			intMsgCnt += 2;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			const string Command = strLowerMessage.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) - intMsgCnt);
			if (Command == "on")
			{
				if (DisabledOBDiscuss.count(eve.fromDiscuss))
				{
					DisabledOBDiscuss.erase(eve.fromDiscuss);
					AddMsgToQueue("�ɹ��ڱ����������������Թ�ģʽ!", eve.fromDiscuss, false);
				}
				else
				{
					AddMsgToQueue("�ڱ������������Թ�ģʽû�б�����!", eve.fromDiscuss, false);
				}
				return;
			}
			if (Command == "off")
			{
				if (!DisabledOBDiscuss.count(eve.fromDiscuss))
				{
					DisabledOBDiscuss.insert(eve.fromDiscuss);
					ObserveDiscuss.clear();
					AddMsgToQueue("�ɹ��ڱ����������н����Թ�ģʽ!", eve.fromDiscuss, false);
				}
				else
				{
					AddMsgToQueue("�ڱ������������Թ�ģʽû�б�����!", eve.fromDiscuss, false);
				}
				return;
			}
			if (DisabledOBDiscuss.count(eve.fromDiscuss))
			{
				AddMsgToQueue("�ڱ������������Թ�ģʽ�ѱ�����!", eve.fromDiscuss, false);
				return;
			}
			if (Command == "list")
			{
				string Msg = "��ǰ���Թ�����:";
				const auto Range = ObserveDiscuss.equal_range(eve.fromDiscuss);
				for (auto it = Range.first; it != Range.second; ++it)
				{
					Msg += "\n" + getName(it->second, eve.fromDiscuss) + "(" + to_string(it->second) + ")";
				}
				const string strReply = Msg == "��ǰ���Թ�����:" ? "��ǰ�����Թ���" : Msg;
				AddMsgToQueue(strReply, eve.fromDiscuss, false);
			}
			else if (Command == "clr")
			{
				ObserveDiscuss.erase(eve.fromDiscuss);
				AddMsgToQueue("�ɹ�ɾ�������Թ���!", eve.fromDiscuss, false);
			}
			else if (Command == "exit")
			{
				const auto Range = ObserveDiscuss.equal_range(eve.fromDiscuss);
				for (auto it = Range.first; it != Range.second; ++it)
				{
					if (it->second == eve.fromQQ)
					{
						ObserveDiscuss.erase(it);
						const string strReply = strNickName + "�ɹ��˳��Թ�ģʽ!";
						AddMsgToQueue(strReply, eve.fromDiscuss, false);
						eve.message_block();
						return;
					}
				}
				const string strReply = strNickName + "û�м����Թ�ģʽ!";
				AddMsgToQueue(strReply, eve.fromDiscuss, false);
			}
			else
			{
				const auto Range = ObserveDiscuss.equal_range(eve.fromDiscuss);
				for (auto it = Range.first; it != Range.second; ++it)
				{
					if (it->second == eve.fromQQ)
					{
						const string strReply = strNickName + "�Ѿ������Թ�ģʽ!";
						AddMsgToQueue(strReply, eve.fromDiscuss, false);
						eve.message_block();
						return;
					}
				}
				ObserveDiscuss.insert(make_pair(eve.fromDiscuss, eve.fromQQ));
				const string strReply = strNickName + "�ɹ������Թ�ģʽ!";
				AddMsgToQueue(strReply, eve.fromDiscuss, false);
			}
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "ti")
		{
			string strAns = strNickName + "�ķ����-��ʱ֢״:\n";
			TempInsane(strAns);
			AddMsgToQueue(strAns, eve.fromDiscuss, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "li")
		{
			string strAns = strNickName + "�ķ����-�ܽ�֢״:\n";
			LongInsane(strAns);
			AddMsgToQueue(strAns, eve.fromDiscuss, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "sc")
		{
			intMsgCnt += 2;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string SanCost = strLowerMessage.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) - intMsgCnt);
			intMsgCnt += SanCost.length();
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string San;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				San += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (SanCost.empty() || SanCost.find("/") == string::npos)
			{
				AddMsgToQueue(GlobalMsg["strSCInvalid"], eve.fromDiscuss, false);

				return;
			}
			if (San.empty() && !(CharacterProp.count(SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)) && CharacterProp[
				SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)].count("����")))
			{
				AddMsgToQueue(GlobalMsg["strSanInvalid"], eve.fromDiscuss, false);

				return;
			}
				for (const auto& character : SanCost.substr(0, SanCost.find("/")))
				{
					if (!isdigit(static_cast<unsigned char>(character)) && character != 'D' && character != 'd' && character != '+' && character != '-')
					{
						AddMsgToQueue(GlobalMsg["strSCInvalid"], eve.fromQQ, false);
						return;
					}
				}
				for (const auto& character : SanCost.substr(SanCost.find("/") + 1))
				{
					if (!isdigit(static_cast<unsigned char>(character)) && character != 'D' && character != 'd' && character != '+' && character != '-')
					{
						AddMsgToQueue(GlobalMsg["strSCInvalid"], eve.fromQQ, false);
						return;
					}
				}
				RD rdSuc(SanCost.substr(0, SanCost.find("/")));
				RD rdFail(SanCost.substr(SanCost.find("/") + 1));
				if (rdSuc.Roll() != 0 || rdFail.Roll() != 0)
				{
					AddMsgToQueue(GlobalMsg["strSCInvalid"], eve.fromDiscuss, false);

					return;
				}
				if (San.length() >= 3)
				{
					AddMsgToQueue(GlobalMsg["strSanInvalid"], eve.fromDiscuss, false);

					return;
				}
				const int intSan = San.empty()
					? CharacterProp[SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)]["����"]
					: stoi(San);
				if (intSan == 0)
				{
					AddMsgToQueue(GlobalMsg["strSanInvalid"], eve.fromDiscuss, false);

					return;
				}
				string strAns = strNickName + "��Sancheck:\n1D100=";
				const int intTmpRollRes = RandomGenerator::Randint(1, 100);
				strAns += to_string(intTmpRollRes);

				if (intTmpRollRes <= intSan)
				{
					strAns += " �ɹ�\n���Sanֵ����" + SanCost.substr(0, SanCost.find("/"));
					if (SanCost.substr(0, SanCost.find("/")).find("d") != string::npos)
						strAns += "=" + to_string(rdSuc.intTotal);
					strAns += +"��,��ǰʣ��" + to_string(max(0, intSan - rdSuc.intTotal)) + "��";
					if (San.empty())
					{
						CharacterProp[SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)]["����"] = max(0, intSan - rdSuc.intTotal
						);
					}
				}
				else if (intTmpRollRes == 100 || (intSan < 50 && intTmpRollRes > 95))

				{
					strAns += " ��ʧ��\n���Sanֵ����" + SanCost.substr(SanCost.find("/") + 1);
					// ReSharper disable once CppExpressionWithoutSideEffects
					rdFail.Max();
					if (SanCost.substr(SanCost.find("/") + 1).find("d") != string::npos)
						strAns += "���ֵ=" + to_string(rdFail.intTotal);
					strAns += +"��,��ǰʣ��" + to_string(max(0, intSan - rdFail.intTotal)) + "��";
					if (San.empty())
					{
						CharacterProp[SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)]["����"] = max(0, intSan - rdFail.intTotal
						);
					}
				}
				else
				{
					strAns += " ʧ��\n���Sanֵ����" + SanCost.substr(SanCost.find("/") + 1);
					if (SanCost.substr(SanCost.find("/") + 1).find("d") != string::npos)
						strAns += "=" + to_string(rdFail.intTotal);
					strAns += +"��,��ǰʣ��" + to_string(max(0, intSan - rdFail.intTotal)) + "��";
					if (San.empty())
					{
						CharacterProp[SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)]["����"] = max(0, intSan - rdFail.intTotal
						);
					}
				}
				AddMsgToQueue(strAns, eve.fromDiscuss, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "en")
		{
			intMsgCnt += 2;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strSkillName;
			while (intMsgCnt != eve.message.length() && !isdigit(static_cast<unsigned char>(eve.message[intMsgCnt])) && !isspace(static_cast<unsigned char>(eve.message[intMsgCnt]))
				)
			{
				strSkillName += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
			while (isspace(static_cast<unsigned char>(eve.message[intMsgCnt])))
				intMsgCnt++;
			string strCurrentValue;
			while (isdigit(static_cast<unsigned char>(eve.message[intMsgCnt])))
			{
				strCurrentValue += eve.message[intMsgCnt];
				intMsgCnt++;
			}
			int intCurrentVal;
			if (strCurrentValue.empty())
			{
				if (CharacterProp.count(SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)) && CharacterProp[SourceType(
					eve.fromQQ, DiscussT, eve.fromDiscuss)].count(strSkillName))
				{
					intCurrentVal = CharacterProp[SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)][strSkillName];
				}
				else if (SkillDefaultVal.count(strSkillName))
				{
					intCurrentVal = SkillDefaultVal[strSkillName];
				}
				else
				{
					AddMsgToQueue(GlobalMsg["strEnValInvalid"], eve.fromDiscuss, false);
					return;
				}
			}
			else
			{
				if (strCurrentValue.length() > 3)
				{
					AddMsgToQueue(GlobalMsg["strEnValInvalid"], eve.fromDiscuss, false);

					return;
				}
				intCurrentVal = stoi(strCurrentValue);
			}
			string strAns = strNickName + "��" + strSkillName + "��ǿ��ɳ��춨:\n1D100=";
			const int intTmpRollRes = RandomGenerator::Randint(1, 100);
			strAns += to_string(intTmpRollRes) + "/" + to_string(intCurrentVal);

			if (intTmpRollRes <= intCurrentVal && intTmpRollRes <= 95)
			{
				strAns += " ʧ��!\n���" + (strSkillName.empty() ? "���Ի���ֵ" : strSkillName) + "û�б仯!";
			}
			else
			{
				strAns += " �ɹ�!\n���" + (strSkillName.empty() ? "���Ի���ֵ" : strSkillName) + "����1D10=";
				const int intTmpRollD10 = RandomGenerator::Randint(1, 10);
				strAns += to_string(intTmpRollD10) + "��,��ǰΪ" + to_string(intCurrentVal + intTmpRollD10) + "��";
				if (strCurrentValue.empty())
				{
					CharacterProp[SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)][strSkillName] = intCurrentVal +
						intTmpRollD10;
				}
			}
			AddMsgToQueue(strAns, eve.fromDiscuss, false);
		}
	//Fate
	else if (strLowerMessage.substr(intMsgCnt, 4) == "fate")
		{
			intMsgCnt += 4;
			while (isspace(strLowerMessage[intMsgCnt]))
				intMsgCnt++;
			const string Command = strLowerMessage.substr(intMsgCnt, eve.message.find(' ', intMsgCnt) - intMsgCnt);
			if (Command == "on")
			{
				if (DisabledJRFATEDiscuss.count(eve.fromDiscuss))
				{
					DisabledJRFATEDiscuss.erase(eve.fromDiscuss);
					AddMsgToQueue("�ɹ��ڴ˶�������������FATE!", eve.fromDiscuss, false);
				}
				else
				{
					AddMsgToQueue("�ڴ˶���������FATEû�б�����!", eve.fromDiscuss, false);
				}
				return;
			}
			if (Command == "off")
			{
				if (!DisabledJRFATEDiscuss.count(eve.fromDiscuss))
				{
					DisabledJRFATEDiscuss.insert(eve.fromDiscuss);
					AddMsgToQueue("�ɹ��ڴ˶��������н���FATE!", eve.fromDiscuss, false);
				}
				else
				{
					AddMsgToQueue("�ڴ˶���������FATEû�б�����!", eve.fromDiscuss, false);
				}
				return;
			}
			if (DisabledJRFATEDiscuss.count(eve.fromDiscuss))
			{
				AddMsgToQueue("�ڴ˶���������FATE�ѱ�����!", eve.fromDiscuss, false);
				return;
			}
			char cstrDate[100] = {};
			time_t time_tTime = 0;
			time(&time_tTime);
			tm tmTime{};
			localtime_s(&tmTime, &time_tTime);
			strftime(cstrDate, 100, "%F", &tmTime);
			if (JRFATE.count(eve.fromQQ) && JRFATE[eve.fromQQ].Date == cstrDate)
			{
				const string strReply = strNickName + " ���շ������� \n" + tarotCard[JRFATE[eve.fromQQ].FATEVal];
				AddMsgToQueue(strReply, eve.fromDiscuss, false);
			}
			else
			{

				int iFATE = RandomGenerator::Randint(1, 44);
				JRFATE[eve.fromQQ].Date = cstrDate;
				JRFATE[eve.fromQQ].FATEVal = iFATE;
				string strReply(strNickName + " ���շ������� \n" + tarotCard[JRFATE[eve.fromQQ].FATEVal]);
				AddMsgToQueue(strReply, eve.fromDiscuss, false);
			}
		}
	else if (strLowerMessage.substr(intMsgCnt, 5) == "tarot")
		{
			const string strReply(strNickName + " �е������� \n" + tarotCard[RandomGenerator::Randint(1, 44)]);
			AddMsgToQueue(strReply, eve.fromDiscuss, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 3) == "scp")
	{
	intMsgCnt += 3;
	while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
		intMsgCnt++;
	string strSCP = eve.message.substr(intMsgCnt);
	for (auto i : strSCP)
	{
		if (!isdigit(i))
		{
			AddMsgToQueue(GlobalMsg["strSCPErr"], eve.fromDiscuss, false);
			return;
		}
	}
	if (strSCP > "3999")
	{
		AddMsgToQueue(GlobalMsg["strSCPErr"], eve.fromDiscuss, false);
		return;
	}
	if (strSCP.length() == 4)
	{
		if (strSCP >= "1000" && strSCP <= "1999")
		{
			const string scpa = GlobalMsg["strSCP"] + strSCP;
			const string scpb = GlobalMsg["strSCPWeb"] + "-ii/scp-" + strSCP;
			AddMsgToQueue(scpa + "\n" + scpb, eve.fromDiscuss, false);
		}
		else if (strSCP >= "2000" && strSCP <= "2999")
		{
			const string scpa = GlobalMsg["strSCP"] + strSCP;
			const string scpb = GlobalMsg["strSCPWeb"] + "-iii/scp-" + strSCP;
			AddMsgToQueue(scpa + "\n" + scpb, eve.fromDiscuss, false);
		}
		else if (strSCP >= "3000" && strSCP <= "3999")
		{
			const string scpa = GlobalMsg["strSCP"] + strSCP;
			const string scpb = GlobalMsg["strSCPWeb"] + "-iv/scp-" + strSCP;
			AddMsgToQueue(scpa + "\n" + scpb, eve.fromDiscuss, false);
		}
	}
	else if (strSCP.length() == 3)
	{
		const string scpa = GlobalMsg["strSCP"] + strSCP;
		const string scpb = GlobalMsg["strSCPWeb"] + "-" + strSCP;
		AddMsgToQueue(scpa + "\n" + scpb, eve.fromDiscuss, false);
	}
	else if (strSCP.length() == 2)
	{
		const string scpa = GlobalMsg["strSCP"] + "0" + strSCP;
		const string scpb = GlobalMsg["strSCPWeb"] + "-i/scp-0" + strSCP;
		AddMsgToQueue(scpa + "\n" + scpb, eve.fromDiscuss, false);
	}
	else if (strSCP.length() == 1)
	{
		const string scpa = GlobalMsg["strSCP"] + "00" + strSCP;
		const string scpb = GlobalMsg["strSCPWeb"] + "-i/scp-00" + strSCP;
		AddMsgToQueue(scpa + "\n" + scpb, eve.fromDiscuss, false);
	}
	}

	else if (strLowerMessage.substr(intMsgCnt, 2) == "nn")
		{
			intMsgCnt += 2;
			while (isspace(static_cast<unsigned char>(eve.message[intMsgCnt])))
				intMsgCnt++;
			string name = eve.message.substr(intMsgCnt);
			if (name.length() > 50)
			{
				AddMsgToQueue(GlobalMsg["strNameTooLongErr"], eve.fromDiscuss, false);
				return;
			}
			if (!name.empty())
			{
				Name->set(eve.fromDiscuss, eve.fromQQ, name);
				const string strReply = "�ѽ�" + strNickName + "�����Ƹ���Ϊ" + strip(name);
				AddMsgToQueue(strReply, eve.fromDiscuss, false);
			}
			else
			{
				if (Name->del(eve.fromDiscuss, eve.fromQQ))
				{
					const string strReply = "�ѽ�" + strNickName + "������ɾ��";
					AddMsgToQueue(strReply, eve.fromDiscuss, false);
				}
				else
				{
					const string strReply = strNickName + GlobalMsg["strNameDelErr"];
					AddMsgToQueue(strReply, eve.fromDiscuss, false);
				}
			}
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "me")
		{
			intMsgCnt += 2;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strAction = strLowerMessage.substr(intMsgCnt);
			if (strAction == "on")
			{
				if (DisabledMEDiscuss.count(eve.fromDiscuss))
				{
					DisabledMEDiscuss.erase(eve.fromDiscuss);
					AddMsgToQueue("�ɹ��ڱ���������������.me����!", eve.fromDiscuss, false);
				}
				else
				{
					AddMsgToQueue("�ڱ�����������.me����û�б�����!", eve.fromDiscuss, false);
				}
				return;
			}
			if (strAction == "off")
			{
				if (!DisabledMEDiscuss.count(eve.fromDiscuss))
				{
					DisabledMEDiscuss.insert(eve.fromDiscuss);
					AddMsgToQueue("�ɹ��ڱ����������н���.me����!", eve.fromDiscuss, false);
				}
				else
				{
					AddMsgToQueue("�ڱ�����������.me����û�б�����!", eve.fromDiscuss, false);
				}
				return;
			}
			if (DisabledMEDiscuss.count(eve.fromDiscuss))
			{
				AddMsgToQueue("�ڱ�����������.me�����ѱ�����!", eve.fromDiscuss, false);
				return;
			}
			if (DisabledMEDiscuss.count(eve.fromDiscuss))
			{
				AddMsgToQueue(GlobalMsg["strMEDisabledErr"], eve.fromDiscuss, false);
				return;
			}
			strAction = strip(eve.message.substr(intMsgCnt));
			if (strAction.empty())
			{
				AddMsgToQueue("��������Ϊ��!", eve.fromDiscuss, false);
				return;
			}
			const string strReply = strNickName + strAction;
			AddMsgToQueue(strReply, eve.fromDiscuss, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 3) == "set")
		{
			intMsgCnt += 3;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strDefaultDice = strLowerMessage.substr(intMsgCnt, strLowerMessage.find(" ", intMsgCnt) - intMsgCnt);
			while (strDefaultDice[0] == '0')
				strDefaultDice.erase(strDefaultDice.begin());
			if (strDefaultDice.empty())
				strDefaultDice = "100";
			for (auto charNumElement : strDefaultDice)
				if (!isdigit(static_cast<unsigned char>(charNumElement)))
				{
					AddMsgToQueue(GlobalMsg["strSetInvalid"], eve.fromDiscuss, false);
					return;
				}
			if (strDefaultDice.length() > 5)
			{
				AddMsgToQueue(GlobalMsg["strSetTooBig"], eve.fromDiscuss, false);
				return;
			}
			const int intDefaultDice = stoi(strDefaultDice);
			if (intDefaultDice == 100)
				DefaultDice.erase(eve.fromQQ);
			else
				DefaultDice[eve.fromQQ] = intDefaultDice;
			const string strSetSuccessReply = "�ѽ�" + strNickName + "��Ĭ�������͸���ΪD" + strDefaultDice;
			AddMsgToQueue(strSetSuccessReply, eve.fromDiscuss, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 4) == "coc6")
		{
			intMsgCnt += 4;
			if (strLowerMessage[intMsgCnt] == 's')
				intMsgCnt++;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strNum;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strNum += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (strNum.length() > 2)
			{
				AddMsgToQueue(GlobalMsg["strCharacterTooBig"], eve.fromQQ, false);
				return;
			}
			const int intNum = stoi(strNum.empty() ? "1" : strNum);
			if (intNum > 10)
			{
				AddMsgToQueue(GlobalMsg["strCharacterTooBig"], eve.fromDiscuss, false);
				return;
			}
			if (intNum == 0)
			{
				AddMsgToQueue(GlobalMsg["strCharacterCannotBeZero"], eve.fromDiscuss, false);
				return;
			}
			string strReply = strNickName;
			COC6(strReply, intNum);
			AddMsgToQueue(strReply, eve.fromDiscuss, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 3) == "dnd")
		{
			intMsgCnt += 3;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strNum;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strNum += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (strNum.length() > 2)
			{
				AddMsgToQueue(GlobalMsg["strCharacterTooBig"], eve.fromQQ, false);
				return;
			}
			const int intNum = stoi(strNum.empty() ? "1" : strNum);
			if (intNum > 10)
			{
				AddMsgToQueue(GlobalMsg["strCharacterTooBig"], eve.fromDiscuss, false);
				return;
			}
			if (intNum == 0)
			{
				AddMsgToQueue(GlobalMsg["strCharacterCannotBeZero"], eve.fromDiscuss, false);
				return;
			}
			string strReply = strNickName;
			DND(strReply, intNum);
			AddMsgToQueue(strReply, eve.fromDiscuss, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 3) == "coc")
		{
			intMsgCnt += 3;
			if (strLowerMessage[intMsgCnt] == '7')
				intMsgCnt++;
			if (strLowerMessage[intMsgCnt] == 's')
				intMsgCnt++;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strNum;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strNum += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (strNum.length() > 2)
			{
				AddMsgToQueue(GlobalMsg["strCharacterTooBig"], eve.fromQQ, false);
				return;
			}
			const int intNum = stoi(strNum.empty() ? "1" : strNum);
			if (intNum > 10)
			{
				AddMsgToQueue(GlobalMsg["strCharacterTooBig"], eve.fromDiscuss, false);
				return;
			}
			if (intNum == 0)
			{
				AddMsgToQueue(GlobalMsg["strCharacterCannotBeZero"], eve.fromDiscuss, false);
				return;
			}
			string strReply = strNickName;
			COC7(strReply, intNum);
			AddMsgToQueue(strReply, eve.fromDiscuss, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "ra")
		{
			intMsgCnt += 2;
			string strSkillName;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))intMsgCnt++;
			while (intMsgCnt != strLowerMessage.length() && !isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !
				isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && strLowerMessage[intMsgCnt] != '=' && strLowerMessage[intMsgCnt] !=
				':')
			{
				strSkillName += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) || strLowerMessage[intMsgCnt] == '=' || strLowerMessage[intMsgCnt] ==
				':')intMsgCnt++;
			string strSkillVal;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strSkillVal += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				intMsgCnt++;
			}
			string strReason = eve.message.substr(intMsgCnt);
			int intSkillVal;
			if (strSkillVal.empty())
			{
				if (CharacterProp.count(SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)) && CharacterProp[SourceType(
					eve.fromQQ, DiscussT, eve.fromDiscuss)].count(strSkillName))
				{
					intSkillVal = CharacterProp[SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)][strSkillName];
				}
				else if (SkillDefaultVal.count(strSkillName))
				{
					intSkillVal = SkillDefaultVal[strSkillName];
				}
				else
				{
					AddMsgToQueue(GlobalMsg["strUnknownPropErr"], eve.fromDiscuss, false);
					return;
				}
			}
			else if (strSkillVal.length() > 3)
			{
				AddMsgToQueue(GlobalMsg["strPropErr"], eve.fromDiscuss, false);
				return;
			}
			else
			{
				intSkillVal = stoi(strSkillVal);
			}
			const int intD100Res = RandomGenerator::Randint(1, 100);
			string strReply = strNickName + "����" + strSkillName + "�춨: D100=" + to_string(intD100Res) + "/" +
				to_string(intSkillVal) + " ";
			if (intD100Res <= 5)strReply += "��ɹ�";
			else if (intD100Res <= intSkillVal / 5)strReply += "���ѳɹ�";
			else if (intD100Res <= intSkillVal / 2)strReply += "���ѳɹ�";
			else if (intD100Res <= intSkillVal)strReply += "�ɹ�";
			else if (intD100Res <= 95)strReply += "ʧ��";
			else strReply += "��ʧ��";
			if (!strReason.empty())
			{
				strReply = "����" + strReason + " " + strReply;
			}
			AddMsgToQueue(strReply, eve.fromDiscuss, false);
		}
	else if (strLowerMessage.substr(intMsgCnt, 2) == "rc")
		{
			intMsgCnt += 2;
			string strSkillName;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))intMsgCnt++;
			while (intMsgCnt != strLowerMessage.length() && !isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !
				isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && strLowerMessage[intMsgCnt] != '=' && strLowerMessage[intMsgCnt] !=
				':')
			{
				strSkillName += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) || strLowerMessage[intMsgCnt] == '=' || strLowerMessage[intMsgCnt] ==
				':')intMsgCnt++;
			string strSkillVal;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strSkillVal += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				intMsgCnt++;
			}
			string strReason = eve.message.substr(intMsgCnt);
			int intSkillVal;
			if (strSkillVal.empty())
			{
				if (CharacterProp.count(SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)) && CharacterProp[SourceType(
					eve.fromQQ, DiscussT, eve.fromDiscuss)].count(strSkillName))
				{
					intSkillVal = CharacterProp[SourceType(eve.fromQQ, DiscussT, eve.fromDiscuss)][strSkillName];
				}
				else if (SkillDefaultVal.count(strSkillName))
				{
					intSkillVal = SkillDefaultVal[strSkillName];
				}
				else
				{
					AddMsgToQueue(GlobalMsg["strUnknownPropErr"], eve.fromDiscuss, false);
					return;
				}
			}
			else if (strSkillVal.length() > 3)
			{
				AddMsgToQueue(GlobalMsg["strPropErr"], eve.fromDiscuss, false);
				return;
			}
			else
			{
				intSkillVal = stoi(strSkillVal);
			}
			const int intD100Res = RandomGenerator::Randint(1, 100);
			string strReply = strNickName + "����" + strSkillName + "�춨: D100=" + to_string(intD100Res) + "/" +
				to_string(intSkillVal) + " ";
			if (intD100Res == 1)strReply += "��ɹ�";
			else if (intD100Res <= intSkillVal / 5)strReply += "���ѳɹ�";
			else if (intD100Res <= intSkillVal / 2)strReply += "���ѳɹ�";
			else if (intD100Res <= intSkillVal)strReply += "�ɹ�";
			else if (intD100Res <= 95 || (intSkillVal >= 50 && intD100Res != 100))strReply += "ʧ��";
			else strReply += "��ʧ��";
			if (!strReason.empty())
			{
				strReply = "����" + strReason + " " + strReply;
			}
			AddMsgToQueue(strReply, eve.fromDiscuss, false);
		}
	else if (strLowerMessage[intMsgCnt] == 'r' ||strLowerMessage[intMsgCnt] == 'h')
		{
			bool isHidden = false;
			if (strLowerMessage[intMsgCnt] == 'h')
				isHidden = true;
			intMsgCnt += 1;
			bool boolDetail = true;
			if (eve.message[intMsgCnt] == 's')
			{
				boolDetail = false;
				intMsgCnt++;
			}
			if (strLowerMessage[intMsgCnt] == 'h')
			{
				isHidden = true;
				intMsgCnt += 1;
			}
			while (isspace(static_cast<unsigned char>(eve.message[intMsgCnt])))
				intMsgCnt++;
			string strMainDice;
			string strReason;
			bool tmpContainD = false;
			int intTmpMsgCnt;
			for (intTmpMsgCnt = intMsgCnt; intTmpMsgCnt != eve.message.length() && eve.message[intTmpMsgCnt] != ' ';
				intTmpMsgCnt++)
			{
				if (strLowerMessage[intTmpMsgCnt] == 'd' || strLowerMessage[intTmpMsgCnt] == 'p' || strLowerMessage[
					intTmpMsgCnt] == 'b' || strLowerMessage[intTmpMsgCnt] == '#' || strLowerMessage[intTmpMsgCnt] == 'f'
						||
						strLowerMessage[intTmpMsgCnt] == 'a')
					tmpContainD = true;
					if (!isdigit(static_cast<unsigned char>(strLowerMessage[intTmpMsgCnt])) && strLowerMessage[intTmpMsgCnt] != 'd' && strLowerMessage[
						intTmpMsgCnt] != 'k' && strLowerMessage[intTmpMsgCnt] != 'p' && strLowerMessage[intTmpMsgCnt] != 'b'
							&&
							strLowerMessage[intTmpMsgCnt] != 'f' && strLowerMessage[intTmpMsgCnt] != '+' && strLowerMessage[
								intTmpMsgCnt
							] != '-' && strLowerMessage[intTmpMsgCnt] != '#' && strLowerMessage[intTmpMsgCnt] != 'a')
					{
						break;
					}
			}
			if (tmpContainD)
			{
				strMainDice = strLowerMessage.substr(intMsgCnt, intTmpMsgCnt - intMsgCnt);
				while (isspace(static_cast<unsigned char>(eve.message[intTmpMsgCnt])))
					intTmpMsgCnt++;
				strReason = eve.message.substr(intTmpMsgCnt);
			}
			else
				strReason = eve.message.substr(intMsgCnt);

			int intTurnCnt = 1;
			if (strMainDice.find("#") != string::npos)
			{
				string strTurnCnt = strMainDice.substr(0, strMainDice.find("#"));
				if (strTurnCnt.empty())
					strTurnCnt = "1";
				strMainDice = strMainDice.substr(strMainDice.find("#") + 1);
				RD rdTurnCnt(strTurnCnt, eve.fromQQ);
				const int intRdTurnCntRes = rdTurnCnt.Roll();
				if (intRdTurnCntRes == Value_Err)
				{
					AddMsgToQueue(GlobalMsg["strValueErr"], eve.fromDiscuss, false);
					return;
				}
				if (intRdTurnCntRes == Input_Err)
				{
					AddMsgToQueue(GlobalMsg["strInputErr"], eve.fromDiscuss, false);
					return;
				}
				if (intRdTurnCntRes == ZeroDice_Err)
				{
					AddMsgToQueue(GlobalMsg["strZeroDiceErr"], eve.fromDiscuss, false);
					return;
				}
				if (intRdTurnCntRes == ZeroType_Err)
				{
					AddMsgToQueue(GlobalMsg["strZeroTypeErr"], eve.fromDiscuss, false);
					return;
				}
				if (intRdTurnCntRes == DiceTooBig_Err)
				{
					AddMsgToQueue(GlobalMsg["strDiceTooBigErr"], eve.fromDiscuss, false);
					return;
				}
				if (intRdTurnCntRes == TypeTooBig_Err)
				{
					AddMsgToQueue(GlobalMsg["strTypeTooBigErr"], eve.fromDiscuss, false);
					return;
				}
				if (intRdTurnCntRes == AddDiceVal_Err)
				{
					AddMsgToQueue(GlobalMsg["strAddDiceValErr"], eve.fromDiscuss, false);
					return;
				}
				if (intRdTurnCntRes != 0)
				{
					AddMsgToQueue(GlobalMsg["strUnknownErr"], eve.fromDiscuss, false);
					return;
				}
				if (rdTurnCnt.intTotal > 10)
				{
					AddMsgToQueue(GlobalMsg["strRollTimeExceeded"], eve.fromDiscuss, false);
					return;
				}
				if (rdTurnCnt.intTotal <= 0)
				{
					AddMsgToQueue(GlobalMsg["strRollTimeErr"], eve.fromDiscuss, false);
					return;
				}
				intTurnCnt = rdTurnCnt.intTotal;
				if (strTurnCnt.find("d") != string::npos)
				{
					string strTurnNotice = strNickName + "����������: " + rdTurnCnt.FormShortString() + "��";
					if (!isHidden)
					{
						AddMsgToQueue(strTurnNotice, eve.fromDiscuss, false);
					}
					else
					{
						strTurnNotice = "�ڶ��������� " + strTurnNotice;
						AddMsgToQueue(strTurnNotice, eve.fromQQ);
						const auto range = ObserveDiscuss.equal_range(eve.fromDiscuss);
						for (auto it = range.first; it != range.second; ++it)
						{
							if (it->second != eve.fromQQ)
							{
								AddMsgToQueue(strTurnNotice, it->second);
							}
						}
					}
				}
			}
			RD rdMainDice(strMainDice, eve.fromQQ);

			const int intFirstTimeRes = rdMainDice.Roll();
			if (intFirstTimeRes == Value_Err)
			{
				AddMsgToQueue(GlobalMsg["strValueErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes == Input_Err)
			{
				AddMsgToQueue(GlobalMsg["strInputErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes == ZeroDice_Err)
			{
				AddMsgToQueue(GlobalMsg["strZeroDiceErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes == ZeroType_Err)
			{
				AddMsgToQueue(GlobalMsg["strZeroTypeErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes == DiceTooBig_Err)
			{
				AddMsgToQueue(GlobalMsg["strDiceTooBigErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes == TypeTooBig_Err)
			{
				AddMsgToQueue(GlobalMsg["strTypeTooBigErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes == AddDiceVal_Err)
			{
				AddMsgToQueue(GlobalMsg["strAddDiceValErr"], eve.fromDiscuss, false);
				return;
			}
			if (intFirstTimeRes != 0)
			{
				AddMsgToQueue(GlobalMsg["strUnknownErr"], eve.fromDiscuss, false);
				return;
			}
			if (!boolDetail && intTurnCnt != 1)
			{
				string strAns = strNickName + "������: " + to_string(intTurnCnt) + "��" + rdMainDice.strDice + ": { ";
				if (!strReason.empty())
					strAns.insert(0, "����" + strReason + " ");
				vector<int> vintExVal;
				while (intTurnCnt--)
				{
					// �˴�����ֵ����
					// ReSharper disable once CppExpressionWithoutSideEffects
					rdMainDice.Roll();
					strAns += to_string(rdMainDice.intTotal);
					if (intTurnCnt != 0)
						strAns += ",";
					if ((rdMainDice.strDice == "D100" || rdMainDice.strDice == "1D100") && (rdMainDice.intTotal <= 5 ||
						rdMainDice.intTotal >= 96))
						vintExVal.push_back(rdMainDice.intTotal);
				}
				strAns += " }";
				if (!vintExVal.empty())
				{
					strAns += ",��ֵ: ";
					for (auto it = vintExVal.cbegin(); it != vintExVal.cend(); ++it)
					{
						strAns += to_string(*it);
						if (it != vintExVal.cend() - 1)
							strAns += ",";
					}
				}
				if (!isHidden)
				{
					AddMsgToQueue(strAns, eve.fromDiscuss, false);
				}
				else
				{
					strAns = "�ڶ��������� " + strAns;
					AddMsgToQueue(strAns, eve.fromQQ);
					const auto range = ObserveDiscuss.equal_range(eve.fromDiscuss);
					for (auto it = range.first; it != range.second; ++it)
					{
						if (it->second != eve.fromQQ)
						{
							AddMsgToQueue(strAns, it->second);
						}
					}
				}
			}
			else
			{
				while (intTurnCnt--)
				{
					// �˴�����ֵ����
					// ReSharper disable once CppExpressionWithoutSideEffects
					rdMainDice.Roll();
					string strAns = strNickName + "������: " + (boolDetail
						? rdMainDice.FormCompleteString()
						: rdMainDice.FormShortString());
					if (!strReason.empty())
						strAns.insert(0, "����" + strReason + " ");
					if (!isHidden)
					{
						AddMsgToQueue(strAns, eve.fromDiscuss, false);
					}
					else
					{
						strAns = "�ڶ��������� " + strAns;
						AddMsgToQueue(strAns, eve.fromQQ);
						const auto range = ObserveDiscuss.equal_range(eve.fromDiscuss);
						for (auto it = range.first; it != range.second; ++it)
						{
							if (it->second != eve.fromQQ)
							{
								AddMsgToQueue(strAns, it->second);
							}
						}
					}
				}
			}
			if (isHidden)
			{
				const string strReply = strNickName + "������һ�ΰ���";
				AddMsgToQueue(strReply, eve.fromDiscuss, false);
			}
		}
}

EVE_System_GroupMemberDecrease(eventSystemGroupMemberDecrease)
{
	if (beingOperateQQ == getLoginQQ())
	{
		BanFriendList.insert(fromQQ);
		BanGroupList.insert(fromGroup);
		AddMsgToQueue("���Լ�Ⱥ<" + to_string(fromGroup) + ">��Υ������ѱ�������������������������ϵ���ս�(25528272027)", fromQQ);
		AddMsgToQueue("�ѽ�QQ<" + to_string(fromQQ) + ">��Ⱥ<" + to_string(fromGroup) + ">������������" + "ԭ�򣺱��߳�Ⱥ", MASTER);
		return 1;
	}
	return 0;
}


EVE_System_GroupMemberIncrease(eventGroupMemberIncrease)
{
	if (BanFriendList.count(beingOperateQQ) && WelcomeMsg.count(fromGroup))
	{ 
		const string strReply = "����һֻ�������Ĳ�����״�������أ�";
		AddMsgToQueue(strReply,fromGroup,false);
	}
	if (!BanFriendList.count(beingOperateQQ) && beingOperateQQ != getLoginQQ() && WelcomeMsg.count(fromGroup))
	{
		string strReply = WelcomeMsg[fromGroup];
		while (strReply.find("{@}") != string::npos)
		{
			strReply.replace(strReply.find("{@}"), 3, "[CQ:at,qq=" + to_string(beingOperateQQ) + "]");
		}
		while (strReply.find("{nick}") != string::npos)
		{
			strReply.replace(strReply.find("{nick}"), 6, getStrangerInfo(beingOperateQQ).nick);
		}
		while (strReply.find("{age}") != string::npos)
		{
			strReply.replace(strReply.find("{age}"), 5, to_string(getStrangerInfo(beingOperateQQ).age));
		}
		while (strReply.find("{sex}") != string::npos)
		{
			strReply.replace(strReply.find("{sex}"), 5,
			                 getStrangerInfo(beingOperateQQ).sex == 0
				                 ? "��"
				                 : getStrangerInfo(beingOperateQQ).sex == 1
				                 ? "Ů"
				                 : "δ֪");
		}
		while (strReply.find("{qq}") != string::npos)
		{
			strReply.replace(strReply.find("{qq}"), 4, to_string(beingOperateQQ));
		}
		AddMsgToQueue(strReply, fromGroup, false);
	}
	return 0;
}

EVE_Request_AddFriend(eventRequest_AddFriend)
{
	setFriendAddRequest(responseFlag, 1, "");
	return 1;
}
EVE_Request_AddGroup(eventRequest_AddGroup)
{
	if (subType == 2)
	{
		if (BanFriendList.count(fromQQ))
		{
			return 1;
		}
		setGroupAddRequest(responseFlag, 2, 1, "");
		AddMsgToQueue(getStrangerInfo(fromQQ).nick + "(" + to_string(fromQQ) + ")�����Ҽ���Ⱥ:" + getGroupList()[fromGroup] + "(" + to_string(fromGroup) + ")", MASTER);

	}
	return 0;
}

EVE_Disable(eventDisable)
{
	Enabled = false;
	ilInitList.reset();
	Name.reset();
	ofstream ofstreamDisabledGroup(strFileLoc + "DisabledGroup.RDconf", ios::out | ios::trunc);
	for (auto it = DisabledGroup.begin(); it != DisabledGroup.end(); ++it)
	{
		ofstreamDisabledGroup << *it << std::endl;
	}
	ofstreamDisabledGroup.close();

	ofstream ofstreamDisabledDiscuss(strFileLoc + "DisabledDiscuss.RDconf", ios::out | ios::trunc);
	for (auto it = DisabledDiscuss.begin(); it != DisabledDiscuss.end(); ++it)
	{
		ofstreamDisabledDiscuss << *it << std::endl;
	}
	ofstreamDisabledDiscuss.close();

	//Fate
	ofstream ofstreamDisabledFATEGroup(strFileLoc + "DisabledJRFATEGroup.RDconf", ios::out | ios::trunc);
	for (auto it = DisabledJRFATEGroup.begin(); it != DisabledJRFATEGroup.end(); ++it)
	{
		ofstreamDisabledFATEGroup << *it << std::endl;
	}
	ofstreamDisabledFATEGroup.close();

	ofstream ofstreamDisabledFATEDiscuss(strFileLoc + "DisabledJRFATEDiscuss.RDconf", ios::out | ios::trunc);
	for (auto it = DisabledJRFATEDiscuss.begin(); it != DisabledJRFATEDiscuss.end(); ++it)
	{
		ofstreamDisabledFATEDiscuss << *it << std::endl;
	}
	ofstreamDisabledFATEDiscuss.close();
	//Fate

	ofstream ofstreamDisabledMEGroup(strFileLoc + "DisabledMEGroup.RDconf", ios::out | ios::trunc);
	for (auto it = DisabledMEGroup.begin(); it != DisabledMEGroup.end(); ++it)
	{
		ofstreamDisabledMEGroup << *it << std::endl;
	}
	ofstreamDisabledMEGroup.close();

	ofstream ofstreamDisabledMEDiscuss(strFileLoc + "DisabledMEDiscuss.RDconf", ios::out | ios::trunc);
	for (auto it = DisabledMEDiscuss.begin(); it != DisabledMEDiscuss.end(); ++it)
	{
		ofstreamDisabledMEDiscuss << *it << std::endl;
	}
	ofstreamDisabledMEDiscuss.close();

	ofstream ofstreamDisabledHELPGroup(strFileLoc + "DisabledHELPGroup.RDconf", ios::in | ios::trunc);
	for (auto it = DisabledHELPGroup.begin(); it != DisabledHELPGroup.end(); ++it)
	{
		ofstreamDisabledHELPGroup << *it << std::endl;
	}
	ofstreamDisabledHELPGroup.close();

	ofstream ofstreamDisabledHELPDiscuss(strFileLoc + "DisabledHELPDiscuss.RDconf", ios::in | ios::trunc);
	for (auto it = DisabledHELPDiscuss.begin(); it != DisabledHELPDiscuss.end(); ++it)
	{
		ofstreamDisabledHELPDiscuss << *it << std::endl;
	}
	ofstreamDisabledHELPDiscuss.close();

	ofstream ofstreamDisabledOBGroup(strFileLoc + "DisabledOBGroup.RDconf", ios::out | ios::trunc);

	ofstream ofstreamBanFriendList(strFileLoc + "BanFriendList.RDconf", ios::out | ios::trunc);
	for (auto it = BanFriendList.begin(); it != BanFriendList.end(); ++it)
	{
		ofstreamBanFriendList << *it << std::endl;
	}
	ofstreamBanFriendList.close();


	ofstream ofstreamBanGroupList(strFileLoc + "BanGroupList.RDconf", ios::out | ios::trunc);
	for (auto it = BanGroupList.begin(); it != BanGroupList.end(); ++it)
	{
		ofstreamBanGroupList << *it << std::endl;
	}
	ofstreamBanGroupList.close();

	for (auto it = DisabledOBGroup.begin(); it != DisabledOBGroup.end(); ++it)
	{
		ofstreamDisabledOBGroup << *it << std::endl;
	}
	ofstreamDisabledOBGroup.close();

	ofstream ofstreamDisabledOBDiscuss(strFileLoc + "DisabledOBDiscuss.RDconf", ios::out | ios::trunc);
	for (auto it = DisabledOBDiscuss.begin(); it != DisabledOBDiscuss.end(); ++it)
	{
		ofstreamDisabledOBDiscuss << *it << std::endl;
	}
	ofstreamDisabledOBDiscuss.close();

	ofstream ofstreamObserveGroup(strFileLoc + "ObserveGroup.RDconf", ios::out | ios::trunc);
	for (auto it = ObserveGroup.begin(); it != ObserveGroup.end(); ++it)
	{
		ofstreamObserveGroup << it->first << " " << it->second << std::endl;
	}
	ofstreamObserveGroup.close();

	ofstream ofstreamObserveDiscuss(strFileLoc + "ObserveDiscuss.RDconf", ios::out | ios::trunc);
	for (auto it = ObserveDiscuss.begin(); it != ObserveDiscuss.end(); ++it)
	{
		ofstreamObserveDiscuss << it->first << " " << it->second << std::endl;
	}
	ofstreamObserveDiscuss.close();
	//Fate
	ofstream ofstreamJRFATE(strFileLoc + "JRFATE.RDconf", ios::out | ios::trunc);
	for (auto it = JRFATE.begin(); it != JRFATE.end(); ++it)
	{
		ofstreamJRFATE << it->first << " " << it->second.Date << " " << it->second.FATEVal << std::endl;
	}
	ofstreamJRFATE.close();
	//Fate
	ofstream ofstreamCharacterProp(strFileLoc + "CharacterProp.RDconf", ios::out | ios::trunc);
	for (auto it = CharacterProp.begin(); it != CharacterProp.end(); ++it)
	{
		for (auto it1 = it->second.cbegin(); it1 != it->second.cend(); ++it1)
		{
			ofstreamCharacterProp << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " "
				<< it1->first << " " << it1->second << std::endl;
		}
	}
	ofstreamCharacterProp.close();
	ofstream ofstreamDefault(strFileLoc + "Default.RDconf", ios::out | ios::trunc);
	for (auto it = DefaultDice.begin(); it != DefaultDice.end(); ++it)
	{
		ofstreamDefault << it->first << " " << it->second << std::endl;
	}
	ofstreamDefault.close();

	ofstream ofstreamWelcomeMsg(strFileLoc + "WelcomeMsg.RDconf", ios::out | ios::trunc);
	for (auto it = WelcomeMsg.begin(); it != WelcomeMsg.end(); ++it)
	{
		while (it->second.find(' ') != string::npos)it->second.replace(it->second.find(' '), 1, "{space}");
		while (it->second.find('\t') != string::npos)it->second.replace(it->second.find('\t'), 1, "{tab}");
		while (it->second.find('\n') != string::npos)it->second.replace(it->second.find('\n'), 1, "{endl}");
		while (it->second.find('\r') != string::npos)it->second.replace(it->second.find('\r'), 1, "{enter}");
		ofstreamWelcomeMsg << it->first << " " << it->second << std::endl;
	}
	ofstreamWelcomeMsg.close();
	JRFATE.clear();
	DefaultDice.clear();
	DisabledGroup.clear();
	DisabledDiscuss.clear();
	BanFriendList.clear();
	BanGroupList.clear();
	DisabledJRFATEGroup.clear();
	DisabledJRFATEDiscuss.clear();
	DisabledMEGroup.clear();
	DisabledMEDiscuss.clear();
	DisabledOBGroup.clear();
	DisabledOBDiscuss.clear();
	ObserveGroup.clear();
	ObserveDiscuss.clear();
	strFileLoc.clear();
	return 0;
}

EVE_Exit(eventExit)
{
	if (!Enabled)
		return 0;
	ilInitList.reset();
	Name.reset();
	ofstream ofstreamDisabledGroup(strFileLoc + "DisabledGroup.RDconf", ios::out | ios::trunc);
	for (auto it = DisabledGroup.begin(); it != DisabledGroup.end(); ++it)
	{
		ofstreamDisabledGroup << *it << std::endl;
	}
	ofstreamDisabledGroup.close();

	ofstream ofstreamDisabledDiscuss(strFileLoc + "DisabledDiscuss.RDconf", ios::out | ios::trunc);
	for (auto it = DisabledDiscuss.begin(); it != DisabledDiscuss.end(); ++it)
	{
		ofstreamDisabledDiscuss << *it << std::endl;
	}
	ofstreamDisabledDiscuss.close();

	//Fate
	ofstream ofstreamDisabledFATEG(strFileLoc + "DisabledJRFATEGroup.RDconf", ios::out | ios::trunc);
	for (auto it = DisabledJRFATEGroup.begin(); it != DisabledJRFATEGroup.end(); ++it)
	{
		ofstreamDisabledFATEG << *it << std::endl;
	}
	ofstreamDisabledFATEG.close();

	ofstream ofstreamDisabledFATED(strFileLoc + "DisabledJRFATEDiscuss.RDconf", ios::out | ios::trunc);
	for (auto it = DisabledJRFATEDiscuss.begin(); it != DisabledJRFATEDiscuss.end(); ++it)
	{
		ofstreamDisabledFATED << *it << std::endl;
	}
	ofstreamDisabledFATED.close();

	ofstream ofstreamBanFriendList(strFileLoc + "BanFriendList.RDconf", ios::out | ios::trunc);
	for (auto it = BanFriendList.begin(); it != BanFriendList.end(); ++it)
	{
		ofstreamBanFriendList << *it << std::endl;
	}
	ofstreamBanFriendList.close();


	ofstream ofstreamBanGroupList(strFileLoc + "BanGroupList.RDconf", ios::out | ios::trunc);
	for (auto it = BanGroupList.begin(); it != BanGroupList.end(); ++it)
	{
		ofstreamBanGroupList << *it << std::endl;
	}
	ofstreamBanGroupList.close();

	//Fate
	ofstream ofstreamDisabledMEGroup(strFileLoc + "DisabledMEGroup.RDconf", ios::out | ios::trunc);
	for (auto it = DisabledMEGroup.begin(); it != DisabledMEGroup.end(); ++it)
	{
		ofstreamDisabledMEGroup << *it << std::endl;
	}
	ofstreamDisabledMEGroup.close();

	ofstream ofstreamDisabledMEDiscuss(strFileLoc + "DisabledMEDiscuss.RDconf", ios::out | ios::trunc);
	for (auto it = DisabledMEDiscuss.begin(); it != DisabledMEDiscuss.end(); ++it)
	{
		ofstreamDisabledMEDiscuss << *it << std::endl;
	}
	ofstreamDisabledMEDiscuss.close();

	ofstream ofstreamDisabledHELPGroup(strFileLoc + "DisabledHELPGroup.RDconf", ios::in | ios::trunc);
	for (auto it = DisabledHELPGroup.begin(); it != DisabledHELPGroup.end(); ++it)
	{
		ofstreamDisabledHELPGroup << *it << std::endl;
	}
	ofstreamDisabledHELPGroup.close();

	ofstream ofstreamDisabledHELPDiscuss(strFileLoc + "DisabledHELPDiscuss.RDconf", ios::in | ios::trunc);
	for (auto it = DisabledHELPDiscuss.begin(); it != DisabledHELPDiscuss.end(); ++it)
	{
		ofstreamDisabledHELPDiscuss << *it << std::endl;
	}
	ofstreamDisabledHELPDiscuss.close();

	ofstream ofstreamDisabledOBGroup(strFileLoc + "DisabledOBGroup.RDconf", ios::out | ios::trunc);
	for (auto it = DisabledOBGroup.begin(); it != DisabledOBGroup.end(); ++it)
	{
		ofstreamDisabledOBGroup << *it << std::endl;
	}
	ofstreamDisabledOBGroup.close();

	ofstream ofstreamDisabledOBDiscuss(strFileLoc + "DisabledOBDiscuss.RDconf", ios::out | ios::trunc);
	for (auto it = DisabledOBDiscuss.begin(); it != DisabledOBDiscuss.end(); ++it)
	{
		ofstreamDisabledOBDiscuss << *it << std::endl;
	}
	ofstreamDisabledOBDiscuss.close();

	ofstream ofstreamObserveGroup(strFileLoc + "ObserveGroup.RDconf", ios::out | ios::trunc);
	for (auto it = ObserveGroup.begin(); it != ObserveGroup.end(); ++it)
	{
		ofstreamObserveGroup << it->first << " " << it->second << std::endl;
	}
	ofstreamObserveGroup.close();

	ofstream ofstreamObserveDiscuss(strFileLoc + "ObserveDiscuss.RDconf", ios::out | ios::trunc);
	for (auto it = ObserveDiscuss.begin(); it != ObserveDiscuss.end(); ++it)
	{
		ofstreamObserveDiscuss << it->first << " " << it->second << std::endl;
	}
	ofstreamObserveDiscuss.close();
	//Fate
	ofstream ofstreamJRFATE(strFileLoc + "JRFATE.RDconf", ios::out | ios::trunc);
	for (auto it = JRFATE.begin(); it != JRFATE.end(); ++it)
	{
		ofstreamJRFATE << it->first << " " << it->second.Date << " " << it->second.FATEVal << std::endl;
	}
	ofstreamJRFATE.close();
	//Fate
	ofstream ofstreamCharacterProp(strFileLoc + "CharacterProp.RDconf", ios::out | ios::trunc);
	for (auto it = CharacterProp.begin(); it != CharacterProp.end(); ++it)
	{
		for (auto it1 = it->second.cbegin(); it1 != it->second.cend(); ++it1)
		{
			ofstreamCharacterProp << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " "
				<< it1->first << " " << it1->second << std::endl;
		}
	}
	ofstreamCharacterProp.close();
	ofstream ofstreamDefault(strFileLoc + "Default.RDconf", ios::out | ios::trunc);
	for (auto it = DefaultDice.begin(); it != DefaultDice.end(); ++it)
	{
		ofstreamDefault << it->first << " " << it->second << std::endl;
	}
	ofstreamDefault.close();

	ofstream ofstreamWelcomeMsg(strFileLoc + "WelcomeMsg.RDconf", ios::out | ios::trunc);
	for (auto it = WelcomeMsg.begin(); it != WelcomeMsg.end(); ++it)
	{
		while (it->second.find(' ') != string::npos)it->second.replace(it->second.find(' '), 1, "{space}");
		while (it->second.find('\t') != string::npos)it->second.replace(it->second.find('\t'), 1, "{tab}");
		while (it->second.find('\n') != string::npos)it->second.replace(it->second.find('\n'), 1, "{endl}");
		while (it->second.find('\r') != string::npos)it->second.replace(it->second.find('\r'), 1, "{enter}");
		ofstreamWelcomeMsg << it->first << " " << it->second << std::endl;
	}
	ofstreamWelcomeMsg.close();
	return 0;
}

MUST_AppInfo_RETURN(CQAPPID);
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinInet.h>

#ifdef _MSC_VER
#pragma comment(lib, "Wininet.lib")
#endif /*_MSC_VER*/

#include <cctype>
#include <ostream>
#include <sstream>

#include "Get.h"
#include "CQEVE_ALL.h"
#include "CQTools.h"
#include "RD.h"
#include "RDConstant.h"
#include "MsgFormat.h"
#include "GlobalVar.h"
#include "EncodingConvert.h"
#include "DiceNetwork.h"
#include "Jrs.h"

using namespace std;
using namespace CQ;

namespace Get
{
	int Random(int lowest, int highest)
	{
		std::random_device getGene;
		std::mt19937 gen(static_cast<unsigned int>(getGene()));
		std::uniform_int_distribution<int> dis(lowest, highest);
		return dis(gen);
	}
	void JRRP(string& into, string& out)
	{
		char* frminto = new char[into.length() + 1];
		strcpy_s(frminto, into.length() + 1, into.c_str());
		std::string des;
		bool res = Network::POST("api.kokona.tech", "/jrrp", 5555, frminto, des);
		delete[] frminto;
		if (res)
		{
			out += des;
		}
		else
		{
			out = "JRRP获取失败! 错误信息:" + des;
		}
	}

	void DND(string& strOutput, int intNum)
	{
		strOutput += "的英雄作成:";
		RD rdDND("4D6K3");
		string strDNDName[6] = { "力量", "体质", "敏捷", "智力", "感知", "魅力" };
		const bool boolAddSpace = intNum != 1;
		int intAllTotal = 0;
		while (intNum--)
		{
			strOutput += "\n";
			for (int i = 0; i <= 5; i++)
			{
				rdDND.Roll();
				strOutput += strDNDName[i] + ":" + to_string(rdDND.intTotal) + " ";
				if (rdDND.intTotal < 10 && boolAddSpace)
					strOutput += "  ";
				intAllTotal += rdDND.intTotal;
			}
			strOutput += "共计:" + to_string(intAllTotal);
			intAllTotal = 0;
		}
	}

	namespace COC
	{
		void COC7(string& strMAns, int intNum)
		{
			strMAns += "的人物作成:";
			string strProperty[] = { "力量", "体质", "体型", "敏捷", "外貌", "智力", "意志", "教育", "幸运" };
			string strRoll[] = { "3D6", "3D6", "2D6+6", "3D6", "3D6", "2D6+6", "3D6", "2D6+6", "3D6" };
			int intAllTotal = 0;
			while (intNum--)
			{
				strMAns += '\n';
				for (int i = 0; i != 9; i++)
				{
					RD rdCOC(strRoll[i]);
					rdCOC.Roll();
					strMAns += strProperty[i] + ":" + to_string(rdCOC.intTotal * 5) + " ";
					intAllTotal += rdCOC.intTotal * 5;
				}
				strMAns += "共计:" + to_string(intAllTotal);
				intAllTotal = 0;
			}
		}
		void COC6(string& strMAns, int intNum)
		{
			strMAns += "的人物作成:";
			string strProperty[] = { "力量", "体质", "体型", "敏捷", "外貌", "智力", "意志", "教育" };
			string strRoll[] = { "3D6", "3D6", "2D6+6", "3D6", "3D6", "2D6+6", "3D6", "3D6+3" };
			const bool boolAddSpace = intNum != 1;
			int intAllTotal = 0;
			while (intNum--)
			{
				strMAns += '\n';
				for (int i = 0; i != 8; i++)
				{
					RD rdCOC(strRoll[i]);
					rdCOC.Roll();
					strMAns += strProperty[i] + ":" + to_string(rdCOC.intTotal) + " ";
					if (boolAddSpace&& rdCOC.intTotal < 10)
						strMAns += "  ";
					intAllTotal += rdCOC.intTotal;
				}
				strMAns += "共计:" + to_string(intAllTotal);
				intAllTotal = 0;
			}
		}
		void TempInsane(string & strAns)
		{
			const int intSymRes = Get::Random(1, 10);
			std::string strTI = "1D10=" + to_string(intSymRes) + "\n症状: " + TempInsanity[intSymRes];
			if (intSymRes == 9)
			{
				const int intDetailSymRes = Get::Random(1, 100);
				strTI = format(strTI, { "1D10=" + to_string(Get::Random(1, 10)), "1D100=" + to_string(intDetailSymRes), strFear[intDetailSymRes] });
			}
			else if (intSymRes == 10)
			{
				const int intDetailSymRes = Get::Random(1, 100);
				strTI = format(strTI, { "1D10=" + to_string(Get::Random(1, 10)), "1D100=" + to_string(intDetailSymRes), strPanic[intDetailSymRes] });
			}
			else
			{
				strTI = format(strTI, { "1D10=" + to_string(Get::Random(1, 10)) });
			}
			strAns += strTI;
		}
		void LongInsane(string & strAns)
		{
			const int intSymRes = Get::Random(1, 10);
			std::string strLI = "1D10=" + to_string(intSymRes) + "\n症状: " + LongInsanity[intSymRes];
			if (intSymRes == 9)
			{
				const int intDetailSymRes = Get::Random(1, 100);
				strLI = format(strLI, { "1D10=" + to_string(Get::Random(1, 10)), "1D100=" + to_string(intDetailSymRes), strFear[intDetailSymRes] });
			}
			else if (intSymRes == 10)
			{
				const int intDetailSymRes = Get::Random(1, 100);
				strLI = format(strLI, { "1D10=" + to_string(Get::Random(1, 10)), "1D100=" + to_string(intDetailSymRes), strPanic[intDetailSymRes] });
			}
			else
			{
				strLI = format(strLI, { "1D10=" + to_string(Get::Random(1, 10)) });
			}
			strAns += strLI;
		}
		void Rule(string & into, string & out)
		{
			string get;
			if (Get::Rules::into(into, get))
			{
				if (get.find("规则: FAQ") != string::npos) out = "不允许FAQ词条检索";
				else out = get;
			}
			else out = "规则数据获取失败, 具体信息:" + get;
		}
	}




	namespace Pure
	{
		void PhysicalTalents(std::string& strAns)
		{
			const int intSymRes = Get::Random(1, 10);
			std::string strTa = "的肉体天赋\n1D10=" + to_string(intSymRes) + "\n " + PhysicalTalent[intSymRes];
			strAns += strTa;
		}
		void MentalTalents(std::string& strAns)
		{
			const int intSymRes = Get::Random(1, 10);
			std::string strTa = "的精神天赋\n1D10=" + to_string(intSymRes) + "\n" + MentalTalent[intSymRes];
			strAns += strTa;
		}
		void CombatTalents(std::string& strAns)
		{
			const int intSymRes = Get::Random(1, 10);
			std::string strTa = "的战斗天赋\n1D10=" + to_string(intSymRes) + "\n" + CombatTalent[intSymRes];
			strAns += strTa;
		}
		void MiscellaneousTalents(std::string& strAns)
		{
			const int intSymRes = Get::Random(1, 10);
			std::string strTa = "的其他天赋\n1D10=" + to_string(intSymRes) + "\n" + MiscellaneousTalent[intSymRes];
			strAns += strTa;
		}
		void InsaneTalents(std::string& strAns)
		{
			const int intSymRes = Get::Random(1, 20);
			std::string strTa = "的疯狂天赋天赋\n1D20=" + to_string(intSymRes) + "\n" + InsaneTalent[intSymRes];
			strAns += strTa;
		}
		void FailedCastings(std::string& strAns)
		{
			const int intSymRes = Get::Random(1, 20);
			std::string strTa = "-法术失误\n1D20=" + to_string(intSymRes) + "\n" + FailedCasting[intSymRes];
			if (intSymRes == 2 || intSymRes == 13)
			{
				strTa = format(strTa, { to_string(Get::Random(1, 6)) });
			}
			else if (intSymRes == 9)
			{
				strTa = format(strTa, { to_string(Get::Random(1, 4)) });
			}
			else if (intSymRes == 16)
			{
				strTa = format(strTa, { to_string(Get::Random(1, 10)), to_string(Get::Random(1, 10)) });
			}
			else if (intSymRes == 14)
			{
				strTa = format(strTa, { to_string(Get::Random(1, 10) + Get::Random(1, 10)) });
			}
			strAns += strTa;
		}
	}






	namespace Rules
	{
		bool into(string& rawStr, string& des)
		{
			if (rawStr.empty())
			{
				des = GlobalMsg["strRulesFormatErr"];
				return false;
			}

			for (auto& chr : rawStr)chr = toupper(static_cast<unsigned char> (chr));

			if (rawStr.find(':') != string::npos)
			{
				const string name = rawStr.substr(rawStr.find(':') + 1);
				if (name.empty())
				{
					des = GlobalMsg["strRulesFormatErr"];
					return false;
				}
				const string rule = rawStr.substr(0, rawStr.find(':'));
				if (name.empty())
				{
					des = GlobalMsg["strRulesFormatErr"];
					return false;
				}
				return out(rule, name, des);
			}
			return out("", rawStr, des);
		}
		bool out(const std::string& rule, const std::string& name, std::string& des)
		{
			const string ruleName = GBKtoUTF8(rule);
			const string itemName = GBKtoUTF8(name);

			string data = "Name=" + UrlEncode(itemName) + "&QQ=" + to_string(CQ::getLoginQQ()) + "&v=20190114";
			if (!ruleName.empty())
			{
				data += "&Type=Rules-" + UrlEncode(ruleName);
			}
			char* frmdata = new char[data.length() + 1];
			strcpy_s(frmdata, data.length() + 1, data.c_str());
			string temp;
			const bool reqRes = Network::POST("api.kokona.tech", "/rules", 5555, frmdata, temp);
			delete[] frmdata;
			if (reqRes)
			{
				des = UTF8toGBK(temp);
				return true;
			}
			if (temp == GlobalMsg["strRequestNoResponse"])
			{
				des = GlobalMsg["strRuleNotFound"];
			}
			else
			{
				des = temp;
			}
			return false;
		}
	}
}
void init(string& msg)
	{
		msg_decode(msg);
	}
void init2(string& msg)
	{
		for (int i = 0; i != msg.length(); i++)
		{
			if (msg[i] < 0)
			{
				if ((msg[i] & 0xff) == 0xa1 && (msg[i + 1] & 0xff) == 0xa1)
				{
					msg[i] = 0x20;
					msg.erase(msg.begin() + i + 1);
				}
				else if ((msg[i] & 0xff) == 0xa3 && (msg[i + 1] & 0xff) >= 0xa1 && (msg[i + 1] & 0xff) <= 0xfe)
				{
					msg[i] = msg[i + 1] - 0x80;
					msg.erase(msg.begin() + i + 1);
				}
				else
				{
					i++;
				}
			}
		}

		while (isspace(static_cast<unsigned char>(msg[0])))
			msg.erase(msg.begin());
		while (!msg.empty() && isspace(static_cast<unsigned char>(msg[msg.length() - 1])))
			msg.erase(msg.end() - 1);
		if (msg.substr(0, 2) == "。")
		{
			msg.erase(msg.begin());
			msg[0] = '.';
		}
		if (msg[0] == '!')
			msg[0] = '.';
	}

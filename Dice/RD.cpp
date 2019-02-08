#include "CQEVE_ALL.h"
#include "CQTools.h"
#include <cctype>
#include "RD.h"
#include "RDConstant.h"
#include "MsgFormat.h"
using namespace std;
using namespace CQ;

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
	if (msg.substr(0, 2) == "��")
	{
		msg.erase(msg.begin());
		msg[0] = '.';
	}
	if (msg[0] == '!')
		msg[0] = '.';
}

void COC7(string& strMAns, int intNum)
{
	strMAns += "����������:";
	string strProperty[] = {"����STR", "����CON", "����SIZ", "����DEX", "��òAPP", "����INT", "��־POW", "����EDU", "����LUCK"};
	string strRoll[] = {"3D6", "3D6", "2D6+6", "3D6", "3D6", "2D6+6", "3D6", "2D6+6", "3D6"};
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
		strMAns += "����:" + to_string(intAllTotal);
		intAllTotal = 0;
	}
}

void COC6(string& strMAns, int intNum)
{
	strMAns += "����������:";
	string strProperty[] = {"����STR", "����CON", "����SIZ", "����DEX", "��òAPP", "����INT", "��־POW", "����EDU" };
	string strRoll[] = {"3D6", "3D6", "2D6+6", "3D6", "3D6", "2D6+6", "3D6", "3D6+3"};
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
			if (boolAddSpace && rdCOC.intTotal < 10)
				strMAns += "  ";
			intAllTotal += rdCOC.intTotal;
		}
		strMAns += "����:" + to_string(intAllTotal);
		intAllTotal = 0;
	}
}

void DND(string& strOutput, int intNum)
{
	strOutput += "��Ӣ������:";
	RD rdDND("4D6K3");
	string strDNDName[6] = {"����", "����", "����", "����", "��֪", "����"};
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
		strOutput += "����:" + to_string(intAllTotal);
		intAllTotal = 0;
	}
}

void TempInsane(string& strAns)
{
	const int intSymRes = RandomGenerator::Randint(1, 10);
	std::string strTI = "1D10=" + to_string(intSymRes) + "\n֢״: " + TempInsanity[intSymRes];
	if (intSymRes == 9)
	{
		const int intDetailSymRes = RandomGenerator::Randint(1, 100);
		strTI = format(strTI, {
			"1D10=" + to_string(RandomGenerator::Randint(1, 10)), "1D100=" + to_string(intDetailSymRes), strFear[intDetailSymRes]
		});
	}
	else if (intSymRes == 10)
	{
		const int intDetailSymRes = RandomGenerator::Randint(1, 100);
		strTI = format(strTI, {
			"1D10=" + to_string(RandomGenerator::Randint(1, 10)), "1D100=" + to_string(intDetailSymRes), strPanic[intDetailSymRes]
		});
	}
	else
	{
		strTI = format(strTI, {"1D10=" + to_string(RandomGenerator::Randint(1, 10))});
	}
	strAns += strTI;
}

void LongInsane(string& strAns)
{
	const int intSymRes = RandomGenerator::Randint(1, 10);
	std::string strLI = "1D10=" + to_string(intSymRes) + "\n֢״: " + LongInsanity[intSymRes];
	if (intSymRes == 9)
	{
		const int intDetailSymRes = RandomGenerator::Randint(1, 100);
		strLI = format(strLI, {
			"1D10=" + to_string(RandomGenerator::Randint(1, 10)), "1D100=" + to_string(intDetailSymRes), strFear[intDetailSymRes]
		});
	}
	else if (intSymRes == 10)
	{
		const int intDetailSymRes = RandomGenerator::Randint(1, 100);
		strLI = format(strLI, {
			"1D10=" + to_string(RandomGenerator::Randint(1, 10)), "1D100=" + to_string(intDetailSymRes), strPanic[intDetailSymRes]
		});
	}
	else
	{
		strLI = format(strLI, {"1D10=" + to_string(RandomGenerator::Randint(1, 10))});
	}
	strAns += strLI;
}

#include "InitList.h"
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include "CQTools.h"
#include <functional>
using namespace std;


void Initlist::insert(long long group, int value, string nickname)
{
	if (!mpInitlist.count(group))
	{
		mpInitlist[group] = vector<INIT>{INIT(nickname, value)};
	}
	else
	{
		for (auto& it : mpInitlist[group])
		{
			if (it.strNickName == nickname)
			{
				it.intValue = value;
				return;
			}
		}
		mpInitlist[group].push_back(INIT(nickname, value));
	}
}

void Initlist::show(long long group, string& strMAns)
{
	if (!mpInitlist.count(group) || mpInitlist[group].empty())
	{
		strMAns = "错误:请先使用.ri指令投掷先攻值";
		return;
	}
	strMAns = "先攻顺序：";
	sort(mpInitlist[group].begin(), mpInitlist[group].end(), greater<>());
	int i = 1;
	for (const auto& it : mpInitlist[group])
	{
		strMAns += '\n' + to_string(i) + "." + it.strNickName + " " + to_string(it.intValue);
		++i;
	}
}

bool Initlist::clear(long long group)
{
	if (mpInitlist.count(group))
	{
		mpInitlist.erase(group);
		return true;
	}
	return false;
}

void Initlist::save() const
{
	ofstream ofINIT(FilePath, ios::out | ios::trunc);
	for (const auto& it : mpInitlist)
	{
		for (auto it1 : it.second)
		{
			ofINIT << it.first << " " << base64_encode(it1.strNickName) << " " << it1.intValue << endl;
		}
	}
	ofINIT.close();
}

void Initlist::read()
{
	ifstream ifINIT(FilePath);
	if (ifINIT)
	{
		long long Group = 0;
		int value;
		string nickname;
		while (ifINIT >> Group >> nickname >> value)
		{
			insert(Group, value, base64_decode(nickname));
		}
	}
	ifINIT.close();
}

Initlist::Initlist(const std::string& FilePath) : StorageBase(FilePath)
{
	Initlist::read();
}

Initlist::~Initlist()
{
	Initlist::save();
}

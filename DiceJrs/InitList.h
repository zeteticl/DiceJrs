#pragma once
#ifndef JRS_INIT
#define JRS_INIT
#include <string>
#include <map>
#include <utility>
#include <vector>
#include "StorageBase.h"

struct INIT
{
	std::string strNickName;
	int intValue = 0;
	INIT() = default;

	INIT(std::string strNickName, const int intValue = 0) : strNickName(std::move(strNickName)), intValue(intValue)
	{
	}

	bool operator>(const INIT second) const
	{
		return this->intValue > second.intValue;
	}
};

class Initlist : public StorageBase
{
	std::map<long long, std::vector<INIT>> mpInitlist;
public:
	void insert(long long group, int value, std::string nickname);
	void show(long long group, std::string& strMAns);
	bool clear(long long group);
	void save() const override;
	void read() override;
	Initlist(const std::string& FilePath);
	~Initlist();
};


#endif /*JRS_INIT*/

#pragma once
#ifndef JRS_GET_RULE
#define JRS_GET_RULE
#include <string>

namespace GetRule
{
	std::string getLastErrorMsg();
	bool analyze(std::string& rawStr, std::string& des);
	bool get(const std::string& rule, const std::string& name, std::string& des);
};
#endif /*JRS_GET_RULE*/

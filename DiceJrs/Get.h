#pragma once
#ifndef JRS_GET
#define JRS_GET
namespace Get
{
	int Random(int lowest, int highest);
	void COC6D(std::string&);
	void COC6(std::string&, int);
	void COC7D(std::string&);
	void COC7(std::string&, int);
	void DND(std::string&, int);
	void LongInsane(std::string&);
	void TempInsane(std::string&);
	//Rules
	std::string getLastErrorMsg();
	bool analyze(std::string& rawStr, std::string& des);
	bool get(const std::string& rule, const std::string& name, std::string& des);
}
void init(std::string&);
void init2(std::string&);
#endif /*JRS_GET*/

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
	void JRRP(std::string&, std::string&);
	void Rule(std::string&, std::string&);
	namespace Rules
	{ 
		std::string getLastErrorMsg();
		bool into(std::string& rawStr, std::string& des);
		bool out(const std::string& rule, const std::string& name, std::string& des);
	}
}
void init(std::string&);
void init2(std::string&);
#endif /*JRS_GET*/

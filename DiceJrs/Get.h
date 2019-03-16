#pragma once
#ifndef JRS_GET
#define JRS_GET
namespace Get
{
	int Random(int lowest, int highest);
	int RandomNet(int min, int max);
	void init(std::string&);
	void init2(std::string&);
	void COC6D(std::string&);
	void COC6(std::string&, int);
	void COC7D(std::string&);
	void COC7(std::string&, int);
	void DND(std::string&, int);
	void LongInsane(std::string&);
	void TempInsane(std::string&);
}
#endif /*JRS_GET*/

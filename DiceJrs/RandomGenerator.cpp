#include "RandomGenerator.h"
#include <random>
#include <chrono>
#include <sstream>
#include <ostream>
#include <fstream>
#include <string>
#include <iostream>
#include "DiceNetwork.h"
#include "GlobalVer.h"
#include "CQEVE_ALL.h"
#include "EncodingConvert.h"
#include "Jrs.h"

namespace RandomGenerator
{
	int Randint(int lowest, int highest)
	{
		std::random_device getGene;
		std::mt19937 gen(static_cast<unsigned int>(getGene()));
		std::uniform_int_distribution<int> dis(lowest, highest);
		return dis(gen);
	}
/*
  int Randint(int lowest, int highest)
  {
    ostringstream getRD;
    getRD << "/integers/?num=1&min=" << lowest << "&max=" << highest << "&col=1&base=10&firmat=plain&rnd=new";
    string getRDweb = getRD;
    Network::GET("www.random.org", getRDweb, 443, getRDs);
    int msgRDs std::stoi(getRDs);
    return msgRDs;
  }
*/
}




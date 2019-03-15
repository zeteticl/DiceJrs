#include "RandomGenerator.h"
#include <random>
#include <chrono>
#include <sstream>
#include <ostream>
#include <fstream>
#include <string>
#include <iostream>
#include "DiceNetwork.h"
#include "GlobalVar.h"
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
}




#include "RandomGenerator.h"
#include <random>
#include <chrono>

namespace RandomGenerator
{
	int Randint(int lowest, int highest)
	{
    std::random_device GetGene;
		std::mt19937 gen(static_cast<unsigned int>(GetGene()));
		std::uniform_int_distribution<int> dis(lowest, highest);
		return dis(gen);
	}
}




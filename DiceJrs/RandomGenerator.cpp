#include "RandomGenerator.h"
#include <random>
#include <chrono>

namespace RandomGenerator
{
	inline unsigned long long GetCycleCount()
	{
		return static_cast<unsigned long long> (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	}

	int Randint(int lowest, int highest)
	{
		std::mt19937 gen(static_cast<unsigned int>(GetCycleCount()));
		std::uniform_int_distribution<int> dis(lowest, highest);
		return dis(gen);
	}
}




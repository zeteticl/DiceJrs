#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <ostream>
#include <sstream>
#include "CQEVE_ALL.h"
#include "EncodingConvert.h"
#include "DiceNetwork.h"
#include "GlobalVar.h"
#include "Get.h"

using namespace std;
using namespace CQ;

namespace Get
{
	int RandomNet(int min, int max)
	{
		ostringstream getRDwebside;
		getRDwebside << "/integers/?num=1&min=" << min << "&max=" << max << "&col=1&base=10&firmat=plain&rnd=new";
		string get = getRDwebside.str();
		const char* getRDw = get.c_str();
		string isRDs;
			if (!Network::GET("www.random.org", getRDw, 443, isRDs))
			{
				int msgRDs = 0000;
				return msgRDs;
			}
		isRDs = UTF8toGBK(isRDs);
		int msgRDs = stoi(isRDs);
		return msgRDs;
	}
}

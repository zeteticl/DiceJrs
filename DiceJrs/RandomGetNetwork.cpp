#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <fstream>
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
    getRDwebside << "/integers/?num=1&min=" << min << "&max=" << man << "&col=1&base=10&firmat=plain&rnd=new";
    if (!Network::GET("www.random.org", getRDwebside, 443, isRDs))
	{
		int msgRDs = "0000";
		return msgRDs;
  }
	isRDs = UTF8toGBK(isRDs);
  int msgRDs std::stoi(getRDs);
  return msgRDs;
  }
}

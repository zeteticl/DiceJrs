#include "GetSCP.h"

using namespace std;

ostringstream getSCP;

namespace GetSCP
{
  toSCP(std::string strSCP)
  {
    Cstring cstrSCP(strSCP.c_str());
			if (!cstrSCP.SpanIncluding(_T("0123456789")) == cstrSCP)
			{
        std::string msgSCP = GlobalMsg["strSCPErr"];
				return msgSCP;
			}
		if (strSCP > "3999")
		{
      std::string msgSCP = GlobalMsg["strSCPErr"];
			return msgSCP;
		}
		if (strSCP.length() == 4)
		{
			if (strSCP >= "1000" && strSCP <= "1999")
			{
				getSCP.clear();
				getSCP.str("");
				getSCP << GlobalMsg["strSCP"] << strSCP << "\n" << GlobalMsg["strSCPWeb"] << "-ii/scp-" << strSCP << endl;
			  string msgSCP = getSCP.str();
        AddMsgToQueue(msgSCP, eve.fromGroup, false);
      }
			else if (strSCP >= "2000" && strSCP <= "2999")
			{
				getSCP.clear();
				getSCP.str("");
				getSCP << GlobalMsg["strSCP"] << strSCP << "\n" << GlobalMsg["strSCPWeb"] << "-iii/scp-" << strSCP << endl;
			  string msgSCP = getSCP.str();
        r;
			}
			else if (strSCP >= "3000" && strSCP <= "3999")
			{
				getSCP.clear();
				getSCP.str("");
				getSCP << GlobalMsg["strSCP"] << strSCP << "\n" << GlobalMsg["strSCPWeb"] << "-iv/scp-" << strSCP << endl;
			  string msgSCP = getSCP.str();
        AddMsgToQueue(msgSCP, eve.fromGroup, false);
			}
		}
		else if (strSCP.length() <= 3)
		{
				getSCP.clear();
				getSCP.str("");
				str.format("%03s", strSCP);
				getSCP << GlobalMsg["strSCP"] << strSCP << "\n" << GlobalMsg["strSCPWeb"] << "-i/scp-" << strSCP << endl;
			  string msgSCP = getSCP.str();
        AddMsgToQueue(msgSCP, eve.fromGroup, false);
		}
  }
}

#pragma once
#ifndef JRS_NETWORK
#define JRS_NETWORK
#include <string>

namespace Network
{
	bool POST(const char* serverName, const char* objectName, unsigned short port, char *frmdata, std::string& des);
	bool GET(const char* serverName, const char* objectName, unsigned short port, std::string& des);
}
#endif /*JRS_NETWORK*/
#pragma once
#ifndef JRS_ENCODING_CONVERT
#define JRS_ENCODING_CONVERT
#include <string>
std::string GBKtoUTF8(const std::string& strGBK);
std::string UTF8toGBK(const std::string& strUTF8);
std::string UrlEncode(const std::string& str);
std::string UrlDecode(const std::string& str);
#endif /*JRS_ENCODING_CONVERT*/
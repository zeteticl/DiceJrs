#define WIN32_LEAN_AND_MEAN
#include <string>
#include <Windows.h>
#include <cassert>

std::string GBKtoUTF8(const std::string& strGBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, nullptr, 0);
	auto* const str1 = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, len);
	len = WideCharToMultiByte(CP_UTF8, 0, str1, -1, nullptr, 0, nullptr, nullptr);
	auto* const str2 = new char[len + 1];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, len, nullptr, nullptr);
	std::string strOutUTF8(str2);
	delete[] str1;
	delete[] str2;
	return strOutUTF8;
}

std::string UTF8toGBK(const std::string& strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, nullptr, 0);
	auto* const wszGBK = new wchar_t[len + 1];
	MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, nullptr, 0, nullptr, nullptr);
	auto* const szGBK = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, nullptr, nullptr);
	std::string strTemp(szGBK);
	delete[] szGBK;
	delete[] wszGBK;
	return strTemp;
}

unsigned char ToHex(const unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}
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

unsigned char FromHex(const unsigned char x)
{
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else y = x - '0';
	return y;
}

std::string UrlEncode(const std::string & str)
{
	std::string strTemp;
	const size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (isalnum(static_cast<unsigned char>(str[i])) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ')
			strTemp += "+";
		else
		{
			strTemp += '%';
			strTemp += ToHex(static_cast<unsigned char>(str[i]) >> 4);
			strTemp += ToHex(static_cast<unsigned char>(str[i]) % 16);
		}
	}
	return strTemp;
}

std::string UrlDecode(const std::string & str)
{
	std::string strTemp;
	const size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (str[i] == '+') strTemp += ' ';
		else if (str[i] == '%')
		{
			assert(i + 2 < length);
			unsigned char high = FromHex(static_cast<unsigned char>(str[++i]));
			unsigned char low = FromHex(static_cast<unsigned char>(str[++i]));
			strTemp += high * 16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
}
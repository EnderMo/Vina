#pragma once
#include <stringapiset.h>
namespace vui
{
	class Encoding {
	public:
		static Encoding Utf8()
		{
			return Encoding(65001);
		}
		static Encoding Utf7()
		{
			return Encoding(65000);
		}
		static Encoding Ascii()
		{
			return Encoding(37);
		}
		static Encoding Default()
		{
			return Encoding(0);
		}
		static Encoding Ansi(unsigned int cp)
		{
			return Encoding(cp);
		}
		char* GetBytes(const wchar_t* str)
		{
			int size = WideCharToMultiByte(cp, 0, str, -1, nullptr, 0, nullptr, nullptr);
			char* buf = new char[size + 1];
			WideCharToMultiByte(cp, 0, str, -1, buf, size, nullptr, nullptr);
			buf[size] = '\0'; // 零终止字符串
			return buf;
		}
		wchar_t* GetString(const char* buf)
		{
			int size = MultiByteToWideChar(cp, 0, buf, -1, nullptr, 0);
			wchar_t* str = new wchar_t[size + 1];
			MultiByteToWideChar(cp, 0, buf, -1, str, size);
			str[size] = '\0'; // 零终止字符串
			return str;
		}
#ifdef _STRING_
		char* GetBytesStl(const std::wstring& str)
		{
			return GetBytes(str.c_str());
		}
		std::wstring GetStringStl(const char* buf)
		{
			auto str = GetString(buf);
			std::wstring res(str);
			delete str;
			return res;
		}
#endif // _STRING_

	protected:
		Encoding(unsigned int _cp)
		{
			cp = _cp;
		}
		unsigned int cp;
	};
}
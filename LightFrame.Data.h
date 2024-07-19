#pragma once
#include "framework.h"
#include "VertexUI/vui.parser/vui.parser.hpp"
#include <Psapi.h>
#include <ShlObj.h>
#include <direct.h>
#include <io.h>
#include <fstream>
#include <regex>
#include <string_view>
#pragma warning(disable:4996)
wchar_t LocalAppData[MAX_PATH] = L""; //Tag"\\"
wchar_t LocalData[MAX_PATH] = L""; //Tag"\\"
char LocalDataA[MAX_PATH] = ""; //Tag"\\"
wchar_t LocalData2[MAX_PATH] = L"";//Tag "/"
wchar_t LocalCache[MAX_PATH] = L""; //Tag"vui"
char LocalCacheA[MAX_PATH] = ""; //Tag"vui"
wchar_t LocalCache2[MAX_PATH] = L""; //Tag"vui //"
wchar_t LocalRes[MAX_PATH] = L""; //Tag"\\"

wchar_t LocalCom[MAX_PATH] = L"";
wchar_t LocalCards[MAX_PATH] = L"";
char LocalComA[MAX_PATH] = "";

wchar_t* s2ws(std::string str)
{
	int len = MultiByteToWideChar(936, 0, str.c_str(), str.size(), nullptr, 0);
	wchar_t* buf = new wchar_t[len + 1];
	MultiByteToWideChar(936, 0, str.c_str(), str.size(), buf, len);
	buf[len] = '\0';
	wchar_t* const ret = buf;
	return ret;
}



std::string WString2String(const std::wstring& ws)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const wchar_t* wchSrc = ws.c_str();
	size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
	char* chDest = new char[nDestSize];
	memset(chDest, 0, nDestSize);
	wcstombs(chDest, wchSrc, nDestSize);
	std::string strResult = chDest;
	delete[]chDest;
	setlocale(LC_ALL, strLocale.c_str());
	return strResult;
}
void SetTagW(wchar_t* buf, wchar_t* OldTag, wchar_t* newTag)
{
	wchar_t* start;
	int newlen = wcslen(newTag);
	int oldlen = wcslen(OldTag);
	start = wcsstr(buf, OldTag);
	while (start) {
		if (newlen > oldlen) wmemmove(start + newlen - oldlen, start, wcslen(start) + 1);
		if (newlen < oldlen) wmemmove(start, start + oldlen - newlen, wcslen(start) + 1);
		wmemmove(start, newTag, newlen);
		start += newlen;
		start = wcsstr(buf, OldTag);
	}

}
/*
void SetTagW(wchar_t*& buf, wchar_t* OldTag, wchar_t* newTag)
{

	wchar_t* start;
	wchar_t start2[260] = L"";
	swprintf(start2, L"%s", buf);
	int newlen = wcslen(newTag);
	int oldlen = wcslen(OldTag);
	start = wcsstr(start2, OldTag);
	while (start) {
		if (newlen > oldlen) wmemmove(start + newlen - oldlen, start, wcslen(start) + 1);
		if (newlen < oldlen) wmemmove(start, start + oldlen - newlen, wcslen(start) + 1);
		wmemmove(start, newTag, newlen);
		start += newlen;
		start = wcsstr(start2, OldTag);

		wchar_t* start3 = new wchar_t[260];
		swprintf(start3, L"%s", start2);
		buf = start3;
	}

}
*/
void SetDirTag(char* buf, char* OldTag, char* newTag)
{
	char* start;
	int newlen = strlen(newTag);
	int oldlen = strlen(OldTag);
	start = strstr(buf, OldTag);
	while (start) {
		if (newlen > oldlen) memmove(start + newlen - oldlen, start, strlen(start) + 1);
		if (newlen < oldlen) memmove(start, start + oldlen - newlen, strlen(start) + 1);
		memmove(start, newTag, newlen);
		start += newlen;
		start = strstr(buf, OldTag);
	}

}
void Wchar_tToString(std::string& szDst,const wchar_t* wchar)
{
	const wchar_t* wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);
	char* psText;  
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);
	szDst = psText;
	delete[]psText;
}
std::size_t replace_all(std::string& inout, std::string_view what, std::string_view with)
{
	std::size_t count{};
	for (std::string::size_type pos{};
		inout.npos != (pos = inout.find(what.data(), pos, what.length()));
		pos += with.length(), ++count) {
		inout.replace(pos, what.length(), with.data(), with.length());
	}
	return count;
}
std::wstring getMiddleStringW(std::wstring ori, std::wstring start, std::wstring end,int RightMove=0)
{
	std::wstring ret;
	size_t pos = ori.find(start)+start.length();
	size_t pos2 = ori.find(end,pos);
	ret =ori.substr(pos, pos2+RightMove -pos);
	return ret;
}


/*
void SetTagWW(const wchar_t*& buf, wchar_t* OldTag, wchar_t* newTag)
{
	std::string deststr;
	Wchar_tToString(deststr, (wchar_t*)buf);
	std::string begin; Wchar_tToString(begin, OldTag);
	std::string end; Wchar_tToString(end, newTag);
	replace_all(deststr, begin, end);
	std::wstring dest1 = s2ws(deststr);
	buf = dest1.c_str();

}
*/
void replace_all(std::wstring& inout, std::wstring what, std::wstring with) //跟着cppreference抄的（
{
	for (std::wstring::size_type pos{};
		inout.npos != (pos = inout.find(what.data(), pos, what.length()));
		pos += with.size()) {
		inout.replace(pos, what.length(), with.data(), with.length());
	}
}
//返回指针是否为PE数据或者只读数据
/*
void SetTagW(wchar_t* buf, wchar_t* OldTag, wchar_t* newTag)
{
	std::wstring inout = std::wstring(buf); //拷贝c式字符串到c艹式字符串
	std::wstring what = std::wstring(OldTag); //存储到本地变量以便编译器添加后续释放代码（终结器）
	std::wstring with = std::wstring(newTag);
	replace_all(inout, what, with); //替换
	lstrcpyW(buf, (wchar_t*)inout.c_str()); //管他够不够缓冲区,复制过去就是了（（（
}
*/
void SetTagWW(const wchar_t*& buf,const wchar_t* OldTag,const wchar_t* newTag)
{
	//if (!IsDataOrRData((void*)buf)) {}else delete buf; 
	std::wstring inout = std::wstring(buf); //拷贝c式字符串到c艹式字符串
	std::wstring what = std::wstring(OldTag); //存储到本地变量以便编译器添加后续释放代码（终结器）
	std::wstring with = std::wstring(newTag);
	replace_all(inout, what, with); //替换
	//if (!IsDataOrRData((void*)buf)) {}//OutputDebugString(L"内存在不知不觉中溢出了,但你无法察觉,建议打死这个作者\n");
		//delete buf; //删除缓冲区，如果不是Data的话
	buf = (new std::wstring(inout))->c_str(); //拷贝并转换为c式字符串
}
void mkdirs(wchar_t* muldir)
{
	int i, len;
	wchar_t str[512];
	wcsncpy(str, muldir, 512);
	len = wcslen(str);
	for (i = 0; i < len; i++)
	{
		if (str[i] == '/')
		{
			str[i] = '\0';
			if (_waccess(str, 0) != 0)
			{
				_wmkdir(str);
			}
			str[i] = '/';
		}
	}
	if (len > 0 && _waccess(str, 0) != 0)
	{
		_wmkdir(str);
	}
	return;
}
void imkdirs(wchar_t* muldir)
{
	int i, len;
	wchar_t str[512];
	wcsncpy(str, muldir, 512);
	len = wcslen(str);
	for (i = 0; i < len; i++)
	{
		if (str[i] == '\0')
		{
			str[i] = '\0';
			if (_waccess(str, 0) != 0)
			{
				_wmkdir(str);
			}
			str[i] = '\0';
		}
	}
	if (len > 0 && _waccess(str, 0) != 0)
	{
		_wmkdir(str);
	}
	return;
}

int GetTxtLine(const wchar_t* str) {
	int n = 0;
	if (NULL == str) return 0;
	if (str[0] == 0) return 0;
	while (*str) { if (*str == L'\n') n++; str++; }
	if (str[-1] != L'\n') n++;//最后一行末尾没有'\n'也算一行
	return n;
}
void GetAppdataPath(wchar_t* ud)
{
	wchar_t szDocument[MAX_PATH] = { 0 };

	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pidl);
	if (pidl && SHGetPathFromIDList(pidl, szDocument))
	{
		GetShortPathName(szDocument, ud, _MAX_PATH);
	}
}
void GetLocalAppdataPath(wchar_t* ud)
{
	wchar_t szDocument[MAX_PATH] = { 0 };

	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
	if (pidl && SHGetPathFromIDList(pidl, szDocument))
	{
		GetShortPathName(szDocument, ud, _MAX_PATH);
	}
}
void GetAppdataPathA(char ud[MAX_PATH])
{
	char szDocument[MAX_PATH] = { 0 };

	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pidl);
	if (pidl && SHGetPathFromIDListA(pidl, szDocument))
	{
		GetShortPathNameA(szDocument, ud, _MAX_PATH);
		//strcat("\\CommandCube", ud);
	}
}
void GetLocalAppdataPathA(char ud[MAX_PATH])
{
	char szDocument[MAX_PATH] = { 0 };

	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
	if (pidl && SHGetPathFromIDListA(pidl, szDocument))
	{
		GetShortPathNameA(szDocument, ud, _MAX_PATH);
		//strcat("\\CommandCube", ud);
	}
}

const wchar_t* VUIGetObject(vui::parser::fparser& obj, const char* data)
{

	std::string d1;
	obj.get(data, d1);
	return s2ws(d1);
}
const wchar_t* VUIGetObjectEx(vui::parser::fparser& obj, const char* data, const std::optional<std::string>& name)
{

	std::string d1;
	obj.get(data, d1,name);
	return s2ws(d1);
}
const wchar_t* VUIGetObjectW(vui::parser::wparser& obj, const wchar_t* data)
{

	std::wstring d1;
	obj.get(data, d1);
	const wchar_t* newstr = d1.c_str();
	const int len = (int)d1.length();
	wchar_t* str = (wchar_t*)malloc(len);
	wcscpy(str, newstr);
	return str;
}
const wchar_t* VUIGetObjectfW(vui::parser::wfparser& obj, const wchar_t* data, const std::optional<std::wstring>& name)
{

	std::wstring d1;
	obj.get(data, d1, name);
	const wchar_t* newstr = d1.c_str();
	const int len = (int)d1.length();
	wchar_t* str = (wchar_t*)malloc(len);
	wcscpy(str, newstr);
	return str;
}
bool AllisNum(std::wstring str)
{
	for (int i = 0; i < str.size(); i++)
	{
		int tmp = (int)str[i];
		if (tmp >= 48 && tmp <= 57)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	return true;
}
long hextonum(wchar_t* in) {
	// 用指针循环字符串
	wchar_t* pin = in;
	// 以累加方式获得输出结果
	long out = 0;

	while (*pin != 0) {
		// 针对每一个输入字符，依次移位out到高一级(4位对应一个bit)
		out <<= 4;
		// 对这个字符进行位运算，并且结果加到out变量的低位上。
		out += (*pin < 'A') ? *pin & 0xF : (*pin & 0x7) + 9;

		pin++;
	}

	return out;
}

void GetFrameData()
{

}
#pragma once
#include <windows.h>
namespace win32 {
#define ExportFunc(x) public: decltype(x)* x = ExportFunction(#x,DllName);
	class ExportFunction {
	public:
		ExportFunction(const char* name,const wchar_t* DllName)
		{
			this->name = name;
			dll = DllName;
		}
		const wchar_t* dll;
		const char* name;
		template<class Func>
		operator Func()
		{
			return Func(GetProcAddress(GetModuleHandle(dll),name));
		}
	};
	size_t RtlGetVersion(
		OSVERSIONINFOW* lpVersionInformation
	);
	class NtApi {
#define DllName L"ntdll.dll"
		ExportFunc(RtlGetVersion)
#undef DllName
	};
#undef ExportFunc
}
#pragma once
#include <WinUser.h>
#include <vector>
#include <wtypes.h>
#include <functional>
namespace vui {
	class Dispatcher {
	public:
		Dispatcher() {
		}
		void Run()
		{
			MSG msg;
			while (GetMessage(&msg, nullptr, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		void Stop()
		{
			PostQuitMessage(0);
		}
	};
}
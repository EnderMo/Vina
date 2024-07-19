#pragma once
#include <wtypes.h>
#include <typeinfo>
#include "Encoding.h"
#include "VuiException.h"
namespace vui
{
	constexpr UINT WM_SETTHIS = WM_USER + 114514;
	class Window
	{
	public:
		HWND handle()
		{
			CheckHwnd();
			return hwnd;
		}
		void CreateControl()
		{
			if (hwnd != nullptr)
				throw LogicException::LogicException(L"Control handle created.", __LINE__, __FILEW__);
			CreateClass();
			hwnd = CreateWindowExW(0, L"VuiFoundation::Window", TEXT(""), 0, 0, 0, 0, 0, 0, 0, GetModuleHandleW(NULL), nullptr);
			SendMessageW(hwnd, WM_SETTHIS, 0, (LPARAM)this);
			SetWindowPos(hwnd, 0, 0, 0, 0, 0, 0); // resize to avoid missing event
			SendMessageW(hwnd, WM_CREATE, 0, 0);  // created !
			Init();
		}
		void Activate()
		{
			CheckHwnd_th();
			Show();
			// DefWindowProc(hwnd, WM_SYSCOMMAND, SC_RESTORE,0);
			SetForegroundWindow(hwnd);
		}
		void SetCenter()
		{
			CheckHwnd_th();
			int scrWidth, scrHeight;
			RECT rect;
			//获得屏幕尺寸
			scrWidth = GetSystemMetrics(SM_CXSCREEN);
			scrHeight = GetSystemMetrics(SM_CYSCREEN);
			//取得窗口尺寸
			GetWindowRect(hwnd, &rect);
			//重新设置rect里的值
			long width = rect.right - rect.left;
			long height = rect.bottom - rect.top;
			rect.left = (scrWidth - width) / 2;
			rect.top = (scrHeight - height) / 2;

			//移动窗口到指定的位置
			SetWindowPos(hwnd, HWND_TOP, rect.left, rect.top, width, height, SWP_NOSIZE | SWP_NOZORDER);
		}
		void SetLocation(POINT pt)
		{
			CheckHwnd_th();
			SetWindowPos(hwnd, 0, pt.x, pt.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}
		POINT GetLocation()
		{
			CheckHwnd_th();
			RECT rt;
			GetWindowRect(hwnd, &rt);
			return POINT{rt.left, rt.top};
		}
		_declspec(property(put = SetLocation, get = GetLocation)) POINT Location;
		void SetSize(SIZE sz)
		{
			CheckHwnd_th();
			SetWindowPos(hwnd, 0, 0, 0, sz.cx, sz.cy, SWP_NOZORDER | SWP_NOMOVE);
		}
		SIZE GetSize()
		{
			CheckHwnd_th();
			RECT rt;
			GetClientRect(hwnd, &rt);
			return SIZE{rt.right - rt.left, rt.bottom - rt.top};
		}
		enum class WindowState
		{
			Maximzed = SW_SHOWMAXIMIZED,
			Minimized = SW_SHOWMINIMIZED,
			Normal = SW_SHOWNORMAL,
		};
		WindowState GetWindowState()
		{
			CheckHwnd_th();
			WINDOWPLACEMENT wp{0};
			GetWindowPlacement(hwnd, &wp);
			return (WindowState)wp.showCmd;
		}
		void SetWindowState(WindowState ws)
		{
			CheckHwnd_th();
			switch (ws)
			{
			case WindowState::Normal:
				DefWindowProc(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
				break;
			case WindowState::Maximzed:
				DefWindowProc(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
				break;
			case WindowState::Minimized:
				DefWindowProc(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
				break;
			default:
				break;
			}
		}
		_declspec(property(put = SetWindowState, get = GetWindowState)) WindowState State;
		// HDC GetDc()
		//{
		//	CheckHwnd_th();
		//	return GetDC(hwnd);
		// }
		_declspec(property(put = SetSize, get = GetSize)) SIZE Size;
		LONG GetStyle()
		{
			CheckHwnd_th();
			return GetWindowLongW(hwnd, GWL_STYLE);
		}
		void SetStyle(LONG val)
		{
			CheckHwnd_th();
			SetWindowLongW(hwnd, GWL_STYLE, val);
		}
		_declspec(property(put=SetOpacity)) int Opacity;
		void SetOpacity(int alpha)
		{
			CheckHwnd_th();
			ExStyle = ExStyle | WS_EX_LAYERED;
			SetLayeredWindowAttributes(hwnd,0,alpha,LWA_ALPHA);
		}
		_declspec(property(put = SetStyle, get = GetStyle)) LONG Style;
		LONG GetExStyle()
		{
			CheckHwnd_th();
			return GetWindowLongW(hwnd, GWL_EXSTYLE);
		}
		void SetExStyle(LONG val)
		{
			CheckHwnd_th();
			SetWindowLongW(hwnd, GWL_EXSTYLE, val);
		}
		_declspec(property(put = SetExStyle, get = GetExStyle)) LONG ExStyle;
		void Hide()
		{
			CheckHwnd_th();
			// DefWindowProc(hwnd,WM_SYSCOMMAND,SC_CLOSE,0);
			ShowWindow(hwnd, SW_HIDE);
		}
		void Show()
		{
			CheckHwnd();
			// DefWindowProc(hwnd,WM_SYSCOMMAND,SC_HOTKEY,(LPARAM)hwnd);
			ShowWindow(hwnd, SW_SHOW);
		}
		void ShowDialog()
		{
			CheckHwnd();
			ShowWindow(hwnd, SW_SHOW);
			MSG msg;
			while (GetMessageW(&msg, 0, 0, 0))
			{
				if (msg.hwnd != hwnd)
					continue;
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}
		void Invalidate()
		{
			RECT rt;
			GetClientRect(hwnd, &rt);
			InvalidateRect(hwnd, &rt, false);
		}
		void Destroy()
		{
			if (hwnd != nullptr)
			{
				DestroyWindow(hwnd);
			}
		}
		virtual ~Window()
		{
			Destroy();
		}

	protected:
		LRESULT CalcHittest(HWND h, POINT pt, int TitleBarTopSize)
		{
			::ScreenToClient(h, &pt);

			RECT rcClient;
			::GetClientRect(h, &rcClient);
			if (pt.x < rcClient.left + 5 && pt.y < rcClient.top + 5)
				return HTTOPLEFT;
			else if (pt.x > rcClient.right - 5 && pt.y < rcClient.top + 5)
				return HTTOPRIGHT;
			else if (pt.x < rcClient.left + 5 && pt.y > rcClient.bottom - 5)
				return HTBOTTOMLEFT;
			else if (pt.x > rcClient.right - 5 && pt.y > rcClient.bottom - 5)
				return HTBOTTOMRIGHT;
			else if (pt.x < rcClient.left + 5)
				return HTLEFT;
			else if (pt.x > rcClient.right - 5)
				return HTRIGHT;
			else if (pt.y < rcClient.top + 5)
				return HTTOP;
			else if (pt.y > rcClient.bottom - 5)
				return HTBOTTOM;
			return HTCLIENT;
		}
		virtual void Init()
		{
		}
		void CheckHwnd_th()
		{
			if (hwnd == nullptr)
				throw LogicException::LogicException(L"Control handle didn't create.", __LINE__, __FILEW__);
		}
		void CheckHwnd()
		{
			if (hwnd == nullptr)
			{
				CreateControl();
			}
		}
		virtual void CreateClass()
		{
			if (classatom == 0)
			{
				CreateClassInternal(L"VuiFoundation::Window");
			}
		}
		virtual LPARAM WndProc(HWND h, UINT msg, WPARAM wp, LPARAM lp)
		{
			return DefWindowProc(h, msg, wp, lp);
		}
		virtual void CreateClassInternal(const wchar_t *cls)
		{
			HINSTANCE hinst = GetModuleHandleW(NULL);
			WNDCLASSW wc{0};
			if (!GetClassInfoW(hinst, cls, &wc))
			{
				wc.lpszClassName = cls;
				wc.hCursor = LoadCursor(NULL, IDC_ARROW);
				wc.lpfnWndProc = WndProcStub;
				classatom = RegisterClassW(&wc);
				return;
			}
			classatom = 10000;
		}
		static LPARAM CALLBACK WndProcStub(HWND h, UINT msg, WPARAM wp, LPARAM lp)
		{
			auto pthis = (void *)GetWindowLongPtr(h, GWLP_USERDATA);
			if (pthis == nullptr)
			{
				switch (msg) // default wndproc
				{
				case WM_SETTHIS:
					SetWindowLongPtr(h, GWLP_USERDATA, lp);
				}
				return DefWindowProc(h, msg, wp, lp);
			}
			else
			{
				return ((Window *)pthis)->WndProc(h, msg, wp, lp);
			}
		}
		ATOM classatom = 0;
		HWND hwnd = nullptr;
	};
}
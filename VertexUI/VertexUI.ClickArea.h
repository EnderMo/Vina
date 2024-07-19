#pragma once
/*
* VertexUI ClickArea
*
* By EnderMo,VertexStudio 2021
*
*/
#include "framework.h"
#include "VertexUI.Panel.h"
#include <stdio.h>


typedef int RUNFUN;
namespace VertexUI
{
	namespace Click
	{
		int hState = 0;
		int ClickMsg = 0;
		int KeepDownMsg = 0;
		int ScrollMsg = 0;
		int RClickMsg = 0;
		int RDClickMsg = 0;
		int LDClickMsg = 0;
		template<class rf>
		int ClickArea(HWND hWnd, int x, int y, int sizex, int sizey, LPARAM lParam, rf function)
		{
			POINT pt;
			wchar_t txt[80];
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			wsprintf(txt, L"x=%d;y=%d\n", LOWORD(lParam), HIWORD(lParam));
			OutputDebugString(txt);
			RECT rect;
			GetClientRect(hWnd, &rect);
			RECT  rc = { x*gScale,y * gScale,(x + sizex)*gScale,(y + sizey)*gScale };
			if (PtInRect(&rc, pt))
			{
				function();
				return -1;
			}
			return -1;
		}
		int GetPtInfo(const vinaPoint& vp, int x, int y, int sizex, int sizey, RECT& rcx)
		{
			POINT pt;
			//wchar_t txt[80];
			pt.x = vp.x;
			pt.y = vp.y;
			//ClientToScreen(hWnd, &pt);
			RECT  rc = { x * gScale,y * gScale,(x + sizex) * gScale,(y + sizey) * gScale };
			rcx = { (long)(x * gScale),(long)(y * gScale),(long)((x + sizex) * gScale),(long)((y + sizey) * gScale) };
			if (PtInRect(&rc, pt))
			{
				return 1;
			}
			else
			{
				return 0;
			}
			return -1;
		}
		int GetPtInfo(HWND hWnd, int x, int y, int sizex, int sizey)
		{
			RECT rc;
			POINT pt;
			GetCursorPos(&pt);
			//wsprintf(txt, L"x=%d;y=%d\n", LOWORD(lParam), HIWORD(lParam));
		   // OutputDebugString(txt);
			ScreenToClient(hWnd, &pt);
			vinaPoint vp;
			vp.x = pt.x;
			vp.y = pt.y;
			return GetPtInfo(vp, x, y, sizex, sizey, rc);
		}
		int GetPtInfo(const vinaPoint& vpt, int x, int y, int sizex, int sizey)
		{
			RECT rc;
			
			return GetPtInfo(vpt, x, y, sizex, sizey, rc);
		}
		int GetPtInfo(HWND hWnd, VertexUIPos vpos, RECT& rcx)
		{
			RECT rc;
			POINT pt;
			GetCursorPos(&pt);
			//wsprintf(txt, L"x=%d;y=%d\n", LOWORD(lParam), HIWORD(lParam));
		   // OutputDebugString(txt);
			ScreenToClient(hWnd, &pt);
			vinaPoint vp;
			vp.x = pt.x;
			vp.y = pt.y;
			return GetPtInfo(vp, vpos.x, vpos.y, vpos.cx, vpos.cy, rcx);
		}
		int GetScreenPtInfo(int x, int y, int sizex, int sizey, RECT& rcx)
		{
			POINT pt;
			//wchar_t txt[80];
			GetCursorPos(&pt);
			//wsprintf(txt, L"x=%d;y=%d\n", LOWORD(lParam), HIWORD(lParam));
		   // OutputDebugString(txt);
			RECT  rc = { x,y,x + sizex,y + sizey };
			rcx = { x,y,x + sizex,y + sizey };
			if (PtInRect(&rc, pt))
			{
				return 1;
			}
			else
			{
				return 0;
			}
			return -1;
		}
		int _ClickArea(HWND hWnd, RECT rc, LPARAM lParam, RUNFUN function())
		{
			POINT pt;
			wchar_t txt[80];
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			wsprintf(txt, L"x=%d;y=%d\n", LOWORD(lParam), HIWORD(lParam));
			OutputDebugString(txt);
			RECT rect;
			GetClientRect(hWnd, &rect);
			if (PtInRect(&rc, pt))
			{
				function();
				return -1;
			}
			return -1;
		}

		int ClickAreaEx(HWND hWnd, int x, int y, int sizex, int sizey, LPARAM lParam, RUNFUN function(), int msg)
		{
			POINT pt;
			wchar_t txt[80];
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			wsprintf(txt, L"x=%d;y=%d\n", LOWORD(lParam), HIWORD(lParam));
			OutputDebugString(txt);
			RECT rect;
			GetClientRect(hWnd, &rect);
			RECT  rc = { x,y,x + sizex,y + sizey };
			if (PtInRect(&rc, pt))
			{
				if (msg == 1)
				{
					function();
					return 0;
				}
				if (msg == 0)
				{
					return -1;
				}
				else
				{
					return -1;
				}
				return -1;
			}
			return -1;
		}

		int GetAreaPtInfo(HWND hWnd, int x, int y, int sizex, int sizey, RECT& rc2, LPARAM lParam)
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			RECT rect;
			GetClientRect(hWnd, &rect);
			RECT  rc = { x,y,x + sizex,y + sizey };
			rc2 = { x,y,x + sizex,y + sizey };
			if (PtInRect(&rc, pt))
			{
				return 1;
			}
			else
			{
				return 0;
			}
			return -1;
		}

		int ClickAreaPtInfo(HWND hWnd, int x, int y, int sizex, int sizey, LPARAM lParam, int& value)
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			RECT rect;
			GetClientRect(hWnd, &rect);
			RECT  rc = { x,y,x + sizex,y + sizey };
			if (PtInRect(&rc, pt))
			{
				value = 1;
				return -1;
			}
			else
			{
				value = 0;
				return -1;
			}
			return -1;
		}

		int _CreateClickEvent(HWND hWnd, int x, int y, int sizex, int sizey, LPARAM lParam, RUNFUN function(), int msg)
		{
			ClickAreaEx(hWnd, x, y, sizex, sizey, lParam, function, msg);
			return 0;
		}

		int CreateClickEvent(HWND hWnd, int x, int y, int sizex, int sizey, LPARAM lParam, RUNFUN function(), const wchar_t* Panelid)
		{
			int msg = 0;
			if (Panelid == PanelID)
			{
				msg = 1;
			}
			if (Panelid == PrevPanelID)
			{
				msg = 1;
			}

			else
			{
				msg = 0;
			}
			ClickAreaEx(hWnd, x, y, sizex, sizey, lParam, function, msg);
			return 0;
		}

		int _NewValue(int& ov, int v)
		{
			ov = v;
			return ov;
		}


		void SnedClickEvent(HWND hWnd, WPARAM w, LPARAM l)
		{
			ClickMsg = 1;
			SendMessage(hWnd, WM_MOUSEMOVE, w, l);
			ClickMsg = 0;
			KeepDownMsg = 0;
		}
		void SendClickEvent(HWND hWnd, WPARAM w, LPARAM l)
		{
			ClickMsg = 1;
			SendMessage(hWnd, WM_MOUSEMOVE, w, l);
			ClickMsg = 0;
			KeepDownMsg = 0;
		}
		void SendKeepDownEvent(HWND hWnd, WPARAM w, LPARAM l)
		{
			KeepDownMsg = 1;
			SendMessage(hWnd, WM_MOUSEMOVE, w, l);
		}
		void SendScrollEvent(HWND hWnd, WPARAM w, LPARAM l)
		{
			ScrollMsg = 1;
			SendMessage(hWnd, WM_MOUSEMOVE, w, l);
		}
		void SendRClickEvent(HWND hWnd, WPARAM w, LPARAM l)
		{
			RClickMsg = 1;
			SendMessage(hWnd, WM_MOUSEMOVE, w, l);
			RClickMsg = 0;
		}
		void SendRDClickEvent(HWND hWnd, WPARAM w, LPARAM l)
		{
			RDClickMsg = 1;
			SendMessage(hWnd, WM_MOUSEMOVE, w, l);
			RDClickMsg = 0;
		}
		void SendDClickEvent(HWND hWnd, WPARAM w, LPARAM l)
		{
			LDClickMsg = 1;
			SendMessage(hWnd, WM_MOUSEMOVE, w, l);
			LDClickMsg = 0;
		}


	}

}

bool CtlControllerForceStop = false;

#define add_eventPtr(ctl) if(ctl->AddEvent()==0)return 0
#define add_event(ctl) if(ctl.AddEvent()==0)return 0

#define AddBoxClickArea(hWnd,lParam,x,y,cx,cy,runfun) if ((GetPtInfo(hWnd, x, y, cx, cy, rc)) == 1)\
        {\
        if (ClickMsg == 1)\
        {\
            ClickMsg = 0;\
            runfun();\
        }\
        if (hState == 0)\
        {\
            HDC hdc = GetDC(hWnd);\
            CreateRect(hWnd, hdc, rc.left, rc.bottom - 5, (rc.right - rc.left), 5, VERTEXUICOLOR_LAVENDER);\
            DeleteObject(hdc);\
            ReleaseDC(hWnd, hdc);\
            DeleteDC(hdc);\
            hState = 1;\
        }\
        return 0;\
        }
#define AddBoxClickAreaDDraw(hWnd,lParam,x,y,cx,cy,runfun,ia) if ((GetAreaPtInfo(hWnd, x, y, cx, cy, rc, lParam)) == 1)\
        {\
        if (ClickMsg == 1)\
        {\
            ClickMsg = 0;\
            runfun();\
        }\
        if (hState == 0)\
        {\
            ia();\
            hState = 1;\
        }\
        return 0;\
        }
#define AddBoxClickArea_Animation(hWnd,lParam,x,y,cx,cy,runfun) if ((GetAreaPtInfo(hWnd, x, y, cx, cy, rc, lParam)) == 1)\
        {\
        if (ClickMsg == 1)\
        {\
            ClickMsg = 0;\
            runfun();\
        }\
        if (hState == 0)\
        {\
            HDC hdc = GetDC(hWnd);\
            ClickAreaBoxHoverAnimation(hWnd,hdc,rc);\
            DeleteObject(hdc);\
            ReleaseDC(hWnd, hdc);\
            DeleteDC(hdc);\
            hState = 1;\
        }\
        return 0;\
        }

//窗口,鼠标传参,x,y,大小,大小,x偏移量,大小偏移量,高度偏移量,颜色,运行


#define AddBoxClickAreaEx(hWnd,lParam,x,y,cx,cy,x1,wsize,hsize,color,runfun) if ((GetAreaPtInfo(hWnd, x, y, cx, cy, rc, lParam)) == 1)\
        {\
        if (ClickMsg == 1)\
        {\
            ClickMsg = 0;\
            runfun();\
        }\
        if (hState == 0)\
        {\
            HDC hdc = GetDC(hWnd);\
            CreateRect(hWnd, hdc, rc.left + x1, rc.bottom - hsize, rc.right - rc.left - wsize, hsize,color);\
            DeleteObject(hdc);\
            ReleaseDC(hWnd, hdc);\
            DeleteDC(hdc);\
            hState = 1;\
        }\
        return 0;\
        }

#define AddBoxClickAreaExExit(hWnd,lParam,x,y,cx,cy,x1,wsize,hsize,color,runfun) if ((GetAreaPtInfo(hWnd, x, y, cx, cy, rc, lParam)) == 1)\
        {\
        if (ClickMsg == 1)\
        {\
            ClickMsg = 0;\
            runfun();\
            DestroyWindow(hWnd);\
        }\
        if (hState == 0)\
        {\
            HDC hdc = GetDC(hWnd);\
            CreateRect(hWnd, hdc, rc.left + x1, rc.bottom - hsize, rc.right - rc.left - wsize, hsize,color);\
            DeleteObject(hdc);\
            ReleaseDC(hWnd, hdc);\
            DeleteDC(hdc);\
            hState = 1;\
        }\
        return 0;\
        }

#define _AddBoxClickAreaEx(hWnd,lParam,x,y,cx,cy,x1,wsize,hsize,color,runfun,dwf) if ((GetAreaPtInfo(hWnd, x, y, cx, cy, rc, lParam)) == 1)\
        {\
        if (ClickMsg == 1)\
        {\
            ClickMsg = 0;\
            runfun();\
        }\
        if (hState == 0)\
        {\
            dwf\
            hState = 1;\
        }\
        return 0;\
        }
#define AddCtlEvent(hWnd,lParam,ctl) AddBoxClickArea(hWnd,lParam,ctl.x,ctl.y,ctl.sizex,ctl.sizey,ctl.runfun)
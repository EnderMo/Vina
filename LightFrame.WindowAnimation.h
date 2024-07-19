#pragma once
#include "VertexUI/VertexUI.min.h"

VertexUIInit;
#pragma once
#include "framework.h"
int RedrawState = 1;

int FlagRefreshUI = 0;
BYTE Alpha = 255;
#define AlphaSet         SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED); \
SetLayeredWindowAttributes(hWnd, 0, Alpha, LWA_ALPHA);



int FadeOutAnimation(HWND hWnd)
{
	Sleep(20);
	Alpha = 255; AlphaSet;
	Sleep(20);
	Alpha = 215; AlphaSet;
	Sleep(20);
	Alpha = 193; AlphaSet;
	Sleep(20);
	Alpha = 142; AlphaSet;
	Sleep(20);
	Alpha = 99; AlphaSet;
	Sleep(20);
	Alpha = 40; AlphaSet;
	Sleep(20);
	Alpha = 10; AlphaSet;
	return 0;
}
int FadeOutAnimation2(HWND hWnd, int t = 15)
{
	RedrawState = 0;
	RECT rc;
	GetClientRect(hWnd, &rc);
	XSleep(t);
	SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top - 2, SWP_NOMOVE);
	Alpha = 251; AlphaSet;
	XSleep(t);
	SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top - 6, SWP_NOMOVE);
	Alpha = 245; AlphaSet;
	XSleep(t);
	SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top - 18, SWP_NOMOVE);
	Alpha = 235; AlphaSet;
	XSleep(t);
	SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top - 29, SWP_NOMOVE);
	Alpha = 225; AlphaSet;
	XSleep(t);
	SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top - 46, SWP_NOMOVE);
	Alpha = 205; AlphaSet;
	XSleep(t);
	SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top - 86, SWP_NOMOVE);
	Alpha = 185; AlphaSet;
	XSleep(t);
	SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top - 128, SWP_NOMOVE);
	Alpha = 165; AlphaSet;
	XSleep(t);
	SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top - 176, SWP_NOMOVE);
	Alpha = 145; AlphaSet;
	XSleep(t);
	SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top - 244, SWP_NOMOVE);
	Alpha = 105; AlphaSet;
	XSleep(t);
	SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top - 339, SWP_NOMOVE);
	Alpha = 75; AlphaSet;
	XSleep(t);
	SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top - 469, SWP_NOMOVE);
	Alpha = 45; AlphaSet;
	XSleep(t);
	SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top - 519, SWP_NOMOVE);
	Alpha = 31; AlphaSet;
	XSleep(t);
	SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top - 569, SWP_NOMOVE);
	Alpha = 25; AlphaSet;
	XSleep(t);
	SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top - 590, SWP_NOMOVE);
	Alpha = 15; AlphaSet;
	XSleep(t);
	SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top - 605, SWP_NOMOVE);
	Alpha = 6; AlphaSet;
	XSleep(t);
	SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top - 615, SWP_NOMOVE);
	Alpha = 2; AlphaSet;
	XSleep(t);
	SetWindowPos(hWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top - 620, SWP_NOMOVE);
	Alpha = 0; AlphaSet;
	RedrawState = 1;
	XSleep(t);
	return 0;
}
int FadeInAnimation(HWND hWnd)//But it cause window shut down qwq
{
	XSleep(15);
	Alpha = 0; AlphaSet;
	XSleep(15);
	Alpha = 20; AlphaSet;
	XSleep(15);
	Alpha = 40; AlphaSet;
	XSleep(15);
	Alpha = 70; AlphaSet;
	XSleep(15);
	Alpha = 110; AlphaSet;
	XSleep(15);
	Alpha = 140; AlphaSet;
	XSleep(15);
	Alpha = 180; AlphaSet;
	XSleep(15);
	Alpha = 210; AlphaSet;
	XSleep(15);
	Alpha = 250; AlphaSet;
	XSleep(15);
	Alpha = 255; AlphaSet;
	XSleep(15);
	return 0;
}
int AnimationStatus = 0;
int EaseInAnimationTest(HWND hWnd, int TopMove = 0)
{
	RECT rc;
	GetWindowRect(hWnd, &rc);
	RECT crc; GetClientRect(hWnd, &crc);
	if (TopMove != 0)
	{
		rc.top = TopMove;
	}
	Alpha = 0; AlphaSet;
	SetWindowPos(hWnd, HWND_TOPMOST, rc.left, rc.top + 600, 0, 0, SWP_NOSIZE);
	InvalidateRect(hWnd, &crc, 0);
	XSleep(15);
	ShowWindow(hWnd, SW_SHOW);
	Alpha = 20; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 550, 0, 0, SWP_NOSIZE);
	XSleep(15);
	if (AnimationStatus != 1)
	{
		Alpha = 40; AlphaSet;
		SetWindowPos(hWnd, 0, rc.left, rc.top + 500, 0, 0, SWP_NOSIZE);
		XSleep(15);
		Alpha = 60; AlphaSet;
		SetWindowPos(hWnd, 0, rc.left, rc.top + 450, 0, 0, SWP_NOSIZE);
		XSleep(15);
		Alpha = 80; AlphaSet;
		SetWindowPos(hWnd, 0, rc.left, rc.top + 400, 0, 0, SWP_NOSIZE);
		XSleep(15);
		Alpha = 100; AlphaSet;
		SetWindowPos(hWnd, 0, rc.left, rc.top + 350, 0, 0, SWP_NOSIZE);
		XSleep(15);
		Alpha = 120; AlphaSet;
		SetWindowPos(hWnd, 0, rc.left, rc.top + 300, 0, 0, SWP_NOSIZE);
		XSleep(15);
		Alpha = 140; AlphaSet;
		SetWindowPos(hWnd, 0, rc.left, rc.top + 250, 0, 0, SWP_NOSIZE);
		XSleep(15);
		Alpha = 160; AlphaSet;
		SetWindowPos(hWnd, 0, rc.left, rc.top + 210, 0, 0, SWP_NOSIZE);
		XSleep(15);
		Alpha = 170; AlphaSet;
		SetWindowPos(hWnd, 0, rc.left, rc.top + 170, 0, 0, SWP_NOSIZE);
		XSleep(15);
		Alpha = 180; AlphaSet;
		SetWindowPos(hWnd, 0, rc.left, rc.top + 130, 0, 0, SWP_NOSIZE);
		XSleep(15);
		Alpha = 190; AlphaSet;
		SetWindowPos(hWnd, 0, rc.left, rc.top + 100, 0, 0, SWP_NOSIZE);
		XSleep(15);
		Alpha = 200; AlphaSet;
		SetWindowPos(hWnd, 0, rc.left, rc.top + 70, 0, 0, SWP_NOSIZE);
		XSleep(15);
		Alpha = 210; AlphaSet;
		SetWindowPos(hWnd, 0, rc.left, rc.top + 50, 0, 0, SWP_NOSIZE);
		XSleep(15);
	}
	Alpha = 220; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 32, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 225; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 20, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 230; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 18, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 235; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 10, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 240; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 6, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 245; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 4, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 250; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 2, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 255; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 0, 0, 0, SWP_NOSIZE);
	return 0;
}
int WindowEaseIn(HWND hWnd, int TopMove = 0)
{
	RECT rc;
	GetWindowRect(hWnd, &rc);
	RECT crc; GetClientRect(hWnd, &crc);
	if (TopMove != 0)
	{
		rc.top = TopMove;
	}
	Alpha = 0; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 130, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 10; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 100, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 30; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 70, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 60; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 50, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 90; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 32, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 140; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 20, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 190; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 18, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 225; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 10, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 235; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 6, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 245; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 4, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 250; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 2, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 255; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 0, 0, 0, SWP_NOSIZE);
	return 0;
}
int WindowEaseIn2(HWND hWnd, int TopMove = 0)
{
	RECT rc;
	GetWindowRect(hWnd, &rc);
	RECT crc; GetClientRect(hWnd, &crc);
	if (TopMove != 0)
	{
		rc.top = TopMove;
	}
	Alpha = 0; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top -130, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 60; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top - 50, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 90; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top - 32, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 140; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top - 20, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 190; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top - 18, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 225; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top - 10, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 235; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top - 6, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 245; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top - 4, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 250; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top - 2, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 255; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top - 0, 0, 0, SWP_NOSIZE);
	return 0;
}

int WindowEaseOut(HWND hWnd, int TopMove = 0)
{
	RECT rc;
	GetWindowRect(hWnd, &rc);
	RECT crc; GetClientRect(hWnd, &crc);
	if (TopMove != 0)
	{
		rc.top = TopMove;
	}
	Alpha = 240; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top +0, 0, 0, SWP_NOSIZE);
	XSleep(10);
	Alpha = 210; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 2, 0, 0, SWP_NOSIZE);
	XSleep(10);
	Alpha = 150; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 6, 0, 0, SWP_NOSIZE);
	XSleep(10);
	Alpha = 130; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 10, 0, 0, SWP_NOSIZE);
	XSleep(10);
	Alpha = 110; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 15, 0, 0, SWP_NOSIZE);
	XSleep(10);
	Alpha = 90; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 25, 0, 0, SWP_NOSIZE);
	XSleep(10);
	Alpha = 60; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 36, 0, 0, SWP_NOSIZE);
	XSleep(10);
	Alpha = 40; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 48, 0, 0, SWP_NOSIZE);
	XSleep(10);
	Alpha = 20; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 69, 0, 0, SWP_NOSIZE);
	XSleep(10);
	Alpha = 15; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 91, 0, 0, SWP_NOSIZE);
	return 0;
}
int EaseOutAnimationTest(HWND hWnd)
{
	RECT rc;
	GetWindowRect(hWnd, &rc);
	XSleep(15);
	Alpha = 255; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 2, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 250; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 5, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 230; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 9, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 210; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 16, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 190; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 26, 0, 0, SWP_NOSIZE);
	XSleep(15);
	Alpha = 170; AlphaSet;
	SetWindowPos(hWnd, 0, rc.left, rc.top + 50, 0, 0, SWP_NOSIZE);
	XSleep(15);
	if (AnimationStatus != 1)
	{
		Alpha = 150; AlphaSet;
		SetWindowPos(hWnd, 0, rc.left, rc.top + 50, 0, 0, SWP_NOSIZE);
		XSleep(15);
		Alpha = 80; AlphaSet;
		SetWindowPos(hWnd, 0, rc.left, rc.top + 150, 0, 0, SWP_NOSIZE);
		XSleep(15);
		Alpha = 60; AlphaSet;
		SetWindowPos(hWnd, 0, rc.left, rc.top + 150, 0, 0, SWP_NOSIZE);
		XSleep(15);
		Alpha = 40; AlphaSet;
		SetWindowPos(hWnd, 0, rc.left, rc.top + 200, 0, 0, SWP_NOSIZE);
		XSleep(15);
		Alpha = 20; AlphaSet;
		SetWindowPos(hWnd, 0, rc.left, rc.top + 250, 0, 0, SWP_NOSIZE);
		XSleep(15);
		Alpha = 10; AlphaSet;
		SetWindowPos(hWnd, 0, rc.left, rc.top + 300, 0, 0, SWP_NOSIZE);
		XSleep(15);
		Alpha = 0; AlphaSet;
	}
	return 0;
}

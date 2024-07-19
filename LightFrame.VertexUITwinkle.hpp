#pragma once

#include "LightFrame.Data.h"
#include "VertexUI/VertexUI.Panel.h"

#include "VertexUI/vui.parser/vui.parser.hpp"
#include "Vui.Foundation/Encoding.h"
using namespace VertexUI;


int BoxEdgeValueCvt(VertexUIPos vp, int val, int wh, bool Absolute = false)/*0:宽度 1:高度*/
{
	int n = val;
	if (val < 0)
	{
		if (wh == 0)n = vp.cx + val + 1;
		if (wh == 1)n = vp.cy + val + 1;
	}
	return n;
}
int ClientEdgeValueCvt(HWND h, int val, int wh)/*0:宽度 1:高度*/
{
	RECT rc;
	GetClientRect(h, &rc);
	int n = BoxEdgeValueCvt({ 0,0,rc.right,rc.bottom }, val, wh);
	return n;
}
int ClientEdgeValueCvt(HWND hWnd, VertexUIPos vp, int val, int wh, bool UseSize = false)/*0:宽度 1:高度*/
{
	int n;
	if (UseSize == false)n = ClientEdgeValueCvt(hWnd, val, wh);
	if (UseSize == true)n = BoxEdgeValueCvt(vp, val, wh);
	return n;
}
const wchar_t* ReadVui(const wchar_t* file)
{
	std::string nfile;
	std::ifstream is;
	is.open(file, std::ios::in);
	std::string str; while (std::getline(is, str))
	{

		//状态表达:
		//0:当前正常读取
		//1:匹配到Name相同的部分,添加缓存并收尾后设置为0 QwQ
		//修改status的状态标志
		nfile.append(str);
		nfile.push_back('\n');
	}
	is.close();
	const wchar_t* n = vui::Encoding::Utf8().GetString(nfile.c_str());
	return n;
}

template<typename pT = vui::parser::wparser>
void VertexUITwinkleDirectDraw(HWND hWnd, HDC hdc, const wchar_t* vui, VertexUIPos vp = { 0,0,0,0 }, bool UseSize = false) noexcept
{
	auto obj = vui::parser::wparser(vui);
	VertexUITwinklePreDrawTest(hWnd, hdc, obj, 0, vp, UseSize);
	return;
}
void VertexUITwinkleDrawTest(HWND hWnd, HDC hdc, const wchar_t* file, VertexUIPos vp = { 0,0,0,0 }, bool UseSize = false) noexcept
{
	//MessageBox(0, (const wchar_t*)vui, 0,true, 0,true);
	const wchar_t* n = ReadVui(file);
	VertexUITwinkleDirectDraw(hWnd, hdc, n, vp, UseSize);
}

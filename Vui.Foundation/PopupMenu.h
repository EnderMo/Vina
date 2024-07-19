#pragma once
#include <functional>
#include <vector>
#include "AntiAliasing.h"
#include "Window.h"
#include "Theme.h"
namespace vui
{
    class PopupMenuItem
    {
    public:
        virtual void setspacing(int spacing) = 0;
        virtual SIZE calcsize() = 0;
        virtual void paint(DrawingContext &hdc, SIZE size) = 0;
        virtual void click(POINT pt, int up) = 0;
        virtual void hover(int flag) = 0;
    };
    class PopupMenuItemButton : public PopupMenuItem
    {
    public:
        PopupMenuItemButton(const wchar_t *txt, std::function<void(POINT, int)> clk)
        {
            text = txt;
            clickhandler = clk;
        }
        const wchar_t *text = L"nznb";
        int size = 18;
        int spacing = 0;
        std::function<void(POINT, int)> clickhandler;
        virtual void setspacing(int spacing)
        {
            this->spacing = spacing;
        };
        virtual SIZE calcsize()
        {
            RECT rt = DrawingContext().MeasureString(std::wstring(text), size);
            return SIZE{rt.right - rt.left + spacing * 2, rt.bottom - rt.top + spacing * 2};
        }
        virtual void paint(DrawingContext &hdc, SIZE size)
        {
            hdc.DrawString(RECT{spacing, spacing, size.cx, size.cy}, std::wstring(text), this->size);
        }
        virtual void click(POINT pt, int up)
        {
            clickhandler(pt, up);
        }
        virtual void hover(int flag)
        {
        }
    };
    class PopupMenu : public Window
    {
    public:
        PopupMenu(std::initializer_list<PopupMenuItem *> items, SIZE framert = SIZE{0}, POINT framert2 = POINT{0})
        {
            MenuItems = items;
            framerpt = framert2;
            if (framert.cx == 0)
            {
                framert.cx = GetSystemMetrics(SM_CXSCREEN);
            }
            if (framert.cy == 0)
            {
                framert.cy = GetSystemMetrics(SM_CYSCREEN);
            }
            framesz = framert;
        }
        ~PopupMenu()
        {
            for (auto item : MenuItems)
            {
                delete item;
            }
        }
        POINT framerpt;
        SIZE framesz;
        bool tme_overed = false;
        int spacing = 5;
        std::vector<PopupMenuItem *> MenuItems;
        PopupMenuItem *highlightitem = nullptr;
        virtual LPARAM WndProc(HWND h, UINT msg, WPARAM wp, LPARAM lp) override
        {
            switch (msg)
            {
            case WM_NCHITTEST:
            {
                return HTCLIENT;
            }
            case WM_CREATE:
            {
                SetWindowLongW(h, GWL_EXSTYLE, WS_EX_TOOLWINDOW | WS_EX_LAYERED);
                SetWindowLongW(h, GWL_STYLE, WS_OVERLAPPEDWINDOW & ~WS_CAPTION & ~WS_SYSMENU & ~WS_SIZEBOX);
                SetLayeredWindowAttributes(h, RGB(44, 66, 14), 0, LWA_COLORKEY);
                SIZE totalsize{0};
                totalsize.cy += spacing;
                for (auto item : MenuItems)
                {
                    item->setspacing(spacing);
                    SIZE itemsz = item->calcsize();
                    totalsize.cx = max(totalsize.cx, itemsz.cx);
                    totalsize.cy += itemsz.cy + spacing;
                }
                totalsize.cy += spacing;
                totalsize.cx += spacing * 2;
                POINT pt;
                GetCursorPos(&pt);
                POINT nPt = POINT{pt.x - 5, pt.y - 5};
                if (nPt.x + totalsize.cx - framerpt.x > framesz.cx) // 边缘弹窗优化
                {
                    nPt.x = pt.x - totalsize.cx + 5;
                }
                if (nPt.y + totalsize.cy - framerpt.y > framesz.cy)
                {
                    nPt.y = pt.y - totalsize.cy + 5;
                }
                Location = nPt;
                Size = totalsize;
                SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
                break;
            }
            case WM_MOUSEMOVE:
            {
                if (!tme_overed)
                {
                    TRACKMOUSEEVENT tme;
                    tme.cbSize = sizeof(tme);
                    tme.dwFlags = TME_LEAVE;
                    tme.dwHoverTime = 250;
                    tme.hwndTrack = h;
                    TrackMouseEvent(&tme);
                    tme_overed = true;
                }
                else
                {
                    POINT pt;
                    pt.x = LOWORD(lp);
                    pt.y = HIWORD(lp);

                    SIZE totalsize{0};
                    totalsize.cy += spacing;
                    for (auto item : MenuItems)
                    {
                        SIZE itemsz = item->calcsize();
                        if (pt.x >= spacing && pt.y >= totalsize.cy && pt.x <= Size.cx - spacing && pt.y <= totalsize.cy + itemsz.cy)
                        {
                            highlightitem = item;
                            RECT rt;
                            GetClientRect(h, &rt);
                            InvalidateRect(h, &rt, false);
                            return 0;
                        }
                        totalsize.cy += itemsz.cy;
                        totalsize.cy += spacing;
                    }
                    highlightitem = nullptr;
                    RECT rt;
                    GetClientRect(h, &rt);
                    InvalidateRect(h, &rt, false);
                    return 0;
                }
                break;
            }
            case WM_MOUSELEAVE:
            {
                tme_overed = false;
                RECT rt;
                GetWindowRect(h, &rt);
                POINT pt;
                GetCursorPos(&pt);
                if (!(pt.x > rt.left && pt.y > rt.top && pt.x < rt.right && pt.y < rt.bottom))
                {
                    PostQuitMessage(0);
                }
                break;
            }
            case WM_LBUTTONDOWN:
            {
                POINT pt;
                pt.x = LOWORD(lp);
                pt.y = HIWORD(lp);

                SIZE totalsize{0};
                totalsize.cy += spacing;
                for (auto item : MenuItems)
                {
                    SIZE itemsz = item->calcsize();
                    if (pt.x >= spacing && pt.y >= totalsize.cy && pt.x <= Size.cx - spacing && pt.y <= totalsize.cy + itemsz.cy)
                    {
                        ShowWindow(h, SW_HIDE);
                        item->click(POINT{pt.x - totalsize.cx, pt.y - totalsize.cy}, 0);
                        break;
                    }
                    totalsize.cy += itemsz.cy;
                    totalsize.cy += spacing;
                }
                PostQuitMessage(0);
                break;
            }
            case WM_PAINT:
            {
                DrawingContext dc{this};
                DrawingContext bdc{Size};
                bdc.Clear(RECT{0, 0, Size.cx, Size.cy}, vui::SolidBrush(RGB(44, 66, 14)));
                bdc.DrawRoundRect(RECT{0, 0, Size.cx, Size.cy}, SIZE{spacing , spacing }, vui::SolidBrush(VERTEXUICOLOR_MIDNIGHT), vui::SolidPen(0, 1));
                SIZE totalsize{0};
                totalsize.cy += spacing;
                for (auto item : MenuItems)
                {
                    SIZE itemsz = item->calcsize();
                    DrawingContext memdc{itemsz};
                    if (item == highlightitem)
                    {
                        const int scale = 4;
                        DrawingContext rmemdc{SIZE{(Size.cx - spacing * 2) * scale, itemsz.cy * scale}};
                        rmemdc.CopyFrom(bdc, POINT{0}, POINT{spacing, totalsize.cy}, SIZE{(Size.cx - spacing * 2) * scale, itemsz.cy * scale}, SIZE{(Size.cx - spacing * 2), itemsz.cy});
                        COLORREF clr = RGB(vuicolor.bg_d2_r,vuicolor.bg_d2_g,vuicolor.bg_d2_b);
                        rmemdc.DrawRoundRect(RECT{0, 0, (Size.cx - spacing * 2) * scale, itemsz.cy * scale}, SIZE{spacing * scale * 2, spacing * scale * 2}, vui::SolidBrush(clr));
                        bdc-- > CopyFromUpscale(POINT{spacing, totalsize.cy}, SIZE{(Size.cx - spacing * 2), itemsz.cy}, rmemdc, POINT{0}, scale);
                        // AverageBlt(bdc, POINT{spacing, totalsize.cy}, SIZE{(Size.cx - spacing * 2), itemsz.cy}, rmemdc, POINT{0, 0}, scale);
                    }
                    memdc.CopyFrom(bdc, POINT{}, POINT{totalsize.cx + spacing, totalsize.cy}, itemsz);
                    item->paint(memdc, itemsz);
                    bdc.CopyFrom(memdc, POINT{totalsize.cx + spacing, totalsize.cy}, POINT{}, itemsz);
                    totalsize.cy += itemsz.cy;
                    totalsize.cy += spacing;
                }
                dc.CopyFrom(bdc, POINT{}, POINT{}, Size);
                break;
            }
            }
            return Window::WndProc(h, msg, wp, lp);
        }
    };
}
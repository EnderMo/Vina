#pragma once
#include <windows.h>
#include <iostream>

#define PtInRt(pt, rect) (pt.x > rect.left && pt.x < rect.right && pt.y > rect.top && pt.y < rect.bottom)

namespace vui
{
    class RectConvertor
    {
    public:
        int x;
        int y;
        int cx;
        int cy;
        operator RECT()
        {
            return RECT{x, y, x + cx, y + cy};
        }
    };
    using VertexUIPos = RectConvertor;
    class DebugOutputStream
    {
    public:
        template <class any, class overload_if = std::enable_if<!std::is_pointer<any>::value>::type>
        DebugOutputStream &operator<<(const any a)
        {
            OutputDebugStringW(std::to_wstring(a).c_str());
            return *this;
        }
        DebugOutputStream &operator<<(const wchar_t *a)
        {
            OutputDebugStringW(a);
            return *this;
        }
        DebugOutputStream &operator<<(const char *a)
        {
            OutputDebugStringA(a);
            return *this;
        }
    };
    constexpr DebugOutputStream &dout = *((DebugOutputStream*)0);

}

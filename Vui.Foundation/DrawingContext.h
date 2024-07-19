#pragma once
#include "Window.h"
#include "Theme.h"
#include <string>
namespace vui
{
    class Brush
    {
    public:
        virtual HBRUSH brush() = 0;
    };
    class Pen
    {
    public:
        virtual HPEN pen() = 0;
    };
    class NullPen : public Pen
    {
    public:
        virtual HPEN pen() override
        {
            return (HPEN)GetStockObject(NULL_PEN);
        }
    };
    class NullBrush : public Brush
    {
    public:
        virtual HBRUSH brush() override
        {
            return (HBRUSH)GetStockObject(NULL_BRUSH);
        }
    };
    class SolidPen : public Pen
    {
        HPEN handle;

    public:
        SolidPen(COLORREF cl, int width)
        {
            handle = CreatePen(PS_SOLID, width, cl);
        }
        HPEN pen() override
        {
            return handle;
        }
        ~SolidPen()
        {
            DeleteObject(handle);
        }
    };
    class SolidBrush : public Brush
    {
        HBRUSH handle;

    public:
        SolidBrush(COLORREF cl)
        {
            handle = CreateSolidBrush(cl);
        }
        HBRUSH brush() override
        {
            return handle;
        }
        ~SolidBrush()
        {
            DeleteObject(handle);
        }
    };
    class Bitmap
    {
        HBITMAP bmp;
        unsigned char *scan0 = 0;
        BITMAP obj()
        {
            BITMAP obmp{0};
            GetObject(bmp, sizeof(obmp), &obmp);
            return obmp;
        }
        Bitmap(const Bitmap &rhs) = delete;
        Bitmap &operator=(const Bitmap &rhs) = delete;

    public:
        Bitmap(HBITMAP hbmp)
        {
            bmp = hbmp;
        }
        Bitmap(SIZE sz)
        {
            HDC ndc = GetDC(0);
            bmp = CreateCompatibleBitmap(ndc, sz.cx, sz.cy);
            DeleteObject(ndc);
        }
        ~Bitmap()
        {
            if (!scan0)
            {
                delete scan0;
                scan0 = 0;
            }
            DeleteObject(bmp);
        }
        SIZE size()
        {
            return SIZE{obj().bmWidth, obj().bmHeight};
        }
        long bytesize()
        {
            return obj().bmWidthBytes * obj().bmHeight;
        }
        int bpp()
        {
            return obj().bmBitsPixel;
        }
        unsigned char *lock()
        {
            if (scan0)
                throw;
            scan0 = new unsigned char[bytesize()];
            GetBitmapBits(bmp, bytesize(), scan0);
            return scan0;
        }
        void unlock()
        {
            if (scan0)
            {
                SetBitmapBits(bmp, bytesize(), scan0);
                delete scan0;
                scan0 = 0;
            }
        }
    };
    class DrawingContext
    {
        HDC rhdc = 0;
        HBITMAP rbmp = 0;
        Bitmap *ownbmp = 0;
        DrawingContext(const DrawingContext &rhs) = delete;
        DrawingContext &operator=(const DrawingContext &rhs) = delete;

    public:
        class DrawingContextExtension
        {
        public:
            virtual bool supportdc() { return true; }
            virtual void operator()(DrawingContext &dc) = 0;
            virtual void operator()(HDC hdc) { return; }
        };
        DrawingContext(vui::Window &win)
        {
            rhdc = GetDC(win.handle());
        }
        DrawingContext(vui::Window *win)
        {
            rhdc = GetDC(win->handle());
        }
        DrawingContext(SIZE sz)
        {
            HDC tmp = GetDC(0);
            rhdc = CreateCompatibleDC(tmp);
            rbmp = CreateCompatibleBitmap(tmp, sz.cx, sz.cy);
            SelectObject(rhdc, rbmp);
            DeleteDC(tmp);
        }
        DrawingContext()
        {
            rhdc = GetDC(NULL);
        }
        ~DrawingContext()
        {
            if (rhdc != 0)
                DeleteObject(rhdc);
            if (rbmp != 0)
                DeleteObject(rbmp);
            if (ownbmp != 0)
                DeleteObject(ownbmp);
        }
        DrawingContext &operator--(int)
        {
            return *this;
        }
        DrawingContext &operator>(DrawingContextExtension &ext)
        {
            if (ext.supportdc())
                ext(*this);
            else
                ext(rhdc);
            return *this;
        }
        void CopyFrom(DrawingContext &src, POINT p, POINT pSrc, SIZE s)
        {
            BitBlt(rhdc, p.x, p.y, s.cx, s.cy, src.rhdc, pSrc.x, pSrc.y, SRCCOPY);
        }
        void LogicCopy(DrawingContext &src, POINT p, POINT pSrc, SIZE s, int rop)
        {
            BitBlt(rhdc, p.x, p.y, s.cx, s.cy, src.rhdc, pSrc.x, pSrc.y, rop);
        }
        void CopyFrom(DrawingContext &src, POINT p, POINT pSrc, SIZE s, SIZE sSrc)
        {
            StretchBlt(rhdc, p.x, p.y, s.cx, s.cy, src.rhdc, pSrc.x, pSrc.y, sSrc.cx, sSrc.cy, SRCCOPY);
        }
        void Blend(DrawingContext &src, POINT p, POINT pSrc, SIZE s, int aSrc)
        {
            BLENDFUNCTION bf{0};
            bf.BlendOp = AC_SRC_OVER;
            bf.SourceConstantAlpha = aSrc;
            AlphaBlend(rhdc, p.x, p.y, s.cx, s.cy, src.rhdc, pSrc.x, pSrc.y, s.cx, s.cy, bf);
        }
        void ColorKey(DrawingContext &src, POINT p, POINT pSrc, SIZE s, int colorkey)
        {
            TransparentBlt(rhdc, p.x, p.y, s.cx, s.cy, src.rhdc, pSrc.x, pSrc.y, s.cx, s.cy, colorkey);
        }
        void Clear(RECT r, Brush &b = NullBrush())
        {
            FillRect(rhdc, &r, b.brush());
        }
        void BeginPath()
        {
            ::BeginPath(rhdc);
        }
        void MoveTo(POINT pt)
        {
            MoveToEx(rhdc, pt.x, pt.y, 0);
        }
        void LineTo(POINT pt)
        {
            ::LineTo(rhdc, pt.x, pt.y);
        }
        void EndPath()
        {
            ::EndPath(rhdc);
        }
        void DrawPath()
        {
            StrokeAndFillPath(rhdc);
        }
        void DrawRect(RECT r, Brush &b = NullBrush(), Pen &p = NullPen())
        {
            SelectObject(rhdc, b.brush());
            SelectObject(rhdc, p.pen());
            Rectangle(rhdc, r.left, r.top, r.right, r.bottom);
        }
        void DrawRoundRect(RECT r, SIZE rs, Brush &b = NullBrush(), Pen &p = NullPen())
        {
            SelectObject(rhdc, p.pen());
            SelectObject(rhdc, b.brush());
            RoundRect(rhdc, r.left, r.top, r.right, r.bottom, rs.cx, rs.cy);
        }
        Bitmap &GetBitmap()
        {
            if (!ownbmp)
                ownbmp = new Bitmap((HBITMAP)GetCurrentObject(rhdc, OBJ_BITMAP));
            return *ownbmp;
        }
        void DrawString(RECT r, std::wstring &str, int fs = 12, std::wstring &font = std::wstring(L"Segoe UI"), COLORREF cl = VERTEXUICOLOR_WHITE)
        {
            LOGFONT lf{0};
            lf.lfHeight = -fs;
            // lf.lfQuality = ANTIALIASED_QUALITY;
            wcscpy_s(lf.lfFaceName, font.c_str());
            HFONT hFont = CreateFontIndirect(&lf);
            SelectObject(rhdc, hFont);
            SetTextColor(rhdc, cl);
            SetBkMode(rhdc, TRANSPARENT);
            DrawText(rhdc, str.c_str(), static_cast<int>(str.size()), &r, 0);
            DeleteObject(hFont);
        }
        RECT MeasureString(std::wstring &str, int fs = 12, std::wstring &font = std::wstring(L"Segoe UI"))
        {
            HDC hdc = GetDC(NULL);
            RECT rc{0};
            LOGFONT lf{0};
            lf.lfHeight = -fs;
            // lf.lfQuality = ANTIALIASED_QUALITY;
            wcscpy_s(lf.lfFaceName, font.c_str());
            HFONT hFont = CreateFontIndirect(&lf);
            SelectObject(hdc, hFont);
            DrawText(hdc, str.c_str(), static_cast<int>(str.size()), &rc, DT_CALCRECT);
            DeleteObject(hFont);
            DeleteDC(hdc);
            return rc;
        }
        HDC handle()
        {
            return rhdc;
        }
    };
}
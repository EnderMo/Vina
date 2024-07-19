#pragma once
#include <windows.h>
#include "../DrawingContext.h"
#include <vector>

#define _USE_MATH_DEFINES
#if defined(_USE_MATH_DEFINES) && !defined(_MATH_DEFINES_DEFINED)
#define _MATH_DEFINES_DEFINED

#define M_E 2.71828182845904523536
#define M_LOG2E 1.44269504088896340736
#define M_LOG10E 0.434294481903251827651
#define M_LN2 0.693147180559945309417
#define M_LN10 2.30258509299404568402
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.785398163397448309616
#define M_1_PI 0.318309886183790671538
#define M_2_PI 0.636619772367581343076
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2 1.41421356237309504880
#define M_SQRT1_2 0.707106781186547524401

#endif /* _USE_MATH_DEFINES */

namespace vui
{
    // An strange (SVG-like) path using Gdi
    class Path
    {
    public:
        char MoveToToken = 'M';
        struct MoveTo
        {
            double x;
            double y;
        };
        char LineToToken = 'L';
        struct LineTo
        {
            double x;
            double y;
        };
        char HorizontalLineToken = 'H';
        struct HorizontalLine
        {
            double x;
        };
        char VerticalLineToken = 'V';
        struct VerticalLine
        {
            double y;
        };
        char ClosePathToken = 'Z';
        struct ClosePath
        {
        };
        char CubicCurveToken = 'C';
        struct CubicCurve
        {
            double x1;
            double y1;
            double x2;
            double y2;
            double x;
            double y;
        };
        /*char ShortCubicCurveToken = 'S';
        struct ShortCubicCurve
        {
            double x2;
            double y2;
            double x;
            double y;
        };*/
        char QuadraticCurveToken = 'Q';
        struct QuadraticCurve
        {
            double x1;
            double y1;
            double x;
            double y;
        };
        /*char ShortQuadraticCurveToken = 'T';
        struct ShortQuadraticCurve
        {
            double x;
            double y;
        };*/
        char ArcToken = 'A';
        struct Arc
        {
            double rx;
            double ry;
            double x_axis_rotation;
            bool large_arc_flag;
            bool sweep_flag;
            double x;
            double y;
        };

        using DataCommand = std::pair<wchar_t, void *>;

        std::vector<DataCommand> Data;

    private:
        DrawingContext &dc = DrawingContext();
        static inline bool Equal(wchar_t a, wchar_t b)
        {
            return a == b || ((int)a - 'a' + 'A') == (int)b;
        }

        static inline bool Relative(wchar_t a)
        {
            return a >= 'a' && a <= 'z';
        }

    public:
        Path(std::initializer_list<DataCommand> &data)
        {
            Data = std::vector<DataCommand>(data);
        }
        Path(std::wstring &data)
        {
            LoadFromString(data);
        }
        void LoadFromString(std::wstring &data)
        {
            for (size_t i = 0; i < data.size(); i++)
            {
                auto c = data[i];
                if (Equal(c, MoveToToken))
                {
                    auto &d = *(new MoveTo({0}));
                    i++;
                    size_t idx;
                    d.x = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    d.y = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    i--;
                    Data.push_back(DataCommand(c, (void *)&d));
                }
                if (Equal(c, LineToToken))
                {
                    auto &d = *(new LineTo({0}));
                    i++;
                    size_t idx;
                    d.x = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    d.y = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    i--;
                    Data.push_back(DataCommand(c, (void *)&d));
                }
                if (Equal(c, HorizontalLineToken))
                {
                    auto &d = *(new HorizontalLine({0}));
                    i++;
                    size_t idx;
                    d.x = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    i--;
                    Data.push_back(DataCommand(c, (void *)&d));
                }
                if (Equal(c, VerticalLineToken))
                {
                    auto &d = *(new VerticalLine({0}));
                    i++;
                    size_t idx;
                    d.y = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    i--;
                    Data.push_back(DataCommand(c, (void *)&d));
                }
                if (Equal(c, QuadraticCurveToken))
                {
                    auto &d = *(new QuadraticCurve({0}));
                    i++;
                    size_t idx;
                    d.x1 = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    d.y1 = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    d.x = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    d.y = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    i--;
                    Data.push_back(DataCommand(c, (void *)&d));
                }
                if (Equal(c, CubicCurveToken))
                {
                    auto &d = *(new CubicCurve({0}));
                    i++;
                    size_t idx;
                    d.x1 = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    d.y1 = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    d.x2 = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    d.y2 = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    d.x = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    d.y = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    i--;
                    Data.push_back(DataCommand(c, (void *)&d));
                }
                if (Equal(c, ArcToken))
                {
                    auto &d = *(new Arc({0}));
                    i++;
                    size_t idx;
                    d.rx = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    d.ry = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    d.x_axis_rotation = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    while (data[i] != L' ')
                        i++;
                    d.large_arc_flag = data[i] == L'1';
                    while (data[i] != L' ')
                        i++;
                    d.sweep_flag = data[i] == L'1';
                    d.x = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    d.y = std::stod(std::wstring(data.c_str() + i), &idx);
                    i += idx;
                    i--;
                    Data.push_back(DataCommand(c, (void *)&d));
                }
                if (Equal(c, ClosePathToken))
                {
                    Data.push_back(DataCommand(c, 0));
                }
            }
            // GetPath
        }
        void Draw(HDC hdc, Pen &p = SolidPen(0, 3), Brush &b = SolidBrush(RGB(255, 255, 255)), double Scale = 1)
        {
            SetBkMode(hdc, TRANSPARENT);
            SelectObject(hdc, p.pen());
            SelectObject(hdc, b.brush());
            for (auto item : Data)
            {
                wchar_t c = item.first;
                if (Equal(c, MoveToToken))
                {
                    EndPath(hdc);
                    StrokeAndFillPath(hdc);
                    BeginPath(hdc);
                    auto &m = *(MoveTo *)item.second;
                    if (Relative(c))
                    {
                        POINT p;
                        GetCurrentPositionEx(hdc, &p);
                        MoveToEx(hdc, static_cast<int>(p.x + m.x * Scale), static_cast<int>(p.y + m.y * Scale), 0);
                    }
                    else
                    {
                        MoveToEx(hdc, static_cast<int>(m.x * Scale), static_cast<int>(m.y * Scale), 0);
                    }
                }
                if (Equal(c, LineToToken))
                {
                    auto &m = *(LineTo *)item.second;
                    if (Relative(c))
                    {
                        POINT p;
                        GetCurrentPositionEx(hdc, &p);
                        ::LineTo(hdc, static_cast<int>(p.x + m.x * Scale), static_cast<int>(p.y + m.y * Scale));
                    }
                    else
                    {
                        ::LineTo(hdc, static_cast<int>(m.x * Scale), static_cast<int>(m.y * Scale));
                    }
                }
                if (Equal(c, VerticalLineToken))
                {
                    auto &m = *(VerticalLine *)item.second;
                    POINT p;
                    GetCurrentPositionEx(hdc, &p);
                    if (Relative(c))
                    {
                        p.y = static_cast<int>(p.y + m.y * Scale);
                    }
                    else
                    {
                        p.y = static_cast<int>(m.y * Scale);
                    }
                    ::LineTo(hdc, p.x, p.y);
                }
                if (Equal(c, HorizontalLineToken))
                {
                    auto &m = *(HorizontalLine *)item.second;
                    POINT p;
                    GetCurrentPositionEx(hdc, &p);
                    if (Relative(c))
                    {
                        p.x = static_cast<int>(p.x + m.x * Scale);
                    }
                    else
                    {
                        p.x = static_cast<int>(m.x * Scale);
                    }
                    ::LineTo(hdc, p.x, p.y);
                }
                if (Equal(c, CubicCurveToken))
                {
                    auto &m = *(CubicCurve *)item.second;
                    POINT p;
                    GetCurrentPositionEx(hdc, &p);
                    POINT pts[4];
                    pts[0] = p;
                    if (Relative(c))
                    {
                        pts[1] = POINT{LONG(p.x + m.x1 * Scale), LONG(p.y + m.y1 * Scale)};
                        pts[2] = POINT{LONG(p.x + m.x2 * Scale), LONG(p.y + m.y2 * Scale)};
                        pts[3] = POINT{LONG(p.x + m.x * Scale), LONG(p.y + m.y * Scale)};
                    }
                    else
                    {
                        pts[1] = POINT{LONG(m.x1 * Scale), LONG(m.y1 * Scale)};
                        pts[2] = POINT{LONG(m.x2 * Scale), LONG(m.y2 * Scale)};
                        pts[3] = POINT{LONG(m.x * Scale), LONG(m.y * Scale)};
                    }
                    PolyBezierTo(hdc, pts, 4);
                }
                if (Equal(c, QuadraticCurveToken))
                {
                    auto &m = *(QuadraticCurve *)item.second;
                    POINT p;
                    GetCurrentPositionEx(hdc, &p);
                    POINT pts[3];
                    pts[0] = p;
                    if (Relative(c))
                    {
                        pts[1] = POINT{LONG(p.x + m.x1 * Scale), LONG(p.y + m.y1 * Scale)};
                        pts[2] = POINT{LONG(p.x + m.x * Scale), LONG(p.y + m.y * Scale)};
                    }
                    else
                    {
                        pts[1] = POINT{LONG(m.x1 * Scale), LONG(m.y1 * Scale)};
                        pts[2] = POINT{LONG(m.x * Scale), LONG(m.y * Scale)};
                    }
                    PolyBezierTo(hdc, pts, 3);
                }
                if (Equal(c, ArcToken))
                {
                    auto &m = *(Arc *)item.second;
                    POINT p;
                    GetCurrentPositionEx(hdc, &p);
                    DrawArc(hdc, p, m);
                }
                if (Equal(c, ClosePathToken))
                {
                    EndPath(hdc);
                    StrokeAndFillPath(hdc);
                }
            }
            EndPath(hdc);
            StrokeAndFillPath(hdc);
        }

    private:
        static inline void DrawArc(HDC hdc, POINT p, Arc a)
        {
            // From https://github.com/nornagon/flatten-svg/blob/master/src/svg-to-paths.ts
            // MIT
            LONG *cur = &p.x;
            double x = a.x;
            double y = a.y;
            double phi = a.x_axis_rotation;
            bool fA = a.large_arc_flag;
            bool fS = a.sweep_flag;
            double rx_ = a.rx;
            double ry_ = a.ry;
            auto mpx = (cur[0] - x) / 2;
            auto mpy = (cur[1] - y) / 2;
            auto x1_ = cos(phi) * mpx + sin(phi) * mpy;
            auto y1_ = -sin(phi) * mpx + cos(phi) * mpy;
            auto x1_2 = x1_ * x1_;
            auto y1_2 = y1_ * y1_;
            // ... ensure radii are large enough
            auto L = x1_2 / (rx_ * rx_) + y1_2 / (ry_ * ry_);
            auto rx = L <= 1 ? sqrt(L) * rx_ : rx_;
            auto ry = L <= 1 ? sqrt(L) * ry_ : ry_;
            auto rx2 = rx * rx;
            auto ry2 = ry * ry;
            auto factor = (rx2 * ry2 - rx2 * y1_2 - ry2 * x1_2) / (rx2 * y1_2 + ry2 * x1_2);
            if (abs(factor) < 0.0001)
                factor = 0;
            if (factor < 0)
                factor = -factor;
            auto k = (fA == fS ? -1 : 1) * sqrt(factor);
            auto cx_ = k * rx * y1_ / ry;
            auto cy_ = k * -ry * x1_ / rx;
            auto cx = cos(phi) * cx_ - sin(phi) * cy_ + (cur[0] + x) / 2;
            auto cy = sin(phi) * cx_ + cos(phi) * cy_ + (cur[1] + y) / 2;

            auto t1 = ang(1, 0, (x1_ - cx_) / rx, (y1_ - cy_) / ry);
            auto dt_ = fmod(ang((x1_ - cx_) / rx, (y1_ - cy_) / ry, (-x1_ - cx_) / rx, (-y1_ - cy_) / ry), M_PI * 2);
            auto dt = fS == 0 && dt_ > 0 ? dt_ - M_PI * 2 : fS == 1 && dt_ < 0 ? dt_ + M_PI * 2
                                                                               : dt_;

            // now:
            // - (cx, cy) is the center of the ellipse
            // - (rx, ry) is the radius
            // - phi is the angle around the x-axis of the current
            //   coordinate system to the x-axis of the ellipse
            // - t1 is the start angle of the elliptical arc prior to the stretch and rotate operations.
            // - t1+dt is the end angle of the elliptical arc prior to the stretch and rotate operations.

            // parameterization:
            // ( x )  =  ( cos phi   -sin phi ) . ( rx cos t )  +  ( cx )
            // ( y )  =  ( sin phi    cos phi )   ( ry sin t )     ( cy )

            // https://i.imgur.com/JORhNjU.jpg
            // maximum error based on maximum deviation from true arc
            constexpr auto maxError = 0.1;
            auto n = ceil(abs(dt) / acos(1 - 1 / rx));
            auto i = 1;
            auto theta = t1 + dt * i / n;
            auto tx = cos(phi) * rx * cos(theta) - sin(phi) * ry * sin(theta) + cx;
            auto ty = sin(phi) * rx * cos(theta) + cos(phi) * ry * sin(theta) + cy;
            ::MoveToEx(hdc, int(tx), int(ty), 0);
            for (; i <= n; i++)
            {
                theta = t1 + dt * i / n;
                tx = cos(phi) * rx * cos(theta) - sin(phi) * ry * sin(theta) + cx;
                ty = sin(phi) * rx * cos(theta) + cos(phi) * ry * sin(theta) + cy;
                ::LineTo(hdc, int(tx), int(ty));
            }
            //::LineTo(hdc, int(a.x), int(a.y));
        }

        static inline double ang(double ux, double uy, double vx, double vy)
        {
            /*
            (ux*vy - uy*vx < 0 ? -1 : 1) *
              acos((ux*vx+uy*vy) / sqrt(ux*ux+uy*uy)*sqrt(vx*vx+vy*vy))
              */
            // https://github.com/paperjs/paper.js/blob/f5366fb3cb53bc1ea52e9792e2ec2584c0c4f9c1/src/path/Path.js#L2516
            return atan2(ux * vy - uy * vx, ux * vx + uy * vy);
        }
    };

    class DrawPath : public DrawingContext::DrawingContextExtension
    {
    public:
        Path *Data;
        Pen *pPen;
        Brush *pBrush;
        double Scale;
        DrawPath(Path &data, Pen &p = SolidPen(0, 1), Brush &b = SolidBrush(RGB(32, 82, 61)), double scale = 1)
        {
            Data = &data;
            pPen = &p;
            pBrush = &b;
            Scale = scale;
        }
        void operator()(DrawingContext &dc) override
        {
            HDC hdc = dc.handle();
            Data->Draw(hdc, *pPen, *pBrush, Scale);
        }
    };
}

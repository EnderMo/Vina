#pragma once
#include <windows.h>
namespace vui
{
    class VertexUIColorStandard
    {
    public:
        static inline int VuiAOrBOrCMedium(int c, int a, int b, int c1, bool DNC = false)
        {
            if (c > 128)
            {
                if (c > 200)
                    return c1;
                else
                    return a;
            }
            else
                return b;
        }
        static inline int GetMaxValue(int num, int max)
        {
            if (num >= max)
                return max;
            else
                return num;
        }
        static inline int GetMinValue(int num, int min)
        {
            if (num <= min)
                return min;
            else
                return num;
        }

        static inline int SetColorBright(int c, int b, bool DarkModeNotChange = false)
        {
            if (c > 128)
            {
                if (c - b >= 255)
                    return 255;
                else
                    return c - b;
            }
            else if (DarkModeNotChange == false)
                return c + b;
            else
                return c;
        }

        static inline int SetThemedColorBright(int c, int b, int Themed, bool DarkModeNotChange = false)
        {
            if (c > 128)
            {
                return Themed;
            }
            else if (DarkModeNotChange == false)
                return c + b;
            else
                return c;
        }

        static inline int VuiBlackOrWhiteOpposite(int c)
        {
            if (c > 128)
                return 20;
            else
                return 244;
        }

        static inline int VuiBlackOrWhiteMedium(int c)
        {
            if (c > 128)
                return 128;
            else
                return 15;
        }

        static inline unsigned long VuiMediumChangeToLight(unsigned long Clr, int Val = 30)
        {
            if (((GetRValue(Clr) + GetGValue(Clr) + GetBValue(Clr)) / 3) <= 128)
                return Clr;
            else
                return RGB(GetMaxValue(GetRValue(Clr) + Val, 255), GetMaxValue(GetGValue(Clr) + Val, 255), GetMaxValue(GetBValue(Clr) + Val, 255));
        }
        static inline unsigned long VuiDarkChangeToMedium(unsigned long Clr, int Val = 30)
        {
            if (((GetRValue(Clr) + GetGValue(Clr) + GetBValue(Clr)) / 3) >= 128)
                return Clr;
            return RGB(GetMaxValue(GetRValue(Clr) + Val, 255), GetMaxValue(GetGValue(Clr) + Val, 255), GetMaxValue(GetBValue(Clr) + Val, 255));
        }
        static inline long VuiCalcShadow(long Clr)
        {
            if (((GetRValue(Clr) + GetGValue(Clr) + GetBValue(Clr)) / 3) <= 128)
                return RGB(25, 25, 27);
            else if (((GetRValue(Clr) < GetBValue(Clr) - 8) && GetGValue(Clr) < GetBValue(Clr) - 8))
                return RGB(GetRValue(Clr) - 40, GetGValue(Clr) - 40, GetBValue(Clr) - 20);
            else if (((GetGValue(Clr) < GetRValue(Clr) - 8) && GetBValue(Clr) < GetRValue(Clr) - 8))
                return RGB(GetRValue(Clr) - 20, GetGValue(Clr) - 40, GetBValue(Clr) - 40);
            else
                return RGB(190, 190, 194);
        }
        VertexUIColorStandard()
        {
            bg_d1_r = SetColorBright(bg_r, 10);
            bg_d1_g = SetColorBright(bg_g, 10);
            bg_d1_b = SetColorBright(bg_b, 10);
            bg_d2_r = SetColorBright(bg_r, 30);
            bg_d2_g = SetColorBright(bg_g, 30);
            bg_d2_b = SetColorBright(bg_b, 30);

            if (bg_r <= 128)
            {
                title.bg_r = bg_r;
                title.bg_g = bg_g;
                title.bg_b = bg_b;
            }
            else
            {
                title.bg_r = title.oR;
                title.bg_g = title.oG;
                title.bg_b = title.oB;
            }
            title.bg_d1_r = SetColorBright(title.bg_r, 10);
            title.bg_d1_g = SetColorBright(title.bg_g, 10);
            title.bg_d1_b = SetColorBright(title.bg_b, 10);
            title.bg_d2_r = SetColorBright(title.bg_r, 30);
            title.bg_d2_g = SetColorBright(title.bg_g, 30);
            title.bg_d2_b = SetColorBright(title.bg_b, 30);

            ctl_d1_r = SetColorBright(bg_r, 35);
            ctl_d1_g = SetColorBright(bg_g, 35);
            ctl_d1_b = SetColorBright(bg_b, 35);
            ctl_blur_r = SetColorBright(bg_r, -5);
            ctl_blur_g = SetColorBright(bg_g, -5);
            ctl_blur_b = SetColorBright(bg_b, -5);

            txt_r = VuiBlackOrWhiteOpposite(bg_r);
            txt_g = VuiBlackOrWhiteOpposite(bg_g);
            txt_b = VuiBlackOrWhiteOpposite(bg_b);
        }
        // 42 47 56
        // 38 41 54
        // 241 242 244
        class titlebar
        {
        public:
            // 158 142 250
            int oR = 158;
            int oG = 142;
            int oB = 250;
            int bg_r = 158;
            int bg_g = 142;
            int bg_b = 250;
            int bg_d1_r, bg_d1_g, bg_d1_b = 10;
            int bg_d2_r, bg_d2_g, bg_d2_b = 30;
        };
        titlebar title;
        int bg_r = 38;
        int bg_g = 41;
        int bg_b = 54;
        int bg_d1_r, bg_d1_g, bg_d1_b = 10;
        int bg_d2_r, bg_d2_g, bg_d2_b = 30;

        int ctl_d1_r, ctl_d1_g, ctl_d1_b;
        int ctl_blur_r, ctl_blur_g, ctl_blur_b;

        int txt_r, txt_g, txt_b;
    };
    _declspec(selectany) VertexUIColorStandard vuicolor;
}

#define VERTEXUICOLOR_THEME RGB(228, 212, 255)

#define VERTEXUICOLOR_DARKEN RGB(20, 20, 20)

#define VERTEXUICOLOR_TABDARKEN RGB(57, 62, 71)

#define VERTEXUICOLOR_WHITEDRAW RGB(244, 244, 244)

#define VERTEXUICOLOR_WHITE RGB(vui::vuicolor.txt_r, vui::vuicolor.txt_g, vui::vuicolor.txt_b)

#define VERTEXUICOLOR_SEA RGB(45, 137, 239)

#define VERTEXUICOLOR_DEEPSEA RGB(15, 107, 209)

#define VERTEXUICOLOR_LAVENDER RGB(108, 92, 200)

#define VERTEXUICOLOR_LAVENDER_ RGB(128, 112, 220)

#define VERTEXUICOLOR_BLOOMLAVENDER RGB(158, 142, 250)

#define VERTEXUICOLOR_GREENSEA RGB(26, 188, 156)

#define VERTEXUICOLOR_GREENDEEPSEA RGB(4, 138, 106)

#define VERTEXUICOLOR_REALLYGREENDEEPSEA RGB(68, 102, 103)

#define VERTEXUICOLOR_DARKENX RGB(10, 10, 10)

#define VERTEXUICOLOR_PEACHRED RGB(232, 77, 61)

#define VERTEXUICOLOR_MIDNIGHT RGB(vui::vuicolor.bg_d1_r, vui::vuicolor.bg_d1_g, vui::vuicolor.bg_d1_b)

#define VERTEXUICOLOR_MIDNIGHTPLUS RGB(vui::vuicolor.bg_d2_r, vui::vuicolor.bg_d2_g, vui::vuicolor.bg_d2_b)

#define VERTEXUICOLOR_DARKNIGHT RGB(vui::vuicolor.bg_r, vui::vuicolor.bg_g, vui::vuicolor.bg_b)

#define VERTEXUICOLOR_FOREST RGB(38, 212, 110)

#define VERTEXUICOLOR_DAWN RGB(255, 115, 119)

#define VERTEXUICOLOR_LDARK RGB(42 + 35, 47 + 35, 56 + 35)

#define VERTEXUICOLOR_OTHER_GITHUB RGB(88, 166, 255)

#define VERTEXUI_DARKEN RGB(20, 20, 20)

#define VERTEXUI_WHITE RGB(244, 244, 244)

#define VERTEXUI_GREENSEA RGB(26, 188, 156)

#define VERTEXUICOLOR_SHADOWNIGHT RGB(57, 62, 71)
#pragma once
namespace vui
{
    inline double CalcEaseInOutCurve(double t, double b, double c, double d)
    {
        t += 10;
        if ((t /= d / 2) < 1)
            return c / 2 * t * t * t + b;
        return c / 2 * ((t -= 2) * t * t + 2) + b;
    }
} // namespace vui
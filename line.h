#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include <cmath>
#include <algorithm>
#include "curve.h"
#include "circle.h"

using namespace std;

void ParametricLine(HDC hdc, int xs, int ys, int xe, int ye, COLORREF c)
{
    int dx = xe - xs;
    int dy = ye - ys;
    for (double t = 0; t <= 1; t += 0.001)
    {
        int x = xs + dx * t;
        int y = ys + dy * t;
        SetPixel(hdc, x, y, c);
    }
}

void DDALine(HDC hdc, int xs, int ys, int xe, int ye, COLORREF c)
{
    int dx = xe - xs;
    int dy = ye - ys;
    if (abs(dx) >= abs(dy))
    {
        if (xs > xe)
        {
            swap(xs, xe);
            swap(ys, ye);
        }
        int x = xs;
        double y = ys;
        SetPixel(hdc, xs, ys, c);
        double slope = (double)dy / dx;
        while (x < xe)
        {
            x++;
            y += slope;
            SetPixel(hdc, x, round(y), c);
        }
    }
    else
    {
        if (ys > ye)
        {
            swap(xs, xe);
            swap(ys, ye);
        }
        double x = xs;
        int y = ys;
        SetPixel(hdc, xs, ys, c);
        double slope1 = (double)dx / dy;
        while (y < ye)
        {
            y++;
            x += slope1;
            SetPixel(hdc, round(x), y, c);
        }
    }
}

void MidPointLine(HDC hdc, int xs, int ys, int xe, int ye, COLORREF c)
{
    int dx = xe - xs, dy = ye - ys;
    if ((dx < 0) ^ (dy < 0))
    {
        // tan is negative
        if (abs(dy) > abs(dx))
        {
            if (ys > ye)
            {
                swap(xs, xe);
                swap(ys, ye);
                dx = -dx;
                dy = -dy;
            }
            int d = 2 * dx + dy;
            int x = xs, y = ys;
            while (y < ye)
            {
                SetPixel(hdc, x, y, c);
                if (d < 0)
                {
                    // right to the line
                    d += dx + dy;
                    x--;
                }
                else
                {
                    // left to the line
                    d += dx;
                }
                y++;
            }
        }
        else
        {
            if (xe > xs)
            {
                swap(xs, xe);
                swap(ys, ye);
                dx = -dx;
                dy = -dy;
            }
            int d = dx + 2 * dy;
            int x = xs, y = ys;
            while (x > xe)
            {
                SetPixel(hdc, x, y, c);
                if (d < 0)
                {
                    // above the line
                    d += dy;
                }
                else
                {
                    // under the line
                    d += dx + dy;
                    y++;
                }
                x--;
            }
        }
    }
    else
    {
        // tan is positive
        if (abs(dy) > abs(dx))
        {
            if (ye < ys)
            {
                swap(xs, xe);
                swap(ys, ye);
                dx = -dx;
                dy = -dy;
            }
            int d = 2 * dx - dy;
            int x = xs, y = ys;
            while (y < ye)
            {
                SetPixel(hdc, x, y, c);
                if (d < 0)
                {
                    d += dx; // right to the line
                }
                else
                {
                    d += dx - dy; // left to the line
                    x++;
                }
                y++;
            }
        }
        else
        {
            if (xe < xs)
            {
                swap(xs, xe);
                swap(ys, ye);
                dx = -dx;
                dy = -dy;
            }
            int d = dx - 2 * dy;
            int x = xs, y = ys;
            while (x < xe)
            {
                SetPixel(hdc, x, y, c);
                if (d >= 0)
                {
                    d += -dy; // above the line
                }
                else
                {
                    d += -dy + dx; // under the line
                    y++;
                }
                x++;
            }
        }
    }
}

void QuarterLine(HDC hdc, int xs, int ys, int x, int y, COLORREF c, int quarter)
{
    switch (quarter)
    {
    case 1:
        MidPointLine(hdc, xs, ys, xs + x, ys - y, c);
        swap(x, y);
        MidPointLine(hdc, xs, ys, xs + x, ys - y, c);
        break;
    case 2:
        MidPointLine(hdc, xs, ys, xs + x, ys + y, c);
        swap(x, y);
        MidPointLine(hdc, xs, ys, xs + x, ys + y, c);
        break;
    case 3:
        MidPointLine(hdc, xs, ys, xs - x, ys + y, c);
        swap(x, y);
        MidPointLine(hdc, xs, ys, xs - x, ys + y, c);
        break;
    case 4:
        MidPointLine(hdc, xs, ys, xs - x, ys - y, c);
        swap(x, y);
        MidPointLine(hdc, xs, ys, xs - x, ys - y, c);
        break;
    default:
        break;
    }
}

Point PointInside(int xc1, int yc1, int r1, int xc2, int yc2, int r2)
{
    int dx = xc2 - xc1;
    int dy = yc2 - yc1;
    for (double t = 0; t <= 1; t += 0.0001)
    {
        int x = xc1 + dx * t;
        int y = yc1 + dy * t;
        int inside = InsideCircle(x, y, xc1, yc1, r1) + InsideCircle(x, y, xc2, yc2, r2);
        if (inside == 2)
            return Point(x, y);
    }
    return Point(-1, -1);
}

void DrawLine(HDC hdc, int xs, int ys, int xe, int ye, COLORREF c, int choice)
{
    if (choice == 0)
    {
        ParametricLine(hdc, xs, ys, xe, ye, c);
    }
    else if (choice == 1)
    {
        DDALine(hdc, xs, ys, xe, ye, c);
    }
    else
    {
        MidPointLine(hdc, xs, ys, xe, ye, c);
    }
}

#endif // LINE_H_INCLUDED

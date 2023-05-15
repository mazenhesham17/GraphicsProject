#ifndef FILL_H_INCLUDED
#define FILL_H_INCLUDED

#include "circle.h"
#include "curve.h"
#include "line.h"
#include <stack>

using namespace std;

int dx[] = {+0, +0, +1, -1, +1, -1, +1, -1}, dy[] = {+1, -1, +0, +0, +1, -1, -1, +1};

void FillWithHermite(HDC hdc, int xleft, int ytop, int xright, int ybottom, COLORREF c)
{
    int step = 15;
    int xmid1 = xleft + (xright - xleft) / 3;
    int xmid2 = xright - (xright - xleft) / 3;
    int ymid1 = ybottom + ( ytop - ybottom )/3 ;
    int ymid2 = ytop - ( ytop - ybottom )/3 ;
    for (int i = xleft + step ; i + step < xright; i += step )
    {
        DrawCurve(hdc, Point(i, ybottom), Point( xmid1 , ymid1), Point(i, ytop), Point( xmid2 , ymid2), c, 0);
    }
}

void FillWithBezier(HDC hdc, int xleft, int ytop, int xright, int ybottom, COLORREF c)
{
    int xmid1 = xleft + (xright - xleft) / 3;
    int xmid2 = xright - (xright - xleft) / 3;
    int step = 15;
    for (int i = ybottom + step; i + step < ytop; i += step)
    {
        DrawCurve(hdc, Point(xleft, i), Point(xmid1, i + step), Point(xmid2, i - step), Point(xright, i), c, 1);
    }
}

void FillWithCircles(HDC hdc, int xs, int ys, int R, COLORREF c, int quarter)
{
    int step = 5;
    for (int i = 0; i < R; i += step)
    {
        DrawQuaterCircle(hdc, xs, ys, i, c, quarter);
    }
}

void FillWithLines(HDC hdc, int xs, int ys, int R, COLORREF c, int quarter)
{
    int x = 0, y = R;
    int d = 1 - R;
    int cnt = 0, step = 5;
    while (y > x)
    {
        if (cnt % step == 0)
        {
            QuarterLine(hdc, xs, ys, x, y, c, quarter);
        }
        if (d <= 0)
        {
            d += 2 * x + 3;
        }
        else
        {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
        cnt++;
    }
}

void RecursiveFloodFill(HDC hdc, int x, int y, COLORREF fc, COLORREF bc)
{
    COLORREF cur = GetPixel(hdc, x, y);
    if (cur == bc || cur == fc)
        return;
    SetPixel(hdc, x, y, fc);
    for (int i = 0; i < 4; i++)
    {
        RecursiveFloodFill(hdc, x + dx[i], y + dy[i], fc, bc);
    }
}

void IterativeFloodFill(HDC hdc, int x, int y, COLORREF fc, COLORREF bc)
{
    stack<Point> S;
    S.push(Point(x, y));
    while (!S.empty())
    {
        Point v = S.top();
        S.pop();
        COLORREF c = GetPixel(hdc, v.x, v.y);
        if (c == bc || c == fc)
            continue;
        SetPixel(hdc, v.x, v.y, fc);
        S.push(Point(v.x + 1, v.y));
        S.push(Point(v.x - 1, v.y));
        S.push(Point(v.x, v.y + 1));
        S.push(Point(v.x, v.y - 1));
    }
}

#endif // FILL_H_INCLUDED

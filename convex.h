#ifndef CONVEX_H_INCLUDED
#define CONVEX_H_INCLUDED

#include <vector>
#include "line.h"
#include "curve.h"

using namespace std;

void scanLine(Point s, Point e, vector<Point> &table)
{
    if (s.y == e.y)
        return;
    if (s.y > e.y)
        swap(s, e);
    double x = s.x;
    int y = s.y;
    double dx = 1.0 * (e.x - s.x) / (e.y - s.y);
    while (y < e.y)
    {
        table[y].x = min(table[y].x, (int)ceil(x));
        table[y].y = max(table[y].y, (int)floor(x));
        x += dx, y++;
    }
}

void fillScanLines(HDC hdc, vector<Point> &table, COLORREF c)
{
    for (int y = 0; y < MAX; y++)
    {
        int x = table[y].x;
        while (x <= table[y].y)
        {
            SetPixel(hdc, x, y, c);
            x++;
        }
    }
}

void DrawPolygon(HDC hdc, vector<Point> &points, COLORREF c)
{
    if (points.empty())
        return;
    Point last = points.back();
    for (int i = 0; i < (int)points.size(); i++)
    {
        MidPointLine(hdc, last.x, last.y, points[i].x, points[i].y, c);
        last = points[i];
    }
}

void FillConvexPolygon(HDC hdc, vector<Point> &points, COLORREF c)
{
    if (points.empty())
        return;
    vector<Point> table(MAX);
    for (int i = 0; i < MAX; i++)
    {
        table[i] = Point(MAX, -MAX);
    }
    Point last = points.back();
    for (int i = 0; i < (int)points.size(); i++)
    {
        scanLine(last, points[i], table);
        last = points[i];
    }
    fillScanLines(hdc, table, c);
    points.clear();
}

#endif // CONVEX_H_INCLUDED

#ifndef CURVE_H_INCLUDED
#define CURVE_H_INCLUDED

#include <cmath>
const int MAX = 100000;

struct Point
{
    int x;
    int y;
    Point(int _x = 0, int _y = 0)
    {
        x = _x;
        y = _y;
    }
};

void HermiteCurve(HDC hdc, Point p0, Point s0, Point p1, Point s1, COLORREF c, int n)
{
    double a3 = 2 * p0.x + s0.x - 2 * p1.x + s1.x;
    double b3 = 2 * p0.y + s0.y - 2 * p1.y + s1.y;
    double a2 = -3 * p0.x + -2 * s0.x + 3 * p1.x - s1.x;
    double b2 = -3 * p0.y + -2 * s0.y + 3 * p1.y - s1.y;
    double a1 = s0.x;
    double b1 = s0.y;
    double a0 = p0.x;
    double b0 = p0.y;
    double dt = 1.0 / (n - 1);
    for (double t = 0; t <= 1; t += dt)
    {
        int x = a3 * t * t * t + a2 * t * t + a1 * t + a0 + 0.5;
        int y = b3 * t * t * t + b2 * t * t + b1 * t + b0 + 0.5;
        SetPixel(hdc, x, y, c);
    }
}

void BezierCurve(HDC hdc, Point p0, Point p1, Point p2, Point p3, COLORREF c, int n)
{
    Point s0 = Point(3 * (p1.x - p0.x), 3 * (p1.y - p0.y));
    Point s1 = Point(3 * (p3.x - p2.x), 3 * (p3.y - p2.y));
    HermiteCurve(hdc, p0, s0, p3, s1, c, n);
}

void DrawCardinalSpline(HDC hdc, vector<Point> &points, double t, COLORREF c)
{
    if (points.size() < 3)
        return;
    int n = points.size();
    t = 1 - t;
    Point s0 = Point(t * (points[2].x - points[0].x), t * (points[2].y - points[0].y));
    HermiteCurve(hdc, points[0], s0, points[1], s0, c, 100001);
    for (int i = 2; i < n - 1; i++)
    {
        Point s1 = Point(t * (points[i + 1].x - points[i - 1].x), t * (points[i + 1].y - points[i - 1].y));
        HermiteCurve(hdc, points[i - 1], s0, points[i], s1, c, 100001);
        s0 = s1;
    }
    Point s1 = Point(t * (points[n - 1].x - points[n - 3].x), t * (points[n - 1].y - points[n - 3].y));
    HermiteCurve(hdc, points[n - 2], s0, points[n - 1], s1, c, 100001);
    points.clear();
}

void DrawCurve(HDC hdc, Point p0, Point p1, Point p2, Point p3, COLORREF c, int choice)
{
    if (choice == 0)
    {
        HermiteCurve(hdc, p0, p1, p2, p3, c, 10001);
    }
    else
    {
        BezierCurve(hdc, p0, p1, p2, p3, c, 10001);
    }
}

#endif // CURVE_H_INCLUDED

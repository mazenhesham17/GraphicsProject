#ifndef ELLIPSE_H_INCLUDED
#define ELLIPSE_H_INCLUDED

#include <cmath>
#include <algorithm>

void Draw4Points(HDC hdc, int xs, int ys, int x, int y, COLORREF c)
{
    int dx[] = {1, 1, -1, -1}, dy[] = {-1, 1, -1, 1};
    for (int j = 0; j < 4; j++)
    {
        SetPixel(hdc, xs + dx[j] * x, ys + dy[j] * y, c);
    }
}

void DirectEllipse(HDC hdc, int xs, int ys, int a, int b, COLORREF c)
{
    double x = 0, y = b;
    Draw4Points(hdc, xs, ys, x, y, c);
    while (x * b * b <= y * a * a)
    {
        int xc = round(x);
        int yc = round(y);
        Draw4Points(hdc, xs, ys, xc, yc, c);
        x++;
        y = sqrt((1 - (x * x) / (a * a)) * (b * b));
    }
    x = a, y = 0;
    Draw4Points(hdc, xs, ys, x, y, c);
    while (x * b * b >= y * a * a)
    {
        int xc = round(x);
        int yc = round(y);
        Draw4Points(hdc, xs, ys, xc, yc, c);
        y++;
        x = sqrt((1 - (y * y) / (b * b)) * (a * a));
    }
}

void PolarEllipse(HDC hdc, int xs, int ys, int a, int b, COLORREF c)
{
    double x = a, y = 0;
    double theta = 0, dtheta = 1.0 / max(a, b);
    double dcos = cos(dtheta), dsin = sin(dtheta);
    Draw4Points(hdc, xs, ys, x, y, c);
    while (theta <= PI / 2)
    {
        Draw4Points(hdc, xs, ys, round(x), round(y), c);
        x = x * dcos - a * y / b * dsin;
        y = y * dcos + x * b / a * dsin;
        theta += dtheta;
    }
}

void MidPointEllipse(HDC hdc, int xs, int ys, int a, int b, COLORREF c)
{
    int x = 0, y = b;
    int asquare = a * a, bsquare = b * b;
    int dinit = bsquare - b * asquare + asquare / 4;
    Draw4Points(hdc, xs, ys, x, y, c);
    while (2 * bsquare * x <= 2 * asquare * y)
    {
        Draw4Points(hdc, xs, ys, x, y, c);
        if (dinit <= 0)
        {
            dinit += (2 * x + 1) * bsquare;
        }
        else
        {
            dinit += (2 * x + 1) * bsquare + (-2 * y + 1) * asquare;
            y--;
        }
        x++;
    }
    dinit = (x * x + x) * bsquare + bsquare / 4 + (y * y - 2 * y + 1) * asquare - asquare * bsquare;
    Draw4Points(hdc, xs, ys, x, y, c);
    while (y >= 0)
    {
        Draw4Points(hdc, xs, ys, x, y, c);
        y--;
        if (dinit <= 0)
        {
            x++;
            dinit += (2 * x) * bsquare + (-2 * y + 1) * asquare;
        }
        else
        {
            dinit += (-2 * y + 1) * asquare;
        }
    }
}

void DrawEllipse(HDC hdc, int xs, int ys, int a, int b, COLORREF c, int choice)
{
    if (choice == 0)
    {
        DirectEllipse(hdc, xs, ys, a, b, c);
    }
    else if (choice == 1)
    {
        PolarEllipse(hdc, xs, ys, a, b, c);
    }
    else
    {
        MidPointEllipse(hdc, xs, ys, a, b, c);
    }
}

#endif // ELLIPSE_H_INCLUDED

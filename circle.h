#ifndef CIRCLE_H_INCLUDED
#define CIRCLE_H_INCLUDED

#include <cmath>
#include <algorithm>

using namespace std;

const double PI = 3.14159265359;

void DrawQuarterPoints(HDC hdc, int xs, int ys, int x, int y, COLORREF c, int quarter)
{
    switch (quarter)
    {
    case 1:
        SetPixel(hdc, xs + x, ys - y, c);
        swap(x, y);
        SetPixel(hdc, xs + x, ys - y, c);
        break;
    case 2:
        SetPixel(hdc, xs + x, ys + y, c);
        swap(x, y);
        SetPixel(hdc, xs + x, ys + y, c);
        break;
    case 3:
        SetPixel(hdc, xs - x, ys + y, c);
        swap(x, y);
        SetPixel(hdc, xs - x, ys + y, c);
        break;
    case 4:
        SetPixel(hdc, xs - x, ys - y, c);
        swap(x, y);
        SetPixel(hdc, xs - x, ys - y, c);
        break;
    default:
        break;
    }
}

void Draw8Points(HDC hdc, int xs, int ys, int x, int y, COLORREF c)
{
    int dx[] = {1, 1, -1, -1}, dy[] = {-1, 1, -1, 1};
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            SetPixel(hdc, xs + dx[j] * x, ys + dy[j] * y, c);
        }
        std::swap(x, y);
    }
}

void DirectCircle(HDC hdc, int xs, int ys, int R, COLORREF c)
{
    double x = 0;
    double y = R;
    Draw8Points(hdc, xs, ys, R, 0, c);
    while (x < y)
    {
        x++;
        y = sqrt(R * R - x * x);
        Draw8Points(hdc, xs, ys, x, y, c);
    }
}

void PolarCircle(HDC hdc, int xs, int ys, int R, COLORREF c)
{
    double dtheta = 1.0 / R;
    for (double theta = 0; theta < PI / 4; theta += dtheta)
    {
        double x = R * cos(theta);
        double y = R * sin(theta);
        Draw8Points(hdc, xs, ys, round(x), round(y), c);
    }
}

void IterativePolarCircle(HDC hdc, int xs, int ys, int R, COLORREF c)
{
    double dtheta = 1.0 / R;
    double st = sin(dtheta);
    double ct = cos(dtheta);
    double x = R, y = 0;
    Draw8Points(hdc, xs, ys, x, y, c);
    while (x > y)
    {
        double X = x * ct - y * st;
        y = x * st + y * ct;
        x = X;
        Draw8Points(hdc, xs, ys, round(x), round(y), c);
    }
}

void MidPointCircle(HDC hdc, int xs, int ys, int r, COLORREF c, int quarter)
{
    int dinit = 1 - r;
    int x = 0, y = r;
    while (y > x)
    {
        if (quarter != -1)
        {
            DrawQuarterPoints(hdc, xs, ys, x, y, c, quarter);
        }
        else
        {
            Draw8Points(hdc, xs, ys, x, y, c);
        }
        if (dinit <= 0)
        {
            dinit += 2 * x + 3;
        }
        else
        {
            dinit += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

void ModifiedMidPointCircle(HDC hdc, int xs, int ys, int r, COLORREF c)
{
    int dinit = 1 - r;
    int x = 0, y = r;
    int d1 = 3, d2 = 5 - 2 * r;
    while (y > x)
    {
        Draw8Points(hdc, xs, ys, x, y, c);
        if (dinit <= 0)
        {
            dinit += d1;
            d2 += 2;
        }
        else
        {
            dinit += d2;
            d2 += 4;
            y--;
        }
        x++;
        d1 += 2;
    }
}

int DrawSpecialCircle(HDC hdc, int xs, int ys, int xe, int ye, COLORREF c)
{
    int r = sqrt( ( xe - xs )*( xe - xs ) + ( ye - ys )*( ye - ys ) ) ;
    ModifiedMidPointCircle( hdc,xs,ys,r,c ) ;
    return r ;
}

bool InsideCircle( int x , int y , int xs , int ys , int r ){
    return ( (x - xs)*(x - xs) + (y - ys)*(y - ys) - r*r < 0 ) ;
}

int Intersection( int xc1 , int yc1 , int r1 , int xc2 , int yc2 , int r2  ){
    int r1r2 = sqrt( ( xc2 - xc1 )*( xc2 - xc1 ) + ( yc2 - yc1  )*( yc2 - yc1 )  ) ;
    if ( r1 + r2 < r1r2 ){
        return -1 ;
    }else if ( r1 + r2 == r1r2 ){
        return 0 ;
    }else{
        return 1 ;
    }
}

void DrawQuaterCircle(HDC hdc, int xs, int ys, int R, COLORREF c, int quarter)
{
    MidPointCircle(hdc, xs, ys, R, c, quarter);
}

void DrawCircle(HDC hdc, int xs, int ys, int R, COLORREF c, int choice)
{
    if (choice == 0)
    {
        printf("Direct Circle is drawn\n\n");
        DirectCircle(hdc, xs, ys, R, c);
    }
    else if (choice == 1)
    {
        printf("Polar Circle is drawn\n\n");
        PolarCircle(hdc, xs, ys, R, c);
    }
    else if (choice == 2)
    {
        printf("Iterative Polar Circle is drawn\n\n");
        IterativePolarCircle(hdc, xs, ys, R, c);
    }
    else if (choice == 3)
    {
        printf("Midpoint Circle is drawn\n\n");
        MidPointCircle(hdc, xs, ys, R, c, -1);
    }
    else
    {
        printf("Modified Midpoint Circle is drawn\n\n");
        ModifiedMidPointCircle(hdc, xs, ys, R, c);
    }
}

#endif // CIRCLE_H_INCLUDED

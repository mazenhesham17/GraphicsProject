#ifndef FILL_H_INCLUDED
#define FILL_H_INCLUDED

#include "circle.h"
#include "curve.h"
#include "line.h"

void FillWithHermite(HDC hdc, int xleft, int ytop, int xright, int ybottom, COLORREF c )
{
    int step = 15 ;
    for ( int i = xleft + step ; i + step < xright ; i += step )
    {
        DrawCurve(hdc,Point(i,ytop), Point(i,ybottom), Point(i,ybottom), Point(xright-i/2,ytop), c,0 ) ;
    }
}


void FillWithBezier(HDC hdc, int xleft, int ytop, int xright, int ybottom, COLORREF c )
{
    int xmid1 = xleft + ( xright - xleft )/3 ;
    int xmid2 = xright - ( xright - xleft )/3 ;
    int step = 15 ;
    for ( int i = ybottom + step ; i + step < ytop ; i += step )
    {
        DrawCurve(hdc,Point(xleft,i),Point(xmid1,i+step),Point(xmid2,i-step),Point(xright,i),c,1);
    }
}

void FillWithCircles(HDC hdc, int xs, int ys, int R, COLORREF c , int quarter )
{
    int step = 5 ;
    for ( int i = 0 ; i < R ; i += step )
    {
        DrawQuaterCircle(hdc,xs,ys,i,c,quarter) ;
    }
}

void FillWithLines(HDC hdc, int xs, int ys, int R, COLORREF c , int quarter )
{
    int x = 0, y = R ;
    int d = 1 - R ;
    int cnt = 0 , step = 5 ;
    while ( y > x )
    {
        if ( cnt % step == 0 )
        {
            QuarterLine(hdc,xs,ys,x,y,c,quarter) ;
        }
        if ( d <= 0 )
        {
            d += 2*x + 3 ;
        }
        else
        {
            d += 2*(x-y) + 5 ;
            y-- ;
        }
        x++ ;
        cnt++ ;
    }
}

#endif // FILL_H_INCLUDED

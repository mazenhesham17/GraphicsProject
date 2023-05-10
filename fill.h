#ifndef FILL_H_INCLUDED
#define FILL_H_INCLUDED

#include "curve.h"

void FillWithHermite(HDC hdc , int xleft , int ytop , int xright , int ybottom , COLORREF c ){
    int step = 15 ;
    for ( int i = xleft + step ; i + step < xright ; i += step ){
        DrawCurve(hdc,Point(i,ytop) , Point(i,ybottom), Point(i,ybottom) , Point(xright-i/2,ytop), c ,0 ) ;
    }
}


void FillWithBezier(HDC hdc , int xleft , int ytop , int xright , int ybottom , COLORREF c ){
    int xmid1 = xleft + ( xright - xleft )/3 ;
    int xmid2 = xright - ( xright - xleft )/3 ;
    int step = 15 ;
    for ( int i = ybottom + step ; i + step < ytop ; i += step ){
        DrawCurve(hdc,Point(xleft,i),Point(xmid1,i+step),Point(xmid2,i-step),Point(xright,i),c,1);
    }
}

#endif // FILL_H_INCLUDED

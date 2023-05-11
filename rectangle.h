#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

#include "line.h"

void DrawRectangle(HDC hdc, int xleft, int ytop, int xright, int ybottom, COLORREF c)
{
    DrawLine(hdc, xleft, ytop, xleft, ybottom, c, 4);
    DrawLine(hdc, xright, ytop, xright, ybottom, c, 4);
    DrawLine(hdc, xleft, ytop, xright, ytop, c, 4);
    DrawLine(hdc, xleft, ybottom, xright, ybottom, c, 4);
}

void DrawSquare(HDC hdc, int &xleft, int &ytop, int &xright, int &ybottom, int l, COLORREF c)
{
    int xs = xleft - (l + 1) / 2, xe = xleft + (l + 1) / 2;
    int ys = ytop - (l + 1) / 2, ye = ytop + (l + 1) / 2;
    xleft = xs, ytop = ys;
    xright = xe, ybottom = ye;
    DrawRectangle(hdc, xleft, ytop, xright, ybottom, c);
}

#endif // RECTANGLE_H_INCLUDED

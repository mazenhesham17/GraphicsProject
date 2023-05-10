#ifndef CURVE_H_INCLUDED
#define CURVE_H_INCLUDED



struct Point
{
    int x ;
    int y ;
    Point( int _x, int _y )
    {
        x = _x ;
        y = _y ;
    }
};

void HermiteCurve( HDC hdc, Point p0, Point s0, Point p1, Point s1, COLORREF c )
{
    double a3 = 2*p0.x + s0.x -2*p1.x + s1.x ;
    double b3 = 2*p0.y + s0.y -2*p1.y + s1.y ;
    double a2 = -3*p0.x + -2*s0.x + 3*p1.x - s1.x ;
    double b2 = -3*p0.y + -2*s0.y + 3*p1.y - s1.y ;
    double a1 = s0.x ;
    double b1 = s0.y ;
    double a0 = p0.x ;
    double b0 = p0.y ;
    for ( double t = 0 ; t <= 1 ; t += 0.001 )
    {
        int x = a3*t*t*t + a2*t*t + a1*t + a0 + 0.5 ;
        int y = b3*t*t*t + b2*t*t + b1*t + b0 + 0.5 ;
        SetPixel(hdc,x,y,c) ;
    }
}

void BezierCurve( HDC hdc, Point p0, Point p1, Point p2, Point p3, COLORREF c )
{
    Point s0 = Point(3*(p1.x-p0.x),3*(p1.y-p0.y)) ;
    Point s1 = Point(3*(p3.x-p2.x),3*(p3.y-p2.y)) ;
    HermiteCurve(hdc,p0,s0,p3,s1,c) ;
}

void DrawCurve( HDC hdc, Point p0, Point p1, Point p2, Point p3, COLORREF c, int choice )
{
    if ( choice == 0 )
    {
        HermiteCurve(hdc,p0,p1,p2,p3,c) ;
    }
    else
    {
        BezierCurve(hdc,p0,p1,p2,p3,c) ;
    }
}


#endif // CURVE_H_INCLUDED

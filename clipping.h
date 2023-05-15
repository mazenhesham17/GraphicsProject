#ifndef CLIPPING_H_INCLUDED
#define CLIPPING_H_INCLUDED

bool InsideRectangle(int x, int y, int xleft, int ytop, int xright, int ybottom)
{
    return (x >= xleft && x <= xright && y <= ytop && y >= ybottom);
}

short GetOutCode(double x, double y, int xleft, int ytop, int xright, int ybottom)
{
    short res = 0;
    if (x < xleft)
        res |= 1;
    else if (x > xright)
        res |= 4;
    if (y > ytop)
        res |= 2;
    else if (y < ybottom)
        res |= 8;
    return res;
}

void VIntersect(double xs, double ys, double xe, double ye, int x, double &x1, double &y1)
{
    y1 = ys + (x - xs) * (ye - ys) / (xe - xs);
    x1 = x;
}

void HIntersect(double xs, double ys, double xe, double ye, int y, double &x1, double &y1)
{
    x1 = xs + (y - ys) * (xe - xs) / (ye - ys);
    y1 = y;
}

bool CohenClipping(int &xs, int &ys, int &xe, int &ye, int xleft, int ytop, int xright, int ybottom)
{
    short o1 = GetOutCode(xs, ys, xleft, ytop, xright, ybottom);
    short o2 = GetOutCode(xe, ye, xleft, ytop, xright, ybottom);
    double x1 = xs, y1 = ys, x2 = xe, y2 = ye;
    if ((o1 & o2) != 0)
        return false;
    while (o1 || o2)
    {
        // (xs,ys)
        if (o1 & 1)
        {
            // left
            VIntersect(x1, y1, x2, y2, xleft, x1, y1);
        }
        else if (o1 & 2)
        {
            // top
            HIntersect(x1, y1, x2, y2, ytop, x1, y1);
        }
        else if (o1 & 4)
        {
            // right
            VIntersect(x1, y1, x2, y2, xright, x1, y1);
        }
        else if (o1 & 8)
        {
            // bottom
            HIntersect(x1, y1, x2, y2, ybottom, x1, y1);
        }
        o1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom);
        // (xe,ye)
        if (o2 & 1)
        {
            // left
            VIntersect(x1, y1, x2, y2, xleft, x2, y2);
        }
        else if (o2 & 2)
        {
            // top
            HIntersect(x1, y1, x2, y2, ytop, x2, y2);
        }
        else if (o2 & 4)
        {
            // right
            VIntersect(x1, y1, x2, y2, xright, x2, y2);
        }
        else if (o2 & 8)
        {
            // bottom
            HIntersect(x1, y1, x2, y2, ybottom, x2, y2);
        }
        o2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom);
        if ((o1 & o2) != 0)
            return false;
    }
    xs = round(x1), ys = round(y1);
    xe = round(x2), ye = round(y2);
    return true;
}

#endif // CLIPPING_H_INCLUDED

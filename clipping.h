#ifndef CLIPPING_H_INCLUDED
#define CLIPPING_H_INCLUDED

#include <vector>

using namespace std;

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

bool Inside(Point &v, int edge, int choice)
{
    if (choice == 0)
    {
        // left
        return v.x >= edge;
    }
    else if (choice == 1)
    {
        // top
        return v.y <= edge;
    }
    else if (choice == 2)
    {
        // right
        return v.x <= edge;
    }
    else
    {
        // bottom
        return v.y >= edge;
    }
}

Point Intersetion(int x1, int y1, int x2, int y2, int edge, int choice)
{
    double x, y;
    if (choice & 1)
    {
        HIntersect(x1, y1, x2, y2, edge, x, y);
    }
    else
    {
        VIntersect(x1, y1, x2, y2, edge, x, y);
    }
    return Point(x, y);
}

vector<Point> ClipWithEdge(vector<Point> &points, int edge, int choice)
{
    if (points.size() < 2)
        return {};
    vector<Point> temp;
    Point last = points.back();
    bool last_in = Inside(last, edge, choice);
    for (int i = 0; i < (int)points.size(); i++)
    {
        Point cur = points[i];
        bool cur_in = Inside(cur, edge, choice);
        if (cur_in)
        {
            if (!last_in)
            {
                temp.push_back(Intersetion(last.x, last.y, cur.x, cur.y, edge, choice));
            }
            temp.push_back(cur);
        }
        else
        {
            if (last_in)
            {
                temp.push_back(Intersetion(last.x, last.y, cur.x, cur.y, edge, choice));
            }
        }
        last = cur, last_in = cur_in;
    }
    return temp;
}

bool HodgmanClipping(vector<Point> &points, int xleft, int ytop, int xright, int ybottom)
{
    if (points.size() < 2)
        return false;
    int edge[] = {xleft, ytop, xright, ybottom};
    for (int i = 0; i < 4; i++)
    {
        points = ClipWithEdge(points, edge[i], i);
    }
    return points.size() > 2;
}

#endif // CLIPPING_H_INCLUDED

#ifndef GENERAL_H_INCLUDED
#define GENERAL_H_INCLUDED

#include <algorithm>
#include <vector>
#include "curve.h"

using namespace std;

struct EdgeInfo
{
    double x;
    double invSlope;
    int yMax;
    bool operator<(EdgeInfo edge)
    {
        return x < edge.x;
    }
};

typedef vector<EdgeInfo> EdgeList;
EdgeInfo InitEdge(Point &v1, Point &v2)
{
    if (v1.y > v2.y)
    {
        swap(v1, v2);
    }
    EdgeInfo edge;
    edge.x = v1.x;
    edge.invSlope = (double)(v2.x - v1.x) / (v2.y - v1.y);
    edge.yMax = v2.y;
    return edge;
}
void BuildTable(vector<Point> &polygon, vector<EdgeList> &table)
{
    int n = polygon.size();
    Point v1 = polygon[n - 1];
    for (int i = 0; i < n; i++)
    {
        Point v2 = polygon[i];
        if (v1.y == v2.y)
        {
            v1 = v2;
            continue;
        }
        EdgeInfo edge = InitEdge(v1, v2);
        table[v1.y].push_back(edge);
        v1 = polygon[i];
    }
}
void FillGeneralPolygon(HDC hdc, vector<Point> &polygon, COLORREF c)
{
    int n = polygon.size();
    vector<EdgeList> table(MAX);
    BuildTable(polygon, table);
    int y = 0;
    while (y < MAX && table[y].size() == 0)
        y++;
    if (y == MAX)
        return;
    EdgeList ActiveList = table[y];
    while (ActiveList.size() > 0)
    {
        sort(ActiveList.begin(), ActiveList.end());
        for (auto it = ActiveList.begin(); it != ActiveList.end(); it++)
        {
            int x1 = (int)ceil(it->x);
            it++;
            int x2 = (int)floor(it->x);
            for (int x = x1; x <= x2; x++)
                SetPixel(hdc, x, y, c);
        }
        y++;
        auto it = ActiveList.begin();
        while (it != ActiveList.end())
        {
            if (y == it->yMax)
                it = ActiveList.erase(it);
            else
                it++;
        }
        for (auto it = ActiveList.begin(); it != ActiveList.end(); it++)
        {
            it->x += it->invSlope;
        }
        ActiveList.insert(ActiveList.end(), table[y].begin(), table[y].end());
    }
    polygon.clear();
}

#endif // GENERAL_H_INCLUDED

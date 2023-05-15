#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include "csgraphics.h"
#include <vector>

using namespace std;

struct Data
{
    int idx = -1;
    int n = 0;
    vector<Point> v = {};
    int r = 0;
    double t = 0;
    COLORREF bc = 0;
    COLORREF fc = 0;
    int c = -1;

    Data()
    {
    }

    Data(int id, int xs, int ys, COLORREF col, COLORREF col2, int choice)
    {
        idx = id;
        n = 1;
        v = {Point(xs, ys)};
        fc = col;
        bc = col2;
        c = choice;
    }

    Data(int id, int xs, int ys, int xe, int ye, COLORREF col, COLORREF col2, int choice)
    {
        idx = id;
        n = 2;
        v = {Point(xs, ys), Point(xe, ye)};
        fc = col;
        bc = col2;
        c = choice;
    }

    Data(int id, vector<Point> p, COLORREF col, COLORREF col2, int choice, double tt = 0)
    {
        idx = id;
        n = p.size();
        v = p;
        fc = col;
        bc = col2;
        c = choice;
        t = tt;
    }

    COLORREF GetColor(int x)
    {
        int bits = (1 << 8) - 1;
        int r = x & bits;
        bits <<= 8;
        int g = (x & bits) >> 8;
        bits <<= 8;
        int b = (x & bits) >> 16;
        return RGB(r, g, b);
    }

    void print(FILE *file)
    {
        fprintf(file, "%d %d\n", idx, n);
        for (int i = 0; i < n; i++)
        {
            fprintf(file, "%d %d\n", v[i].x, v[i].y);
        }
        fprintf(file, "%d %lf %d %d %d\n", r, t, (int)fc, (int)bc, c);
    }

    void read(FILE *file)
    {
        fscanf(file, "%d %d\n", &idx, &n);
        v = vector<Point>(n);
        for (int i = 0; i < n; i++)
        {
            fscanf(file, "%d %d\n", &v[i].x, &v[i].y);
        }
        int tempb, tempf;
        fscanf(file, "%d %lf %d %d %d\n", &r, &t, &tempf, &tempb, &c);
        fc = GetColor(tempf);
        bc = GetColor(tempb);
    }
};

void Save(vector<Data> &screen)
{
    FILE *file = fopen("screen.txt", "w");
    int n = screen.size();
    fprintf(file, "%d\n", n);
    for (int i = 0; i < n; i++)
    {
        screen[i].print(file);
    }
    fclose(file);
}

void Load(HDC hdc, vector<Data> &screen)
{
    FILE *file = fopen("screen.txt", "r");
    int n;
    fscanf(file, "%d\n", &n);
    screen = vector<Data>(n);
    for (int i = 0; i < n; i++)
    {
        screen[i].read(file);
    }
    fclose(file);
    for (int i = 0; i < n; i++)
    {
        Data &cur = screen[i];
        vector<Point> temp;
        switch (cur.idx)
        {
        case 6:
            DrawLine(hdc, cur.v[0].x, cur.v[0].y, cur.v[1].x, cur.v[1].y, cur.bc, cur.c);
            break;
        case 7:
            DrawCircle(hdc, cur.v[0].x, cur.v[0].y, 80, cur.bc, cur.c);
            break;
        case 8:
            DrawEllipse(hdc, cur.v[0].x, cur.v[0].y, 60, 120, cur.bc, cur.c);
            break;
        case 9:
            DrawCircle(hdc, cur.v[0].x, cur.v[0].y, 80, cur.bc, 4);
            FillWithLines(hdc, cur.v[0].x, cur.v[0].y, 80, cur.fc, cur.c);
            break;
        case 10:
            DrawCircle(hdc, cur.v[0].x, cur.v[0].y, 80, cur.bc, 4);
            FillWithCircles(hdc, cur.v[0].x, cur.v[0].y, 80, cur.fc, cur.c);
            break;
        case 11:
            DrawRectangle(hdc, cur.v[0].x, cur.v[0].y, cur.v[1].x, cur.v[1].y, cur.bc);
            FillWithHermite(hdc, cur.v[0].x, cur.v[0].y, cur.v[1].x, cur.v[1].y, cur.fc);
            break;
        case 12:
            DrawRectangle(hdc, cur.v[0].x, cur.v[0].y, cur.v[1].x, cur.v[1].y, cur.bc);
            FillWithBezier(hdc, cur.v[0].x, cur.v[0].y, cur.v[1].x, cur.v[1].y, cur.fc);
            break;
        case 13:
            temp = cur.v;
            DrawPolygon(hdc, temp, cur.bc);
            if (cur.c == 1)
            {
                FillConvexPolygon(hdc, temp, cur.fc);
            }
            else
            {
                FillGeneralPolygon(hdc, temp, cur.fc);
            }
            break;
        case 14:
            if (cur.c == -1)
            {
                DrawCircle(hdc, cur.v[0].x, cur.v[0].y, 40, cur.bc, 4);
            }
            else
            {
                IterativeFloodFill(hdc, cur.v[0].x, cur.v[0].y, cur.fc, cur.bc);
            }
            break;
        case 15:
            temp = cur.v;
            DrawCardinalSpline(hdc, temp, cur.t, cur.bc);
            break;
        case 16:
            if (cur.c != -1)
            {
                temp = cur.v;
                if (temp.size() == 2)
                {
                    // line clipping
                    DrawLine(hdc, cur.v[0].x, cur.v[0].y, cur.v[1].x, cur.v[1].y, cur.bc, cur.c);
                }
                else
                {
                    // polygon clipping
                }
            }
            else
            {
                DrawRectangle(hdc, cur.v[0].x, cur.v[0].y, cur.v[1].x, cur.v[1].y, cur.bc);
            }
            break;
        case 17:
            if (cur.c != -1)
            {
                MidPointLine(hdc, cur.v[0].x, cur.v[0].y, cur.v[1].x, cur.v[1].y, cur.bc);
            }
            else
            {
                DrawRectangle(hdc, cur.v[0].x, cur.v[0].y, cur.v[1].x, cur.v[1].y, cur.bc);
            }
            break;
        case 18:
            if (cur.c != -1)
            {
                ModifiedMidPointCircle(hdc, cur.v[0].x, cur.v[0].y, cur.r, cur.bc);
            }
            else
            {
                IterativeFloodFill(hdc, cur.v[0].x, cur.v[0].y, cur.fc, cur.bc);
            }
            break;
        default:
            break;
        }
    }
}

#endif // DATA_H_INCLUDED

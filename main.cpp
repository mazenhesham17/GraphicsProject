#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <string>
#include "csgraphics.h"

using namespace std ;


string titles[] =
{
    "Change the background",
    "Change the mouse shape",
    "Change the drawing color",
    "Clear Screen",
    "Save Screen",
    "Load Screen",
    "Draw Line",
    "Draw Circle",
    "Draw Ellipse",
    "Fill Circle with Lines",
    "Fill Circle with Circles",
    "Fill Square with Hermit Curve",
    "Fill rectangle with Bezier Curve",
    "Convex Filling",
    "Non Convex Filling",
    "Recursive Flood Fill",
    "Iterative Flood Fill",
    "Cardinal Spline Curve",
    "Clipping using Rectangle",
    "Clipping using Square",
};

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd, hwndCombo;                /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) CreateSolidBrush( RGB(200,200,200) );

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Graphics"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               700,                 /* The programs width */
               450,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    hwndCombo = CreateWindowEx(
                    WS_EX_CLIENTEDGE,
                    "ComboBox",
                    "",
                    WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,
                    10,
                    10,
                    220,
                    450,
                    hwnd,
                    NULL,
                    hThisInstance,
                    NULL
                );

    /* Add items to the combo box */
    for ( int i = 0 ; i < 21 ; i++ )
    {
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)titles[i].c_str());
    }

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int idx = -1 ;
    static int xs = -1, ys = -1 , xe = -1 , ye = -1 ;
    HDC hdc ;
    COLORREF WHITE = RGB(255,255,255) ;
    COLORREF bordercolor = RGB(255,0,0) , fillcolor = RGB(0,0,255) ;
    switch (message)                  /* handle the messages */
    {
    case WM_COMMAND:
        if (HIWORD(wParam) == CBN_SELCHANGE)
        {
            idx = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
            if ( idx == 0 )
            {
                // change the color of the background
                SetClassLongPtrA(hwnd,GCLP_HBRBACKGROUND,(LONG_PTR)CreateSolidBrush(WHITE)) ;
                InvalidateRect(hwnd,NULL,true) ;
            }
            else if ( idx == 1 )
            {
                // change the shape of the mouse
                SetClassLongPtrA(hwnd,GCLP_HCURSOR,(LONG_PTR)LoadCursor(NULL, IDC_CROSS)  ) ;
            }
            else if ( idx == 3 )
            {
                // clear the screen
                InvalidateRect(hwnd,NULL,true) ;
            }
        }
        break;

    case WM_LBUTTONDBLCLK:
        if ( idx == 6 ){
            xs = LOWORD(lParam) ;
            ys = HIWORD(lParam) ;
        }else if ( idx == 7 )
        {
            hdc = GetDC(hwnd) ;
            xs = LOWORD(lParam) ;
            ys = HIWORD(lParam) ;
            DrawCircle(hdc,xs,ys,80,bordercolor,3) ;
        }else if ( idx == 8 ){
            hdc = GetDC(hwnd) ;
            xs = LOWORD(lParam) ;
            ys = HIWORD(lParam) ;
            DrawEllipse(hdc,xs,ys,70,140,bordercolor,1) ;
        }else if ( idx == 9 ){
            hdc = GetDC(hwnd) ;
            xs = LOWORD(lParam) ;
            ys = HIWORD(lParam) ;
            DrawCircle(hdc,xs,ys,80,bordercolor,0) ;
            FillWithLines(hdc,xs,ys,80,fillcolor,1) ;
        }else if ( idx == 10 ){
            hdc = GetDC(hwnd) ;
            xs = LOWORD(lParam) ;
            ys = HIWORD(lParam) ;
            DrawCircle(hdc,xs,ys,80,bordercolor,0) ;
            FillWithCircles(hdc,xs,ys,80,fillcolor,1) ;
        }else if ( idx == 11  ){
            hdc = GetDC(hwnd) ;
            xs = LOWORD(lParam) ;
            ys = HIWORD(lParam) ;
            DrawSquare(hdc,xs,ys,xe,ye,250,bordercolor) ;
            FillWithHermite(hdc,xs,ys,xe,ye,fillcolor) ;
        }else if ( idx == 12 ){
            xs = LOWORD(lParam) ;
            ys = HIWORD(lParam) ;
        }else if ( idx == 15 || idx == 16 ){
            hdc = GetDC(hwnd) ;
            xs = LOWORD(lParam) ;
            ys = HIWORD(lParam) ;
            DrawCircle(hdc,xs,ys,40,bordercolor,3) ;
        }
        break;
    case WM_RBUTTONDBLCLK:
        if ( idx == 6 )
        {
            hdc = GetDC(hwnd) ;
            xe = LOWORD(lParam) ;
            ye = HIWORD(lParam) ;
            DrawLine(hdc,xs,ys,xe,ye,bordercolor,0) ;
        }else if ( idx == 12 ){
            hdc = GetDC(hwnd) ;
            xe = LOWORD(lParam) ;
            ye = HIWORD(lParam) ;
            DrawRectangle(hdc,xs,ys,xe,ye,bordercolor) ;
            FillWithBezier(hdc,xs,ys,xe,ye,fillcolor) ;
        }else if ( idx == 15 ){
            hdc = GetDC(hwnd) ;
            xe = LOWORD(lParam) ;
            ye = HIWORD(lParam) ;
            RecursiveFloodFill(hdc,xe,ye,fillcolor,bordercolor) ;
        }else if ( idx == 16 ){
            hdc = GetDC(hwnd) ;
            xe = LOWORD(lParam) ;
            ye = HIWORD(lParam) ;
            IterativeFloodFill(hdc,xe,ye,fillcolor,bordercolor) ;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

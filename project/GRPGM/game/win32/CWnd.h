
#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

#include "type.h"

typedef const char * (* LPFNRENDER)(BMPHEADER & bmpHeader, BMPINFOHEADER & biHeader);
typedef int (* LPFNCONTROL)(int key, int state, int x, int y);

class CWnd
{
public:
        CWnd();
        ~CWnd();

        bool        Create(int width, int height, LPFNRENDER renderproc, LPFNCONTROL controlproc); 
        static void Loop(void);

private:
        LPFNRENDER              render;
        LPFNCONTROL             control;
        HWND                    hWnd;
        int                     width, height;
        void                    OnPaint(void);
        char    *               buffer;
        void                    CalcFPS(void);

        static void             ShowLastError(void);
        static HINSTANCE        hInstance;
        static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        static DWORD WINAPI     RedrawProc(LPVOID lpThreadParameter);
        static HANDLE           hRedrawThread;
};



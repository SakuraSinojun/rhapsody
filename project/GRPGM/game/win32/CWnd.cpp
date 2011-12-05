

#include "CWnd.h"
#undef __STRICT_ANSI__
#include <vector>
#include <iostream>
#include <cstdio>
#include "Tools.h"


static std::vector<CWnd *>      wndVector;
HINSTANCE                       CWnd::hInstance = NULL;
HANDLE                          CWnd::hRedrawThread = NULL;

#define         WM_PAINT60      (WM_USER + 100)

DWORD WINAPI     CWnd::RedrawProc(LPVOID lpThreadParameter)
{
        LARGE_INTEGER   litmp;
        LONGLONG        qt1;
        static LONGLONG qt2 = 0;
        double          dft;
        double          dff;
        DWORD           id = (DWORD)lpThreadParameter;
        HANDLE          evt;

        Debug() << "main thread id = " << id << endl;

        evt = CreateEvent(NULL, 0, 0, TEXT("EVENT_DELAY"));

        while(1)
        {
                QueryPerformanceFrequency(&litmp);
                dff = (double)litmp.QuadPart;

                QueryPerformanceCounter(&litmp);
                qt1 = litmp.QuadPart;

                dft = (qt1 - qt2) / dff;
                if(dft > 1.0 / 60.0)
                {
                        // CTools::Get()->Debug("dft = %f\n", dft);
                        if(PostThreadMessage(id, WM_PAINT60, 0, 0) == 0)
                        {
                                Debug() << "PostThreadMessage failed" << endl;
                                ShowLastError();
                        }
                        qt2 = qt1;
                }
        }
        return 0;
}

CWnd::CWnd()
{
}

CWnd::~CWnd()
{
}

void    CWnd::ShowLastError(void)
{
        LPVOID lpMsgBuf;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,0, GetLastError(), 
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //Ä¬ÈÏÓïÑÔ
                (LPTSTR)&lpMsgBuf, 0, NULL );
        Debug() << lpMsgBuf << endl;
        LocalFree(lpMsgBuf);
}

bool    CWnd::Create(int width, int height, LPFNRENDER renderproc, LPFNCONTROL controlproc)
{
        WNDCLASS        wndClas;
        RECT            rect;
        const DWORD     dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

        this->render    = renderproc;
        this->control   = controlproc;

        if(this->hInstance == NULL)
                this->hInstance = GetModuleHandle(NULL);

        wndClas.cbClsExtra      = 0;
        wndClas.cbWndExtra      = 0;
        wndClas.hbrBackground   = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wndClas.hCursor         = LoadCursor(NULL, IDC_ARROW);
        wndClas.hIcon           = LoadIcon(NULL, IDI_APPLICATION);
        wndClas.hInstance       = hInstance;
        wndClas.lpfnWndProc     = WndProc;
        wndClas.lpszClassName   = "GRPGMWINDOW";
        wndClas.lpszMenuName    = NULL;
        wndClas.style           = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
        RegisterClass(&wndClas);
        
        rect.left       = 0;
        rect.top        = 0;
        rect.right      = width;
        rect.bottom     = height;
        AdjustWindowRect(&rect, dwStyle, FALSE);
        this->width = rect.right - rect.left;
        this->height = rect.bottom - rect.top;

        hWnd = CreateWindow("GRPGMWINDOW", "game", dwStyle,
                            CW_USEDEFAULT, 
                            CW_USEDEFAULT,
                            rect.right - rect.left,
                            rect.bottom - rect.top,
                            NULL,
                            NULL,
                            hInstance,
                            NULL);
        if(hWnd == NULL)
                return false;
        ShowWindow(hWnd, SW_SHOWNORMAL);
        wndVector.push_back(this);
        buffer = new char[width * height * 4];
        return true;
}

void CWnd::Loop(void)
{
        MSG     msg;
        PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
        
        hRedrawThread = CreateThread(NULL, 0, RedrawProc, (LPVOID)GetCurrentThreadId(), 0, NULL);
        if(hRedrawThread == NULL)
        {
                Debug() << "CreateThread failed" << endl;
                ShowLastError();
        }

        while(true)
        {
                if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
                {
                        if (!GetMessage(&msg, NULL, 0, 0))
                                return;
                        if(msg.message == WM_PAINT60)
                        {
                                std::vector<CWnd *>::iterator   iter;
                                CWnd *                          p;
                                for(iter = wndVector.begin(); iter != wndVector.end(); iter ++)
                                {
                                        p = *iter;
                                        if(p->control != NULL)
                                                p->control(KEY_BEFOREPAINT, 0, 0, 0);
                                        p->OnPaint();
                                        if(p->control != NULL)
                                                p->control(KEY_BEFOREPAINT, 0, 0, 0);
                                }
                        }
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                }else{
                        WaitMessage();
                }
        }

}


void CWnd::CalcFPS(void)
{
        LARGE_INTEGER           litmp;
        LONGLONG                qt1;
        static LONGLONG         qt2 = 0;
        double                  dft;
        double                  dff;
        double                  dfm;
        char                    FPS[100];
        static LONGLONG         framecount=0;
        double                  fps;

        QueryPerformanceFrequency(&litmp);
        dff = (double)litmp.QuadPart;

        QueryPerformanceCounter(&litmp);
        qt1 = litmp.QuadPart;

        framecount++;
        if(qt2 == 0)
        {
                qt2 = qt1;
                return;
        }

        dfm = (double)(qt1 - qt2);
        dft = dfm / dff;
        if(dft > 1)
        {
                fps = framecount / dft;
                framecount = 0;
                qt2 = qt1;
                sprintf(FPS,"FPS: %.2fÖ¡/Ãë", fps);
                SetWindowText(hWnd, FPS);
        }
}

void CWnd::OnPaint(void)
{        
        if(this->render == NULL)
                return;

        CalcFPS();

        HBITMAP                 hBitmap;
        BITMAPFILEHEADER        bmpHeader;
        BMPHEADER               bm;
        BMPINFOHEADER           bi;
        const char    *         buffer = render(bm, bi);
        HDC                     hdc;
        HDC                     memDC;
        struct
        {
            BITMAPINFOHEADER    Info;
            DWORD               BitField[3];
        } Header;

        memcpy(&bmpHeader, &bm, sizeof(BMPHEADER));
        memset(&Header,    0,   sizeof(BITMAPINFO));
        memcpy(&Header,    &bi, sizeof(BMPINFOHEADER));

        Header.Info.biPlanes        = 1;
        Header.Info.biCompression   = BI_BITFIELDS;
        Header.BitField[0] = 0xff0000;
        Header.BitField[1] = 0x00ff00;
        Header.BitField[2] = 0x0000ff;

        hdc = GetDC(hWnd);
        hBitmap = CreateDIBitmap(hdc, (BITMAPINFOHEADER *)&Header, CBM_INIT, buffer, (BITMAPINFO *)&Header, DIB_RGB_COLORS);
        if(hBitmap == NULL)
        {
                Debug() << "CreateDIBitmap failed" << endl;
                ReleaseDC(hWnd, hdc);
                ShowLastError();
                return;
        }

        memDC = CreateCompatibleDC(hdc);
        SelectObject(memDC, hBitmap);
        BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
        ReleaseDC(hWnd, hdc);
        DeleteDC(memDC);
        DeleteObject(hBitmap);
        
}

LRESULT CALLBACK CWnd::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
        CWnd    *                       p;
        std::vector<CWnd *>::iterator   iter; 
        PAINTSTRUCT                     ps;

        for(iter = wndVector.begin(); iter != wndVector.end(); iter ++)
        {
                p = *iter;
                if(p->hWnd == hWnd)
                {
                        switch(uMsg)
                        {
                        case WM_CLOSE: 
                                DestroyWindow(hWnd); 
                                break; 
                        case WM_DESTROY: 
                                wndVector.erase(iter);
                                if(wndVector.size() == 0)
                                {
                                        PostQuitMessage(0);
                                        if(hRedrawThread != NULL)
                                                CloseHandle(hRedrawThread);
                                        hRedrawThread = 0;
                                }
                                break; 
                        case WM_ERASEBKGND:
                                return false;
                        case WM_PAINT:
                                if(p->control != NULL)
                                        p->control(KEY_BEFOREPAINT, 0, 0, 0);
                                BeginPaint(hWnd, &ps);
                                p->OnPaint();
                                EndPaint(hWnd, &ps);
                                if(p->control != NULL)
                                        p->control(KEY_BEFOREPAINT, 0, 0, 0);
                                break;
                        case WM_KEYDOWN:
                                p->control(wParam, 0, 0, 0);
                                break;
                        case WM_KEYUP:
                                p->control(wParam, 1, 0, 0);
                                break;
                        case WM_LBUTTONDOWN:
                                break;
                        case WM_LBUTTONUP:
                                break;
                        case WM_RBUTTONDOWN:
                                break;
                        case WM_RBUTTONUP:
                                break;
                        case WM_MBUTTONDOWN:
                                break;
                        case WM_MBUTTONUP:
                                break;
                        case WM_MOUSEWHEEL:
                                break;
                        case WM_MOUSEMOVE:
                                break;
                        default:
                                break;
                        }
                        return DefWindowProc(hWnd, uMsg, wParam, lParam);
                }
        }
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
}




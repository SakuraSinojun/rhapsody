

#include <windows.h>


static HWND m_hWnd = NULL;
static CRITICAL_SECTION  cs;

//////////////////////////////////////////////////////////////////////////////////////////////////


static void _create_window(WNDPROC _lpfnWindowProc, int width, int height)
{
    HWND        hWnd;
    HINSTANCE   hInstance;
    WNDCLASS    wndclas; 
    DWORD       dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX ;
    RECT        rect;
    MSG         msgCur; 

    hInstance = GetModuleHandle(NULL);
    wndclas.cbClsExtra      = 0; 
    wndclas.cbWndExtra      = 0; 
    wndclas.hbrBackground   = (HBRUSH)GetStockObject(BLACK_BRUSH); 
    wndclas.hCursor         = LoadCursor(NULL,IDC_ARROW); 
    wndclas.hIcon           = LoadIcon(NULL,IDI_APPLICATION); 
    wndclas.hInstance       = hInstance; 
    wndclas.lpfnWndProc     = _lpfnWindowProc; 
    wndclas.lpszClassName   = "LINGWINDOW"; 
    wndclas.lpszMenuName    = NULL; 
    wndclas.style           = CS_VREDRAW | CS_HREDRAW | CS_OWNDC; 
    RegisterClass(&wndclas); 

    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;
    AdjustWindowRect(&rect, dwStyle, FALSE);

    hWnd = CreateWindow("LINGWINDOW",
                        "ling",
                        dwStyle ,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT, 
                        rect.right-rect.left,
                        rect.bottom-rect.top,
                        NULL,
                        NULL,
                        hInstance,
                        NULL); 

    ShowWindow (hWnd,SW_SHOWNORMAL); 


    while(GetMessage(&msgCur, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msgCur);
        DispatchMessage(&msgCur);
    }

    return ;
}

////////////////////////////////////////////////////////////////////////////////


BOOL OnIdle()
{
	return TRUE;
}


void OnPaint(HWND hwnd)
{
    HDC     hdc = GetDC(hwnd);
    HBRUSH  hbrush = CreateSolidBrush(RGB(255, 255, 0));
    SelectObject(hdc, hbrush);
    Rectangle(hdc, 100, 100, 500, 500);
    DeleteObject(hbrush);
    ReleaseDC(hwnd, hdc);
}

void OnCreate(HWND hWnd)
{
}

void OnClose()
{       
}

////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK _WndProc(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam) 
{ 
    PAINTSTRUCT 	ps;
    switch(uMsg) 
    {
    case WM_CREATE:
        OnCreate(hwnd);
        break;
    case WM_CLOSE:
        OnClose();
        PostQuitMessage (0);
        break;
    case WM_DESTROY:
        exit(0);
        break;
    case WM_PAINT:
        BeginPaint(hwnd, &ps);
        OnPaint(hwnd);
        EndPaint(hwnd, &ps);
        break;
    case WM_ERASEBKGND:
    default:
        return DefWindowProc(hwnd,uMsg,wParam,lParam); 
    }
    return DefWindowProc(hwnd,uMsg,wParam,lParam); 
}


int main()
{
	_create_window(_WndProc, 640, 480);
	return 0;
}








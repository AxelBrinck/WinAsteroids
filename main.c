#include <windows.h>

HWND Window;

void Draw(HDC hdc, RECT *windowRect)
{
    static int x = 0; x++; x = x > 60 ? 0 : x;
    HBRUSH color = CreateSolidBrush(RGB(200 + x, 255 - x, 255 - x * 2));
    HDC buffer = CreateCompatibleDC(hdc);

    FillRect(hdc, windowRect, color);
    LineTo(hdc, x , x);
    LineTo(hdc, x / 2 + 2 , 23);
    SetPixel(hdc, 400, 400, (COLORREF) 0x00000000);


    DeleteObject(color);
    DeleteDC(buffer);
}

void NextFrame(HWND hwnd)
{
    RECT windowRect;
    PAINTSTRUCT ps;

    HDC hdc = BeginPaint(hwnd, &ps);

    GetClientRect(hwnd, &windowRect);

    Draw(hdc, &windowRect);

    EndPaint(hwnd, &ps);

    DeleteObject(hdc);
}

/**
 * Windows Callback function. Also called WndProc.
 * Windows API will send us messages and we will have to act upon them.
 * Not listening for messages will cause the window to freeze.
 */ 
LRESULT CALLBACK WindowCallback(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    { 
    
    // Called when we create the window.
    case WM_CREATE:
        SetTimer(hwnd, 1, 20, NULL);
        break;

    // Called when window wants our window to destroy.
    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        break;

    // Each tick of the timer, we result on a call to this function.
    case WM_TIMER:
        InvalidateRect(hwnd, NULL, FALSE);
        break;

    // Whenever our window is invalidated (or needs to be redrawn), this
    // function will be called.
    case WM_PAINT:
        NextFrame(hwnd);
        break;

    case WM_KEYDOWN:
        printf("%c", wParam);
        break;

    case WM_KEYUP:
        printf("%c", wParam);
        break;

    // For the rest of the messages (there are many) we will return the deault
    // procedure. This will cause the window to not freeze.
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

/**
 * Main function.
 * Will define and create a window with the Windows API, and poll the message
 * loop.
 */
int APIENTRY WinMain(
    HINSTANCE Instance,
    HINSTANCE PrevInstance,
    LPSTR CommandLine,
    int ShowCode)
{

    // This is the window class. It is going to define several aspects for 
    // our window.
    WNDCLASS WindowClass = { 0 };
    
    WindowClass.lpfnWndProc = WindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpszClassName = "AsteroidsWindowClass";

    // Registering the window with the Window API makes Windows aware of this
    // new window. We must stop execution if there is an error at this point.
    if (!RegisterClass(&WindowClass)) return -1;

    // We now get our window from the API. We are going to save as global var
    // for later use. Of course, if there is a problem at this point its going
    // to be fatal.
    Window = CreateWindow(
        WindowClass.lpszClassName,
        "Asteroids",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        Instance,
        0);

    if (!Window) return -1;

    // Windows message loop.
    // Here we are going to receive messages from Windows API.
    // We are going to process them in the WinProc function.
    // Here that function is called WindowCallback, which is indeed, a called
    // back function.
    MSG msg;
    do
    {
        GetMessageW(&msg, 0, 0, 0);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    while (msg.message != WM_QUIT);

    return 0;
}

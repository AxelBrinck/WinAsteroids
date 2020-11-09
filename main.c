#include <windows.h>

void Draw(HDC hdc, RECT *prc)
{
	static int x = 0; x++;

	HDC hdcBuffer = CreateCompatibleDC(hdc);

    FillRect(hdc, prc, CreateSolidBrush(RGB(200, 255, 255)));
	LineTo(hdc, 500, x);

    BitBlt(hdc, 0, 0, 3000, 3000, hdcBuffer, 0, 0, SRCCOPY);

    DeleteDC(hdcBuffer);
}

void NextFrame(HWND hwnd)
{
	RECT rcClient;
	HDC hdc = GetDC(hwnd);

	GetClientRect(hwnd, &rcClient);

	Draw(hdc, &rcClient);

	ReleaseDC(hwnd, hdc);
}

LRESULT APIENTRY WindowCallback(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
    switch (msg)
    { 
	case WM_CREATE:
		SetTimer(hwnd, 1, 16, NULL);
		break;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		InvalidateRect(hwnd, NULL, FALSE);
		break;

	case WM_PAINT:
		NextFrame(hwnd);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

int APIENTRY WinMain(
	HINSTANCE Instance,
    HINSTANCE PrevInstance,
    LPSTR CommandLine,
    int ShowCode)
{
    WNDCLASS WindowClass = { 0 };
    
    WindowClass.lpfnWndProc = WindowCallback;
    WindowClass.hInstance = Instance;
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpszClassName = "AsteroidsWindowClass";

	if (!RegisterClass(&WindowClass)) return -1;

	HWND Window = CreateWindow(
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

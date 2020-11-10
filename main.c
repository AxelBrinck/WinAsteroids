#include <windows.h>

void Draw(HDC hdc, RECT *windowRect)
{
	static int x = 0; x++; x = x > 60 ? 0 : x;
	HBRUSH color = CreateSolidBrush(RGB(200 + x, 255 - x, 255 - x * 2));

    FillRect(hdc, windowRect, color);
	LineTo(hdc, x , x);
	LineTo(hdc, x / 2 + 2 , 23);
	SetPixel(hdc, 400, 400, (COLORREF) 0x00000000);

	DeleteObject(color);
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

LRESULT CALLBACK WindowCallback(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
    switch (msg)
    { 
	case WM_CREATE:
		// TODO: Create a cleaner way
		SetTimer(hwnd, 1, 20, NULL);
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

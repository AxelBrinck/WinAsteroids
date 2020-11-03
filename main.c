#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC         hdc;
	RECT        rc;

	switch (message)
	{

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);

			GetClientRect(hwnd, &rc);
			SetTextColor(hdc, RGB(240,240,96));
			SetBkMode(hdc, TRANSPARENT);
			DrawText(hdc, "Hello", -1, &rc, DT_CENTER|DT_SINGLELINE|DT_VCENTER);

			EndPaint(hwnd, &ps);
			break;

		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	WNDCLASS wc;

	ZeroMemory(&wc, sizeof wc);
	wc.hInstance     = hInstance;
	wc.lpszClassName = "szAppName";
	wc.lpfnWndProc   = (WNDPROC)WndProc;
	wc.style         = CS_DBLCLKS|CS_VREDRAW|CS_HREDRAW;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);

	if (FALSE == RegisterClass(&wc)) return 0;

	HWND hwnd = CreateWindow(
		"szAppName",
		"szTitle",
		WS_OVERLAPPEDWINDOW|WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		hInstance,
		0);

	if (NULL == hwnd) return 0;

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
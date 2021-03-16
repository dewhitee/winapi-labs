#include <windows.h>
#include <string>
#include <stdio.h>

///BOOL isRect = FALSE;
POINT position;
POINT startPosition;

LONG WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	MSG msg;
	WNDCLASS w;
	memset(&w, 0, sizeof(WNDCLASS));
	w.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	w.lpfnWndProc = WndProc;
	w.hInstance = hInstance;
	w.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	w.lpszClassName = L"My Class";
	w.hIcon = LoadIconW(NULL, IDI_QUESTION); // Set window icon to question mark icon (NULL as we use default one)

	RegisterClass(&w);

	HWND hwnd;
	hwnd = CreateWindow(L"My Class", L"Labwork 2.1 (Deniss Belovs 4801BD)", WS_OVERLAPPEDWINDOW,
		300, 200, 200, 180, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, SW_MINIMIZE);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LONG WINAPI WndProc(HWND hwnd, UINT Message, WPARAM wparam, LPARAM lparam)
{
	DWORD x, y;
	switch (Message)
	{
	case WM_LBUTTONDOWN:
	{
		// Getting current cursor position coordinates
		x = LOWORD(lparam);
		y = HIWORD(lparam);

		HDC hdc = GetDC(hwnd);

		// Initializing rectangle that will be used as white background for text, 
		// to remove previous text in case it was larger than the current one
		RECT r;
		r.right = 160;
		r.bottom = 30;
		r.left = 0;
		r.top = 0;

		// Filling the rectangle with white background
		FillRect(hdc, &r, (HBRUSH)GetStockObject(WHITE_BRUSH));

		// Initializing cursor position widechar string and getting it's length
		WCHAR str[30];
		int len = swprintf_s(str, L"x = %i, y = %i", x, y);

		// Text out current cursor position coordinates
		TextOut(hdc, 10, 10, str, /*lstrlenW(str)*/len);

		ReleaseDC(hwnd, hdc);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		HDC hdc = GetDC(hwnd);

		x = LOWORD(lparam);
		y = HIWORD(lparam);

		WCHAR str[] = L"Right button click occurred.";
		TextOut(hdc, x, y, str, lstrlenW(str));
		break;
	}
	case WM_RBUTTONUP:
	{
		///HDC hdc = GetDC(hwnd);

		// Set start position for LineTo to saved x and y from right button click event
		///MoveToEx(hdc, startPosition.x, startPosition.y, &position);

		// Get current cursor position
		///position.x = LOWORD(lparam);
		///position.y = HIWORD(lparam);

		///LineTo(hdc, position.x, position.y);
		///ReleaseDC(hwnd, hdc);

		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		// Default events such as window resizing, minimazing and maximizing e.t.c
		return DefWindowProc(hwnd, Message, wparam, lparam);
	}
	return 0;
}
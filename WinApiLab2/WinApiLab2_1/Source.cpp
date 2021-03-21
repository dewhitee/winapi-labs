#include <windows.h>
#include <string>
#include <stdio.h>
#include <vector>

// Deniss Belovs 4801BD (Lab 2.1)

RECT cursorPositionTextRect;
WCHAR cursorPositionTextStr[30];
std::vector<POINT> rightButtonClickPositions;

LONG WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

RECT GetCursorPositionTextRect();
HFONT GetCursorPositionTextFont();
HFONT GetButtonClickTextFont();
HFONT GetAdditionalTextFont();
VOID DrawCursorPositionText(HDC hdc);
VOID DrawAdditionalText(HDC hdc);
VOID DrawRightButtonClickMessage(HDC hdc, DWORD x, DWORD y);
VOID RedrawAllRightButtonClickMessages(HDC hdc);

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
	w.lpszClassName = L"My window class";
	w.hIcon = LoadIconW(NULL, IDI_QUESTION); // Set window icon to question mark icon (NULL as we use default one)

	RegisterClass(&w);

	HWND hwnd;
	hwnd = CreateWindow(L"My window class", L"Labwork 2.1 (Deniss Belovs 4801BD)", WS_OVERLAPPEDWINDOW,
		300, 200, 200, 180, NULL, NULL, hInstance, NULL);

	// Init rectangle that will be used as white background for text, 
	// to remove previous text in case it was larger than the current one
	cursorPositionTextRect = GetCursorPositionTextRect();

	// Init the cursor position text
	swprintf_s(cursorPositionTextStr, L"x = 0, y = 0");

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
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// Redraw all right button click messages with custom font
		HFONT font = (HFONT)SelectObject(hdc, (HFONT)GetButtonClickTextFont());
		RedrawAllRightButtonClickMessages(hdc);
		DeleteObject(SelectObject(hdc, font));
		
		// Cursor coordinates text with bold font
		DrawCursorPositionText(hdc);

		// Additional text with different font
		DrawAdditionalText(hdc);
		
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		// Getting current cursor position coordinates
		x = LOWORD(lparam);
		y = HIWORD(lparam);

		HDC hdc = GetDC(hwnd);

		// Filling the rectangle with white color to remove previous text 
		// in case it was larger than the current one.
		FillRect(hdc, &cursorPositionTextRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

		// Init cursor position widechar string
		swprintf_s(cursorPositionTextStr, L"x = %i, y = %i", x, y);

		DrawCursorPositionText(hdc);
		DrawAdditionalText(hdc);

		ReleaseDC(hwnd, hdc);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		HDC hdc = GetDC(hwnd);

		x = LOWORD(lparam);
		y = HIWORD(lparam);

		HFONT font = (HFONT)SelectObject(hdc, (HFONT)GetButtonClickTextFont());
		DrawRightButtonClickMessage(hdc, x, y);
		DeleteObject(SelectObject(hdc, font));
		ReleaseDC(hwnd, hdc);

		rightButtonClickPositions.push_back(POINT{ (LONG)x, (LONG)y });
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

RECT GetCursorPositionTextRect()
{
	RECT cursorPositionTextRect;
	cursorPositionTextRect.right = 240;
	cursorPositionTextRect.bottom = 40;
	cursorPositionTextRect.left = 10;
	cursorPositionTextRect.top = 0;
	return cursorPositionTextRect;
}

HFONT GetCursorPositionTextFont()
{
	return CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, NULL);
}
HFONT GetButtonClickTextFont()
{
	return CreateFont(50, 0, 0, 0, FW_MEDIUM, TRUE, TRUE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, NULL);
}
HFONT GetAdditionalTextFont()
{
	return CreateFont(16, 0, 0, 0, FW_LIGHT, FALSE, TRUE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, NULL);
}
VOID DrawCursorPositionText(HDC hdc)
{
	HFONT font = (HFONT)SelectObject(hdc, (HFONT)GetCursorPositionTextFont());
	DrawText(hdc, cursorPositionTextStr, -1, &cursorPositionTextRect, 
		DT_SINGLELINE | DT_LEFT | DT_VCENTER);

	DeleteObject(SelectObject(hdc, font));
}
VOID DrawAdditionalText(HDC hdc)
{
	HFONT font = (HFONT)SelectObject(hdc, (HFONT)GetAdditionalTextFont());
	RECT r;
	r.right = 360;
	r.bottom = 20;
	r.left = 0;
	r.top = 64;

	DrawText(hdc, L"Last cursor position coordinates (Left mouse click to update)", -1, &r, 
		DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

	DeleteObject(SelectObject(hdc, font));
}
VOID DrawRightButtonClickMessage(HDC hdc, DWORD x, DWORD y)
{
	// Set background drawing mode to trans
	SetBkMode(hdc, TRANSPARENT);

	WCHAR str[] = L"Right button click occurred.";
	TextOut(hdc, x, y, str, lstrlenW(str));
}
VOID RedrawAllRightButtonClickMessages(HDC hdc)
{
	for (POINT position : rightButtonClickPositions)
	{
		DrawRightButtonClickMessage(hdc, position.x, position.y);
	}

	// Restore default background drawing mode
	SetBkMode(hdc, OPAQUE);
}

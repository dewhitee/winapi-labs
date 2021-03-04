#include <windows.h>
#include <string>
#include <stdio.h>

BOOL isRect = FALSE;
BOOL startedDraw = FALSE;
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
	w.hIcon = LoadIconW(hInstance, IDI_APPLICATION);
	w.hCursor = LoadCursorW(hInstance, IDC_ARROW);

	RegisterClass(&w);

	HWND hwnd;
	hwnd = CreateWindow(L"My Class", L"My title", WS_OVERLAPPEDWINDOW,
		300, 200, 200, 180, NULL, NULL, hInstance, NULL);


	ShowWindow(hwnd, SW_MINIMIZE);
	UpdateWindow(hwnd);

	/*HWND hwnd2 = CreateWindow(L"Welcome to the club", L"HELLOOOOOOO", WS_OVERLAPPEDWINDOW,
		350, 250, 400, 280, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd2, SW_MAXIMIZE);
	UpdateWindow(hwnd2);*/

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
		HDC hdc = GetDC(hwnd);

		x = LOWORD(lparam);
		y = HIWORD(lparam);

		WCHAR str[40];
		int len = swprintf_s(str, L"x = %i, y = %i", x, y);

		TextOut(hdc, 10, 10, str, /*lstrlenW(str)*/len);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		//HDC hdc = GetDC(hwnd);

		//// Save current position as start position for LineTo function
		//startPosition.x = LOWORD(lparam);
		//startPosition.y = HIWORD(lparam);

		//WCHAR str[] = L"Middle button click occurred.";
		//TextOut(hdc, startPosition.x, startPosition.y, str, lstrlenW(str));

		//startedDraw = TRUE;
		break;
	}
	case WM_MBUTTONUP:
	{
		//if (startedDraw)
		//{
		//	HDC hdc = GetDC(hwnd);

		//	// Set to erace previous line while not touching other graphical content of this window
		//	SetROP2(hdc, R2_NOT);

		//	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		//	SelectObject(hdc, pen);

		//	MoveToEx(hdc, startPosition.x, startPosition.y, NULL);
		//	LineTo(hdc, position.x, position.y);

		//	DeleteObject(pen);

		//	///HPEN pen = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
		//	///SelectObject(hdc, pen);

		//	//if (position.x != 0 && position.y != 0)
		//	//{
		//	///MoveToEx(hdc, position.x, position.y, NULL);
		//	///LineTo(hdc, position.x, position.y);
		//	//}

		//	///DeleteObject(pen);
		//	///pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

		//	//RedrawWindow(hwnd, 0, 0, RDW_INVALIDATE);

		//	// Get current cursor position
		//	x = LOWORD(lparam);
		//	y = HIWORD(lparam);

		//	// Set start position for LineTo to saved x and y from right button click event
		//	MoveToEx(hdc, startPosition.x, startPosition.y, NULL);

		//	// Draw new line from to current cursor position
		//	LineTo(hdc, x, y);

		//	ReleaseDC(hwnd, hdc);

		//	position.x = x;
		//	position.y = y;
		//}
		break;
	}
	case WM_RBUTTONDOWN:
	{
		HDC hdc = GetDC(hwnd);

		// Save current position as start position for LineTo function
		startPosition.x = LOWORD(lparam);
		startPosition.y = HIWORD(lparam);

		WCHAR str[] = L"Right button click occurred.";
		TextOut(hdc, startPosition.x, startPosition.y, str, lstrlenW(str));

		position.x = 0;
		position.y = 0;

		startedDraw = TRUE;
		break;
	}
	case WM_RBUTTONUP:
	{
		if (startedDraw)
		{
			HDC hdc = GetDC(hwnd);

			// Set start position for LineTo to saved x and y from right button click event
			MoveToEx(hdc, startPosition.x, startPosition.y, &position);

			// Get current cursor position
			position.x = LOWORD(lparam);
			position.y = HIWORD(lparam);

			LineTo(hdc, position.x, position.y);
			ReleaseDC(hwnd, hdc);

			startedDraw = FALSE;
		}
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (startedDraw)
		{
			HDC hdc = GetDC(hwnd);

			// If we are not in the start of coordinates
			if (position.x != 0 && position.y != 0)
			{
				// Set to erace previous line while not touching other graphical content of this window
				SetROP2(hdc, R2_NOT);

				HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
				SelectObject(hdc, pen);
				MoveToEx(hdc, startPosition.x, startPosition.y, NULL);
				LineTo(hdc, position.x, position.y);
				//DeleteObject(pen);
			}


			//RedrawWindow(hwnd, 0, 0, RDW_INVALIDATE);

			// Get current cursor position
			x = LOWORD(lparam);
			y = HIWORD(lparam);

			// Set start position for LineTo to saved x and y from right button click event
			MoveToEx(hdc, startPosition.x, startPosition.y, NULL);

			// Draw new line from to current cursor position
			LineTo(hdc, x, y);

			ReleaseDC(hwnd, hdc);

			position.x = x;
			position.y = y;
		}
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
#include <windows.h>
#include <string>
#include <stdio.h>

BOOL isRect = FALSE;

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
	case WM_PAINT:
	{
		if (isRect)
		{
			RECT r;
			r.right = 600;
			r.bottom = 600;
			r.left = 100;
			r.top = 100;
			PAINTSTRUCT ps1; // this structure contains coordinates of corrupt zone of window to repaint it

			//LPPAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps1);
			// if we use HDC and ReleaseDC then this function will repaint WHOLE window instead of only corrupt zones
			//HDC hdc = GetDC(hwnd);
			SelectObject(hdc, (HBRUSH)GetStockObject(DC_BRUSH));
			SetDCBrushColor(hdc, RGB(255, 0, 0));
			FillRect(hdc, &r, (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(DC_BRUSH)));
			//ReleaseDC(hwnd, hdc);
			EndPaint(hwnd, &ps1);
		}
		break;
	}
	case WM_RBUTTONDBLCLK:
	{
		RECT r;
		r.right = 600;
		r.bottom = 600;
		r.left = 100;
		r.top = 100;
		HDC hdc = GetDC(hwnd);
		FillRect(hdc, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));
		ReleaseDC(hwnd, hdc);
		isRect = TRUE;
		break;
	}
	case WM_LBUTTONUP:
	{
		//y = lparam >> 16;
		//x = lparam & 0xFFFF;
		//wchar_t s[255];
		//swprintf_s(s, L"x = %i y = %i", x, y);
		y = HIWORD(lparam);
		x = LOWORD(lparam);
		MessageBox(hwnd, L"HELLO!" /*s*/, L"WORLD!" /*(LPCWSTR)_itoa_s((int)hwnd, buff, 10)*/, MB_OK);
		break;
	}
	//case WM_MOUSEMOVE:
	//{
	//	y = lparam >> 16;
	//	x = lparam & 0xFFFF;
	//	HDC hdc = GetDC(hwnd); // PEN 0,0
	//	//SetPixel(hdc, x, y, RGB(255, 0, 0));
	//	LineTo(hdc, x, y); // PEN color, x, y
	//	// remember position of x and y in global static variable to LineTo from not zero position
	//	ReleaseDC(hwnd, hdc);
	//	break;
	//}
	case WM_DESTROY:
		PostQuitMessage(0);
		break; 
	default:
		// Default events such as window resizing, minimazing and maximizing e.t.c
		return DefWindowProc(hwnd, Message, wparam, lparam);
	}
	return 0;
}
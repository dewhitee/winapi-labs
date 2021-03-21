#include <windows.h>
#include <string>
#include <stdio.h>
#include <vector>

// Deniss Belovs 4801BD (Lab 2.2)

BOOL startedDraw = FALSE;
POINT position;
POINT startPosition;
std::vector<std::pair<POINT, POINT>> linePositions;

LONG WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

HPEN GetCustomPen();
VOID RedrawAllLines(HDC hdc);
HFONT GetAdditionalTextFont();
VOID DrawAdditionalText(HDC hdc);


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
	w.hCursor = LoadCursorW(NULL, IDC_ARROW); // Set cursor to arrow cursor (NULL as we use default one)

	RegisterClass(&w);

	HWND hwnd;
	hwnd = CreateWindow(L"My window class", L"Labwork 2.2 (Deniss Belovs 4801BD)", WS_OVERLAPPEDWINDOW,
		300, 200, 1200, 800, NULL, NULL, hInstance, NULL);


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
		if (!startedDraw)
		{
			// Restore window
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			DrawAdditionalText(hdc);
			RedrawAllLines(hdc);
			EndPaint(hwnd, &ps);
		}
		break;
	}
	case WM_RBUTTONDOWN:
	{
		HDC hdc = GetDC(hwnd);

		// Save current position as start position for LineTo function
		startPosition.x = LOWORD(lparam);
		startPosition.y = HIWORD(lparam);

		// Nulling the END position of line to disable drawing from this END position to the 
		// new line STARTING position and to disable drawing from the start of coordinates on the mouse move.
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

			// Set line color and style
			HPEN pen = GetCustomPen();
			SelectObject(hdc, pen);

			// Draw the final line
			LineTo(hdc, position.x, position.y);

			DeleteObject(pen);
			ReleaseDC(hwnd, hdc);

			startedDraw = FALSE;

			// Save current start and end position of line in vector
			linePositions.push_back(
				std::make_pair(POINT{ startPosition.x, startPosition.y }, POINT{ position.x, position.y }));
		}
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (startedDraw)
		{
			HDC hdc = GetDC(hwnd);

			// Set line color and style
			HPEN pen = GetCustomPen();
			SelectObject(hdc, pen);

			// If we are not in the start of coordinates
			// (we only want to draw the line when the cursor position is not at the start of coordinates)
			if (position.x != 0 && position.y != 0)
			{
				// Set to erase previous line while not touching other graphical content of this window.
				//
				// Note: Everything was working with it before I added custom style to the line, so 
				// I decided to use the NOTXORPEN instead as it seems to be the only one that works in this case.
				// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-setrop2
				///SetROP2(hdc, R2_NOT);
				SetROP2(hdc, R2_NOTXORPEN);

				// Drawing line with the xor inversed pen-color over the previous one
				MoveToEx(hdc, startPosition.x, startPosition.y, NULL);
				LineTo(hdc, position.x, position.y);
			}

			// Get current cursor position
			x = LOWORD(lparam);
			y = HIWORD(lparam);

			// Set start position for LineTo to saved x and y from right button click event
			MoveToEx(hdc, startPosition.x, startPosition.y, NULL);

			// Draw new line from to current cursor position
			LineTo(hdc, x, y);
			
			DeleteObject(pen);
			ReleaseDC(hwnd, hdc);

			// Save current position to remove the current line on the next WM_MOUSEMOVE call.
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

HPEN GetCustomPen()
{
	return CreatePen(PS_DASHDOT, 1, RGB(0, 123, 255));
}

VOID RedrawAllLines(HDC hdc)
{
	HPEN pen = GetCustomPen();
	SelectObject(hdc, pen);
	for (const std::pair<POINT, POINT>& position : linePositions)
	{
		MoveToEx(hdc, position.first.x, position.first.y, NULL);
		LineTo(hdc, position.second.x, position.second.y);
	}
	DeleteObject(pen);
}

HFONT GetAdditionalTextFont()
{
	return CreateFont(24, 0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, NULL);
}

VOID DrawAdditionalText(HDC hdc)
{
	HFONT font = (HFONT)SelectObject(hdc, (HFONT)GetAdditionalTextFont());
	RECT r;
	r.right = 900;
	r.bottom = 40;
	r.left = 20;
	r.top = 10;

	DrawText(hdc, L"Press right mouse button and drag somewhere while keeping it pressed to draw the line.", -1, &r,
		DT_SINGLELINE | DT_LEFT | DT_VCENTER);

	DeleteObject(SelectObject(hdc, font));
}

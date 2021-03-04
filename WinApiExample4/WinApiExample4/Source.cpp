#include <iostream>
#include <Windows.h>

using namespace std;

// Finds Notepad window with name 'a', draws the line and prints Hello World
int main()
{
	HWND hWnd = FindWindow(NULL, L"a - Notepad");
	cout << "hWnd = " << hWnd << endl;
	HDC hDc = GetDC(hWnd);
	LineTo(hDc, 100, 100);
	TextOut(hDc, 100, 100, L"Hello World!", 13);
	ReleaseDC(hWnd, hDc);
	system("PAUSE");
	return 0;
}
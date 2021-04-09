#include <Windows.h>
#include <tchar.h>
#include <iostream>

using namespace std;

void main() // Thread
{
	DWORD dwExitCode;
	STARTUPINFO cif;
	ZeroMemory(&cif, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	if (CreateProcess(NULL, (LPWSTR)_TEXT("C:\\Windows\\system32\\notepad.exe"),
		NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi) == TRUE)
	{
		cout << "process" << endl;
		cout << "handle" << pi.hProcess << endl;

		Sleep(2000);
		TerminateProcess(pi.hProcess, NO_ERROR);
		if (WaitForSingleObject(pi.hProcess, INFINITE) != WAIT_FAILED) // waits for notepad to close
		{
			GetExitCodeProcess(pi.hProcess, &dwExitCode);
			cout << dwExitCode << endl;
		}

		CloseHandle(pi.hProcess);
	}
	system("PAUSE");
}
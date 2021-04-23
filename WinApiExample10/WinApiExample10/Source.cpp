#include <iostream>
#include <windows.h>

using namespace std;

int main()
{
	wchar_t fileName[] = L"F:\\dev\\winapi\\WinApiExample10\\Debug\\a.txt";
	HANDLE hFile = CreateFile(fileName, 0, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		cout << "ERROR\n";
	}
	else
	{
		CloseHandle(hFile);
		STARTUPINFO startupInfo;
		memset(&startupInfo, 0, sizeof(STARTUPINFO));
		PROCESS_INFORMATION processInfo;
		wchar_t processName[600] = L"F:\\dev\\winapi\\WinApiExample10\\Debug\\slave.exe a.txt ";
		wcscat_s(processName, fileName);

		if (CreateProcess(NULL, processName, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE, NULL, NULL, 
			&startupInfo, &processInfo))
		{
			if (WaitForSingleObject(processInfo.hProcess, 100000) != WAIT_TIMEOUT)
			{
				hFile = CreateFile(L"a.txt", GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hFile == INVALID_HANDLE_VALUE)
				{
					cout << "Error\n";
				}
				else
				{
					DWORD n;
					char a;
					// read from file
					while (ReadFile(hFile, &a, sizeof(char), &n, NULL))
					{
						cout << a;
					}
					cout << endl;
					CloseHandle(hFile);
				}
			}
			CloseHandle(processInfo.hProcess);
		}
	}

	system("pause");
	return 0;
}
#include <iostream>
#include <thread>
#include <windows.h>
#include <system_error>

// Deniss Belovs 4801BD

using namespace std;

int main(int argc, char** argv)
{
	DWORD dwExitCode;
	STARTUPINFO startupInfo;
	ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
	PROCESS_INFORMATION processInfo;

	CONST DWORD currentDirectoryBuffLen = 255;
	WCHAR currentDirectoryBuffer[currentDirectoryBuffLen];
	GetCurrentDirectory(currentDirectoryBuffLen, currentDirectoryBuffer);
	wcout << "Current directory: " << currentDirectoryBuffer << endl;
	wstring currentDirectory = wstring(currentDirectoryBuffer);
	currentDirectory = currentDirectory.substr(0, currentDirectory.size() - wcslen(L"\\WinApiLab6"));
	wcout << "Current directory adjusted: " << currentDirectory << endl;

	if (argc < 2)
	{
		cout << "Not enough arguments.\nStarting default SLAVE process.\n";
		wstring finalPath = currentDirectory + wstring(L"\\Debug\\WinApiLab6_Slave.exe");
		//WCHAR defaultCommand[] = L".\\WinApiLab6_Slave.exe C:\\Users\\DEWHITEE\\Desktop\\TEXTDOCS\\eng.txt";
		wstring defaultCommand = finalPath + wstring(L" C:\\Users\\DEWHITEE\\Desktop\\TEXTDOCS\\eng.txt");
		wcout << "Default command = " << defaultCommand << endl;
		if (CreateProcess(NULL, const_cast<LPWSTR>(defaultCommand.c_str()), NULL, NULL, FALSE, NULL, NULL, NULL, &startupInfo, &processInfo))
		{
			DWORD waitResult = WaitForSingleObject(processInfo.hProcess, 10000);
			if (waitResult == 0)
			{
				cout << "SLAVE is closed\n";
			}
			else if (waitResult == WAIT_TIMEOUT)
			{
				cout << "TIMEOUT\n";
			}
			else
			{
				cout << "ERROR\n";
			}
			//CloseHandle(processInfo.hThread);
			CloseHandle(processInfo.hProcess);
		}
		else
		{
			cout << "Something went wrong\n";
			cout << "ERROR: " << system_category().message(GetLastError()) << endl;
		}
		return 0;
	}

	LPSTR executableName = argv[1];
	
	//if (CreateProcess(NULL, ""))
	return 0;
}

#include <iostream>
#include <thread>
#include <windows.h>

// Deniss Belovs 4801BD

using namespace std;

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		cout << "Not enough arguments\n";
		return 0;
	}

	DWORD dwExitCode;
	STARTUPINFO startupInfo;
	ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	LPSTR executableName = argv[1];
	//if (CreateProcess(NULL, ""))
	return 0;
}

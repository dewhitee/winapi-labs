#include <iostream>
#include <windows.h>

using namespace std;

int main()
{
	STARTUPINFO startupInfo;
	memset(&startupInfo, 0, sizeof(STARTUPINFO));
	PROCESS_INFORMATION processInfo;

	WCHAR secondArg[] = L"C:\\WINDOWS\\system32\\notepad.exe aaa.txt";

	// notepad will run as child process
	//if (CreateProcess(L"C:\\WINDOWS\\system32\\notepad.exe", NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL,
	if (CreateProcess(NULL, secondArg, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL,
		&startupInfo, &processInfo))
	{
		cout << "PID " << processInfo.dwProcessId << endl;
		cout << "TID =" << processInfo.dwThreadId << endl;
		cout << "PROCESS HANDLE = " << processInfo.hProcess << endl;
		cout << "THREAD HANDLE = " << processInfo.hThread << endl;

		
		// SECOND PROCESS
		STARTUPINFO startupInfo2;
		memset(&startupInfo2, 0, sizeof(STARTUPINFO));
		PROCESS_INFORMATION processInfo2;

		WCHAR secondArg2[] = L"C:\\WINDOWS\\system32\\mspaint.exe";

		// Creating MS Paint process
		if (CreateProcess(NULL, secondArg2, NULL, NULL, FALSE, NULL, NULL, NULL,
			&startupInfo2, &processInfo2) == TRUE)
		{
			cout << "PID " << processInfo2.dwProcessId << endl;
			cout << "TID =" << processInfo2.dwThreadId << endl;
			cout << "PROCESS HANDLE = " << processInfo2.hProcess << endl;
			cout << "THREAD HANDLE = " << processInfo2.hThread << endl;

			//DWORD d2 = WaitForSingleObject(processInfo2.hProcess, /*INFINITE*/ 10000);
			//if (d2 == WAIT_TIMEOUT)
			//{
			//	cout << "TIME OUT\n";
			//}
			//else
			//{
			//	cout << "MS Paint has finished his work!\n";
			//}
			HANDLE h[2];
			h[0] = processInfo.hProcess;
			h[1] = processInfo2.hProcess;
			DWORD waitResult = WaitForMultipleObjects(2, h, TRUE /* If TRUE - waiting for ALL to terminate, if FALSE - waiting for ANY to terminate */, 100000 /*100 seconds*/);
			if (waitResult == 0)
			{
				cout << "Notepad is closed\n";
			}
			else if (waitResult == 1)
			{
				cout << "MS Paint is closed\n";
			}
			else if (waitResult == WAIT_TIMEOUT)
			{
				cout << "TIME OUT\n";
			}


			CloseHandle(processInfo2.hProcess);
		}

		//Sleep(5000); // 5 sec
		//
		//TerminateProcess(processInfo.hProcess, 1); // close/kill notepad.exe
		//CloseHandle(processInfo.hProcess); // do not close notepad.exe, instead it closes access for us
		//CloseHandle(processInfo.hThread);
		//
		//DWORD exitCode;
		//GetExitCodeProcess(processInfo.hProcess, &exitCode);
		//cout << "Exit code: " << exitCode << endl;
		//if (exitCode == STILL_ACTIVE)
		//{
		//	cout << "STILL_ACTIVE\n";
		//}

		//DWORD d = WaitForSingleObject(processInfo.hProcess, /*INFINITE*/ 10000);
		//if (d == WAIT_TIMEOUT)
		//{
		//	cout << "TIME OUT\n";
		//}
		//else
		//{
		//	cout << "Notepad has finished his work!\n";
		//}
		CloseHandle(processInfo.hProcess);
	}
	else
	{
		//cout << "ERROR\n";
		ExitProcess(0);
	}

	system("PAUSE");
	//return 0;
	ExitProcess(0);
}
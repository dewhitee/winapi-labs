﻿// Вариант 2.
// 1. Написать программу SLAVE, которая выводит на экран содержимое текстового файла,
// имя которого передается ей в параметре.Если параметр не передан, вывести об этом
// сообщение и закончить программу.Если программа запущена из процесса MASTER, вывести
// на экран уникальный идентификатор приложения MASTER.Если программа запущена сама
// по себе вывести об этом информацию.
// 2. Написать приложение MASTER которое :
// а) получает в качестве параметра имя исполняемого файла и запускает данное
// приложение, указанное в качестве параметра;
// б) если параметр не передан, то находит заданный в программе текстовый файл,
// запускает приложение SLAVE и передает в качестве параметров имя текстового файла
// и уникальный идентификатор приложения MASTER.При завершении программы
// SLAVE завершается MASTER
// 3. Продемонстрировать совместную работу двух приложений.Для этого на экран должна
// выводиться достаточно подробная информация о ходе выполнения каждого из процессов.

#include <iostream>
#include <thread>
#include <windows.h>
#include <system_error>
#include <string>
#include <locale>
#include <codecvt>
#include <algorithm>

// Deniss Belovs 4801BD

using namespace std;

wstring GetExecDirPath();
wstring GetDefaultTxtFilePath();
wstring AsCommand(wstring execPath, wstring filePath);
VOID CreateDefaultTxtFileChecked();
wstring StringToWstring(string str);
VOID StartProcess(wstring command, LPSTARTUPINFO startupInfo, LPPROCESS_INFORMATION processInfo);
VOID ShowProcessTimeInfo(HANDLE hProcess);

int main(int argc, char** argv)
{
	wcout << "--- MASTER INFO ---\n";
	DWORD dwExitCode;
	STARTUPINFO startupInfo;
	ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
	PROCESS_INFORMATION processInfo;

	wstring slaveExecPath = GetExecDirPath() + wstring(L"WinApiLab6_Slave");
	wcout << "Slave exec path: " << slaveExecPath << endl;
	wcout << "PID: " << GetCurrentProcessId() << ", TID: " << GetCurrentThreadId() << endl;

	if (argc < 2)
	{
		wcout << "\nNot enough arguments - absolute path to .txt file is required => Starting default SLAVE process.\n";

		CreateDefaultTxtFileChecked();
		StartProcess(AsCommand(slaveExecPath, GetDefaultTxtFilePath()), &startupInfo, &processInfo);
		wcout << "Stopping MASTER execution...\n";
		return 0;
	}

	StartProcess(AsCommand(slaveExecPath, StringToWstring(argv[1])), &startupInfo, &processInfo);
	wcout << "Stopping MASTER execution...\n";
	return 0;
}

wstring GetExecDirPath()
{
	WCHAR path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);
	wstring execPath = path;
	return execPath.substr(0, execPath.size() - wcslen(L"WinApiLab6_Master.exe"));
}

wstring GetDefaultTxtFilePath()
{
	return GetExecDirPath() + L"default_file.txt";
}

wstring AsCommand(wstring execPath, wstring filePath)
{
	return execPath + L" " + filePath + L" " + to_wstring(GetCurrentProcessId());
}

VOID CreateDefaultTxtFileChecked()
{
	wcout << "\n--- Creating default txt file ---\n";

	HANDLE hFile = CreateFile(GetDefaultTxtFilePath().c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
		wcout << "Default txt file has been successfully created at the " << GetDefaultTxtFilePath() << " path.\n";
	else
		wcout << "Error while trying to create default txt file.\n";

	SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	char defaultText[] = "There is some default text.";
	for (char c : defaultText)
	{
		DWORD bytesWritten;
		WriteFile(hFile, &c, sizeof(char), &bytesWritten, NULL);
	}

	CloseHandle(hFile);
}

wstring StringToWstring(string str)
{
	return wstring_convert<codecvt_utf8<WCHAR>>().from_bytes(str);
}

VOID StartProcess(wstring command, LPSTARTUPINFO startupInfo, LPPROCESS_INFORMATION processInfo)
{
	wcout << "\n--- Starting new process ---\n";
	wcout << "Command passed: " << command << endl;
	if (CreateProcess(NULL, const_cast<LPWSTR>(command.c_str()), NULL, NULL, FALSE, NULL, NULL, NULL, startupInfo, processInfo))
	{
		DWORD waitTime = 10000;
		wcout << "\nNew process info: \n";
		wcout << "\tPID: " << processInfo->dwProcessId << ", TID: " << processInfo->dwThreadId << ", waitTime: " << waitTime * 0.001 << " seconds" << endl;

		DWORD waitResult = WaitForSingleObject(processInfo->hProcess, waitTime);
		if (waitResult == 0)
		{
			cout << "\nwaitResult: SLAVE is closed\n\n";
		}
		else if (waitResult == WAIT_TIMEOUT)
		{
			cout << "\nwaitResult: TIMEOUT\n\n";
		}
		else
		{
			cout << "\nwaitResult: ERROR\n\n";
		}
		ShowProcessTimeInfo(processInfo->hProcess);
		CloseHandle(processInfo->hProcess);
	}
	else
	{
		cout << "Something went wrong while trying to start new process.\n";
		cout << "ERROR: " << system_category().message(GetLastError()) << endl;
	}
}

VOID ShowProcessTimeInfo(HANDLE hProcess)
{
	FILETIME creationFileTime, exitTime, kernelTime, userTime;

	GetProcessTimes(hProcess, &creationFileTime, &exitTime, &kernelTime, &userTime);

	if (exitTime.dwLowDateTime > 0)
	{
		ULARGE_INTEGER start, end;
		start.LowPart = creationFileTime.dwLowDateTime;
		start.HighPart = creationFileTime.dwHighDateTime;
		end.LowPart = exitTime.dwLowDateTime;
		end.HighPart = exitTime.dwHighDateTime;

		double deltaSeconds = (end.QuadPart - start.QuadPart) / 1e7;

		wcout << "Process was running for " << deltaSeconds << " seconds.\n";
	}
}

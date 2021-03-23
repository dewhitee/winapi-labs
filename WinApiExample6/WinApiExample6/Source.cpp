#include <Windows.h>
#include <iostream>

using namespace std;

int main()
{
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);

	//LPSYSTEM_INFO systemInfo = new SYSTEM_INFO; // SYSTEM_INFO *systemInfo;
	//GetSystemInfo(systemInfo);

	cout << "Number of processors > " << systemInfo.dwNumberOfProcessors << endl;
	cout << "Type of processor " << systemInfo.dwProcessorType << endl;
	//cout << "Number of processors > " << systemInfo->dwNumberOfProcessors << endl;
	//delete systemInfo;


	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	cout << systemTime.wHour << ":" << systemTime.wMinute << ". " << systemTime.wDayOfWeek << endl;

	// w = WORD (2 bytes - short unsigned int)

	DWORD drives = GetLogicalDrives();
	cout << (drives & 8 ? "D do exist" : "D do NOT exist") << endl;
	cout << (drives & 16 ? "E do exist" : "E do NOT exist") << endl;
	cout << (drives & 32 ? "F do exist" : "F do NOT exist") << endl;

	cout << GetLogicalDrives() << endl;

	// To show all drives available - run for loop with mask for 32 bit with 'A' + i (to get char)

	//     svk  bs  fk  k
	DWORD d1, d2, d3, d4;
	if (GetDiskFreeSpace(L"c:\\", &d1, &d2, &d3, &d4))
	{
		cout << d1 << " " << d2 << " " << d3 << " " << d4 << endl;
	}

	// Get overall capacity of C drive
	UINT64 overallBytes = (UINT64)d4 * (UINT64)d1 * (UINT64)d2;
	cout << overallBytes << endl;


	wchar_t dir[256];
	DWORD size = GetCurrentDirectory(256, dir);
	cout << "Size of directory: " << size << ", dir = " << dir << endl;

	HANDLE hFile = CreateFile(L"a.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		cout << "Ok\nhFile = " << hFile << endl;
		FILETIME creationTime, lastAccessTime, lastWriteTime;
		if (GetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime))
		{
			// LOW date time and HIGH date time are used because we need 64 bit and one DWORD is only 32 bit
			// Количество десятитысячных долей миллисекунды которые прошли с 01.01.1601 даты
			// Contains a 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601 (UTC). (msdn)
			// FILETIME is useful if we want to compare times
			cout << "Creation time: " << creationTime.dwHighDateTime << " " << creationTime.dwLowDateTime << endl;
			cout << "Last access time: " << lastAccessTime.dwHighDateTime << " " << lastAccessTime.dwLowDateTime << endl;
			cout << "Last write time: " << lastWriteTime.dwHighDateTime << " " << lastWriteTime.dwLowDateTime << endl;

			SYSTEMTIME systemTime;
			FileTimeToSystemTime(&creationTime, &systemTime);
			cout << systemTime.wHour << ":" << systemTime.wMinute << ":" << systemTime.wSecond << ":" << systemTime.wMilliseconds << endl
				<< systemTime.wDay << "/" << systemTime.wMonth << "/" << systemTime.wYear << endl;

			FILETIME newCreationTime;
			newCreationTime.dwHighDateTime = 2000000000; // 31 max
			newCreationTime.dwLowDateTime = 4000000000;

			/// Not working atm
			if (SetFileTime(hFile, &newCreationTime, &lastAccessTime, &lastWriteTime))
			{
				cout << "\nOK\n";
			}
			else
			{
				cout << "\nERROR\n";
			}
			///cout << "high = " << newTime.dwHighDateTime << ", low = " << newTime.dwLowDateTime << endl;
		}
		CloseHandle(hFile);
	}
	else
	{
		cout << "Error!\n";
	}

	// FindFirstFile, FindNextFile - searches ONLY in the current directory - not in the inner directories.
	// Regex * - multiple any char, ? - single any char
	WIN32_FIND_DATA fd;
	HANDLE hFindFile = FindFirstFile(L"*", &fd);
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		cout << "\nFile is not found\n";
	}
	else
	{
		wcout << "\nFile name: " << fd.cFileName << " ";
		cout << (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? "\tDIRECTORY " : "\tFILE ");
		cout << (fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ? "HIDDEN\n" : "NOT HIDDEN\n");

		// Iterate over all files
		while (FindNextFile(hFindFile, &fd))
		{
			wcout << "File name: " << fd.cFileName << " ";
			cout << (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? "\tDIRECTORY " : "\tFILE ");
			cout << (fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ? "HIDDEN\n" : "NOT HIDDEN\n");
		}
		FindClose(hFindFile);
	}

	// Creating file
	HANDLE hFile2 = CreateFile(L"numbs.dat", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile2 == INVALID_HANDLE_VALUE)
	{
		cout << "Access denied. Error\n";
	}
	else
	{
		// Writing to file
		SetFilePointer(hFile2, 0, 0, FILE_END);
		DWORD numberOfWrittenBytes;
		for (int i = 65; i < 75; i++)
		{
			if (!WriteFile(hFile2, &i, sizeof(int), &numberOfWrittenBytes, NULL))
			{
				cout << "Can't write. Error\n";
			}
			cout << numberOfWrittenBytes << " bytes have written'\n";
		}
		// Reading from file
		SetFilePointer(hFile2, 0, 0, FILE_BEGIN);
		DWORD sizeInBytes = GetFileSize(hFile2, NULL);
		//sizeInBytes = sizeInBytes / sizeof(int);
		cout << "size = " << sizeInBytes << endl;
		cout << "Reading:\n";
		for (int i = 0; i < sizeInBytes; i++)
		{
			int currentValue;
			char currentByte;
			//if (ReadFile(hFile2, &currentValue, sizeof(int), &numberOfWrittenBytes, NULL))
			//{
			//	cout << currentValue << endl;
			//}
			if (ReadFile(hFile2, &currentByte, sizeof(char), &numberOfWrittenBytes, NULL))
			{
				cout << currentByte << endl;
			}
		}
		CloseHandle(hFile2);
	}

	system("PAUSE");
	return 0;
}
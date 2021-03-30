#include <Windows.h>
#include <iostream>
#include <lmcons.h>
#include <bitset>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

VOID PrintFreeSpaceOf(CONST WCHAR disk[]);
VOID PrintDiskType(CONST WCHAR disk[]);
VOID Separator();

int main()
{
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);

	// Computer name
	WCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD computerNameSize;

	GetComputerName(computerName, &computerNameSize);
	wcout << "Computer name: " << computerName << endl;
	Separator();

	// Time
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	TIME_ZONE_INFORMATION timeZoneInfo;
	GetTimeZoneInformation(&timeZoneInfo);

	wcout << "Local time: " << systemTime.wHour 
		<< ":" << systemTime.wMinute 
		<< ":" << systemTime.wSecond
		<< "\n" << systemTime.wDay << "/" << systemTime.wMonth << "/" << systemTime.wYear
		<< "\nDay of week: " << systemTime.wDayOfWeek 
		<< endl;
	wcout << "Time zone: (STANDARD) " << timeZoneInfo.StandardName << ", (DAYLIGHT) " << timeZoneInfo.DaylightName << endl;
	Separator();

	// Language
	LANGID userDefaultLangId = GetUserDefaultLangID();
	CONST CHAR langSize = 32;
	WCHAR language[langSize];
	VerLanguageName(userDefaultLangId, language, langSize);

	// Username
	WCHAR userName[UNLEN];
	DWORD userNameBuffer;
	GetUserName(userName, &userNameBuffer);

	// Current directory
	CONST DWORD currentDirectoryBuffLen = 64;
	WCHAR currentDirectoryBuffer[currentDirectoryBuffLen];
	GetCurrentDirectory(currentDirectoryBuffLen, currentDirectoryBuffer);
	wcout << "Current directory: " << currentDirectoryBuffer << endl;

	// Windows directory
	WCHAR windowsDirectoryBuffer[MAX_PATH];
	GetWindowsDirectory(windowsDirectoryBuffer, MAX_PATH);
	wcout << "Windows directory: " << windowsDirectoryBuffer << endl;

	// System directory
	WCHAR systemDirectoryBuffer[MAX_PATH];
	GetSystemDirectory(systemDirectoryBuffer, MAX_PATH);
	wcout << "System directory: " << systemDirectoryBuffer << endl;

	// Logical drives
	DWORD drives = GetLogicalDrives();
	vector<CHAR> logicalDrivesSymbols;
	cout << "Logical drives: (" << drives << ") " << bitset<32>(drives) << endl;
	CONST UINT mask = 1;
	for (CHAR i = 0; i < 32; i++)
	{
		if (drives & (mask << i))
		{
			CHAR symbol = 'A' + i;
			cout << symbol << " - exists\n";
			logicalDrivesSymbols.push_back(symbol);
		}
	}

	// Logical drives strings
	CONST USHORT logicalDriveStringsLength = 1024;
	WCHAR logicalDriveStringsBuffer[logicalDriveStringsLength];
	GetLogicalDriveStrings(logicalDriveStringsLength, logicalDriveStringsBuffer);

	// Drive type, Disk free space
	for (CHAR symbol : logicalDrivesSymbols)
	{
		Separator();
		//WCHAR diskSymbol[] = L"" + symbol + ":\\";
		WCHAR diskSymbol[32];
		swprintf_s(diskSymbol, L"%c:\\", symbol);

		cout << "Disk: " << (CHAR)symbol << endl;
		cout << "Disk type: ";
		PrintDiskType(diskSymbol);
		cout << "Free space: " << endl;
		PrintFreeSpaceOf(diskSymbol);
	}

	system("PAUSE");
	return 0;
}

VOID PrintFreeSpaceOf(CONST WCHAR disk[])
{
	DWORD sectorsPerCluster, bytesPerSector, freeClusters, clusters;
	if (GetDiskFreeSpace(disk, &sectorsPerCluster, &bytesPerSector, &freeClusters, &clusters))
	{
		cout << "SVK (Sectors per cluster): " << sectorsPerCluster 
			<< ",\nBS (Bytes per sector): " << bytesPerSector 
			<< ",\nFK (Total available free clusters): " << freeClusters 
			<< ",\nK (Total available clusters): " << clusters << endl;

		UINT64 overallBytes = (UINT64)clusters * (UINT64)sectorsPerCluster * (UINT64)bytesPerSector;
		cout << "\nOverall bytes: " << overallBytes << " bytes = " 
			<< fixed << setprecision(2) << overallBytes * 0.001 << "KB = "
			<< fixed << setprecision(2) << overallBytes * 0.00001 << "MB = "
			<< fixed << setprecision(2) << overallBytes * 0.000000001 << "GB" << endl;
	}
	else
	{
		cout << "Error while trying to get disk free space!\n";
	}
}
VOID PrintDiskType(CONST WCHAR disk[])
{
	CONST UINT driveType = GetDriveType(disk);
	switch (driveType)
	{
	case DRIVE_UNKNOWN:
		cout << "The drive type cannot be determined.\n";
		break;
	case DRIVE_NO_ROOT_DIR:
		cout << "The root path is invalid.\n";
		break;
	case DRIVE_REMOVABLE:
		cout << "The drive has removable media.\n";
		break;
	case DRIVE_FIXED:
		cout << "The drive has fixed media.\n";
		break;
	case DRIVE_REMOTE:
		cout << "The drive is a remote (network) drive.\n";
		break;
	case DRIVE_CDROM:
		cout << "The drive is a CD-ROM drive.\n";
		break;
	case DRIVE_RAMDISK:
		cout << "The drive is a RAM disk.\n";
		break;
	default:
		cout << "Something went wrong while trying to identify disk type.\n";
		break;
	}
}
VOID Separator()
{
	cout << "\n-------------------------------------\n";
}

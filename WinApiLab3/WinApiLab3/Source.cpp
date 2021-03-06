#include <Windows.h>
#include <iostream>
#include <lmcons.h>
#include <bitset>
#include <vector>
#include <string>
#include <iomanip>

// Deniss Belovs 4801BD

using namespace std;

VOID PrintComputerName();
VOID PrintTimeInfo();
VOID PrintLanguageInfo();
VOID PrintUserName();
VOID PrintCurrentDirectory();
VOID PrintWindowsDirectory();
VOID PrintSystemDirectory();
VOID PrintDrives();
vector<wstring> GetVectorOfLogicalDriveStrings();
VOID PrintFreeSpaceOf(CONST WCHAR disk[]);
VOID PrintAsSpace(const char* title, UINT64 bytes);
VOID PrintDiskType(CONST WCHAR disk[]);
VOID Separator();

int main()
{
	// Computer name
	PrintComputerName();

	// Time
	PrintTimeInfo();

	// Language
	PrintLanguageInfo();

	// Username
	PrintUserName();

	// Current directory
	PrintCurrentDirectory();

	// Windows directory
	PrintWindowsDirectory();

	// System directory
	PrintSystemDirectory();
	Separator();

	// Logical drives
	PrintDrives();

	system("PAUSE");
	return 0;
}

VOID PrintComputerName()
{
	WCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD computerNameSize;

	GetComputerName(computerName, &computerNameSize);
	wcout << "Computer name: " << computerName << endl;
}

VOID PrintTimeInfo()
{
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	TIME_ZONE_INFORMATION timeZoneInfo;
	GetTimeZoneInformation(&timeZoneInfo);

	WCHAR previousFill = wcout.fill('0');
	wcout << "Local time: " << setw(2) << systemTime.wHour
		<< ":" << setw(2) << systemTime.wMinute
		<< ":" << setw(2) << systemTime.wSecond
		<< " " << setw(2) << systemTime.wDay << "/"
		<< setw(2) << systemTime.wMonth << "/"
		<< setw(2) << systemTime.wYear
		<< "\nDay of week: " << systemTime.wDayOfWeek
		<< endl;
	wcout << "Time zone: (STANDARD) " << timeZoneInfo.StandardName << ", (DAYLIGHT) " << timeZoneInfo.DaylightName << endl;
	float bias = (timeZoneInfo.Bias / 60.f) * -1.f; // float cause there are also half-hour time zones
	wcout << "Coordinated Universal Time (UTC): UTC" << (bias >= 0 ? '+' : '-') << bias << endl;
	wcout.fill(previousFill);
}

VOID PrintLanguageInfo()
{
	LANGID userDefaultLangId = GetUserDefaultLangID();
	CONST CHAR langSize = 32;
	WCHAR language[langSize];
	VerLanguageName(userDefaultLangId, language, langSize);
	wcout << "Language: " << language << endl;
}

VOID PrintUserName()
{
	WCHAR userName[UNLEN];
	DWORD userNameBuffer;
	GetUserName(userName, &userNameBuffer);
	wcout << "UserName: " << userName << endl;
}

VOID PrintCurrentDirectory()
{
	CONST DWORD currentDirectoryBuffLen = 64;
	WCHAR currentDirectoryBuffer[currentDirectoryBuffLen];
	GetCurrentDirectory(currentDirectoryBuffLen, currentDirectoryBuffer);
	wcout << "Current directory: " << currentDirectoryBuffer << endl;
}

VOID PrintWindowsDirectory()
{
	WCHAR windowsDirectoryBuffer[MAX_PATH];
	GetWindowsDirectory(windowsDirectoryBuffer, MAX_PATH);
	wcout << "Windows directory: " << windowsDirectoryBuffer << endl;
}

VOID PrintSystemDirectory()
{
	WCHAR systemDirectoryBuffer[MAX_PATH];
	GetSystemDirectory(systemDirectoryBuffer, MAX_PATH);
	wcout << "System directory: " << systemDirectoryBuffer << endl;
}

VOID PrintDrives()
{
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
	auto logicalDriveStrings = GetVectorOfLogicalDriveStrings();

	// Drive type, Disk space
	for (wstring s : logicalDriveStrings)
	{
		Separator();
		wcout << "Disk: " << s << "\n --- Disk type: ";
		PrintDiskType(s.c_str());
		cout << " --- Disk space:\n";
		PrintFreeSpaceOf(s.c_str());
	}
}

vector<wstring> GetVectorOfLogicalDriveStrings()
{
	CONST USHORT length = 1024;
	WCHAR logicalDriveStringsBuffer[length];
	GetLogicalDriveStrings(length, logicalDriveStringsBuffer);

	const WCHAR* ptr = logicalDriveStringsBuffer;
	vector<wstring> logicalDriveStrings;
	wcout << "\nLogical drive strings:\n";
	while (*ptr)
	{
		logicalDriveStrings.push_back(wstring(ptr));
		wcout << logicalDriveStrings.back() << endl;
		ptr += logicalDriveStrings.back().size() + 1;
	}
	return logicalDriveStrings;
}

VOID PrintFreeSpaceOf(CONST WCHAR disk[])
{
	DWORD sectorsPerCluster, bytesPerSector, freeClusters, totalClusters;
	if (GetDiskFreeSpace(disk, &sectorsPerCluster, &bytesPerSector, &freeClusters, &totalClusters))
	{
		cout << "Sectors per cluster: " << sectorsPerCluster 
			<< ",\nBytes per sector: " << bytesPerSector 
			<< ",\nTotal available free clusters: " << freeClusters 
			<< ",\nTotal available clusters: " << totalClusters << endl;

		UINT64 freeBytes = (UINT64)freeClusters * (UINT64)sectorsPerCluster * (UINT64)bytesPerSector;
		UINT64 overallBytes = (UINT64)totalClusters * (UINT64)sectorsPerCluster * (UINT64)bytesPerSector;
		PrintAsSpace("\nFree bytes: ", freeBytes);
		PrintAsSpace("Occupied bytes: ", overallBytes - freeBytes);
		PrintAsSpace("Overall bytes: ", overallBytes);
	}
	else
	{
		cout << "Error while trying to get disk free space!\n";
	}
}
VOID PrintAsSpace(const char* title, UINT64 bytes)
{
	if (bytes > 0)
	{
		cout << title << bytes << " bytes = "
			<< fixed << setprecision(2) << bytes / 1024.0 << "KB = "
			<< fixed << setprecision(2) << bytes / 1024.0 / 1024.0 << "MB = "
			<< fixed << setprecision(2) << bytes / 1024.0 / 1024.0 / 1024.0 << "GB" << endl;
	}
	else
	{
		cout << "Error! Disk is full!" << endl;
	}
}
VOID PrintDiskType(CONST WCHAR disk[])
{
	CONST UINT driveType = GetDriveType(disk);
	switch (driveType)
	{
	// Note: All text is from the msdn page
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

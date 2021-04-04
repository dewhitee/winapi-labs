#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <iomanip>

// Deniss Belovs 4801BD

using namespace std;

struct FoundFile
{
	wstring fileName;
	SYSTEMTIME creationTime;
};

vector<FoundFile> FindFiles(const WCHAR* fileExtension = L".txt", UINT8 filesCount = 10);
wstring ShowFindData(const WIN32_FIND_DATA& fd);
SYSTEMTIME GetCreationTime(const WIN32_FIND_DATA& fd);
VOID CreateFindDirectory();
HANDLE CreateFindFile();
VOID WriteAllFoundTxtFilesData(HANDLE hFile, vector<FoundFile>& foundFiles);
VOID PrintFindFileContents(HANDLE hFile);
VOID PrintCreationTime(FILETIME creationTime);
VOID PrintCreationTime(SYSTEMTIME creationTime);

int main()
{
	auto foundFiles = FindFiles();
	CreateFindDirectory();
	HANDLE hFile = CreateFindFile();
	WriteAllFoundTxtFilesData(hFile, foundFiles);
	PrintFindFileContents(hFile);
	system("pause");
	return 0;
}

vector<FoundFile> FindFiles(const WCHAR* fileExtension, UINT8 filesCount)
{
	vector<FoundFile> foundFiles;
	WIN32_FIND_DATA fd;
	WCHAR findPath[100];
	swprintf_s(findPath, L"C:\\FIND\\*%s", fileExtension);
	wcout << "\n --- Searching for " << fileExtension << " files...\n\n";
	wcout << "File findPath = " << findPath << endl << endl;
	cout << "Files found: \n";
	HANDLE hFindFile = FindFirstFile(findPath, &fd);
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		cout << "\nNo file has been found.\n";
	}
	else
	{
		do
		{
			wstring fileName = ShowFindData(fd);
			SYSTEMTIME creationTime = GetCreationTime(fd);
			PrintCreationTime(creationTime);
			foundFiles.push_back(FoundFile{ fileName, creationTime });
		} 
		while (FindNextFile(hFindFile, &fd));

		FindClose(hFindFile);
	}
	return foundFiles;
}

wstring ShowFindData(const WIN32_FIND_DATA& fd)
{
	wcout << "File name: " << fd.cFileName << " ";
	cout << (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? "\tDirectory " : "\tFile ");
	cout << (fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ? "Hidden | " : "Not hidden | ");
	return fd.cFileName;
}

SYSTEMTIME GetCreationTime(const WIN32_FIND_DATA& fd)
{
	SYSTEMTIME creationSystemTime;
	FileTimeToSystemTime(&fd.ftCreationTime, &creationSystemTime);
	//PrintCreationTime(creationSystemTime);
	return creationSystemTime;
}

VOID CreateFindDirectory()
{
	cout << "\n --- Creating FIND directory...\n\n";
	if (CreateDirectory(L"C:\\FIND", NULL))
	{
		cout << "FIND directory has been created.\n";
	}
	else
	{
		switch (GetLastError())
		{
		case ERROR_ALREADY_EXISTS:
			cout << "The specified directory already exists.\n";
			return;
		case ERROR_PATH_NOT_FOUND:
			cout << "One or more intermediate directories do not exist.\n";
			return;
		}
	}
}

HANDLE CreateFindFile()
{
	cout << "\n --- Creating find.dat...\n\n";
	HANDLE hFile = CreateFile(L"C:\\FIND\\find.dat", GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	int fileSize = GetFileSize(hFile, NULL);
	if (!fileSize)
	{
		cout << "File is empty!\n";
	}
	else
	{
		cout << "File is not empty!\n";
		//ClearFindFile(hFile);
		cout << "File size = " << fileSize << endl;
	}

	return hFile;
}

VOID WriteAllFoundTxtFilesData(HANDLE hFile, vector<FoundFile>& foundFiles)
{
	SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	cout << "\n --- Writing to find.dat...\n\n";
	for (auto&& f : foundFiles)
	{
		//wcout << "Sizeof(buffer) = " << sizeof(FoundFile) << endl;
		DWORD numberOfWrittenBytes;
		WriteFile(hFile, &f, sizeof(FoundFile), &numberOfWrittenBytes, NULL);
		wcout << "Wrote: " << &f << " to the find.dat "
			<< "| FileName: " << f.fileName << ", ";
		PrintCreationTime(f.creationTime);
		//wcout << numberOfWrittenBytes << " bytes have written and the current find.dat fileSize is "
		//	<< GetFileSize(hFile, NULL) << endl;
	}
}

VOID PrintFindFileContents(HANDLE hFile)
{
	DWORD fileSize = GetFileSize(hFile, NULL);
	SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	if (!fileSize)
	{
		cout << "\n --- File is empty!\n\n";
	}
	else
	{
		cout << "\n --- Reading find.dat...\n\n";
		DWORD size = fileSize / sizeof(FoundFile);
		wcout << "FileSize = " << fileSize << ", size = " << size << endl;
		FoundFile* foundFiles = new FoundFile[size];
		DWORD bytesRead;
		ReadFile(hFile, foundFiles, sizeof(FoundFile) * size, &bytesRead, NULL);
		for (int i = 0; i < size; i++)
		{
			wcout << i << "-th file: " << foundFiles[i].fileName << setw(10) << " ";
			PrintCreationTime(foundFiles[i].creationTime);
		}
		wcout << "bytes read: " << bytesRead << endl << endl;
		CloseHandle(hFile);
	}
}

VOID PrintCreationTime(FILETIME creationTime)
{
	wcout << "Creation FILETIME: "
		<< creationTime.dwHighDateTime << " " << creationTime.dwLowDateTime << endl;
}

VOID PrintCreationTime(SYSTEMTIME creationTime)
{
	char previousFill = wcout.fill('0');
	wcout << "Creation time: "
		<< setw(2) << creationTime.wHour << ":" << setw(2) << creationTime.wMinute << ":" << setw(2) << creationTime.wSecond << " "
		<< setw(2) << creationTime.wDay << "/" << setw(2) << creationTime.wMonth << "/" << setw(2) << creationTime.wYear << endl;
	wcout.fill(previousFill);
}

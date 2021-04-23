// 2. Программно реализовать поиск 10 файлов с расширением . txt . Создать на с:\ каталог FIND , внутри каталога создать файл find.dat. 
// Записать в данный файл полные имена всех найденных файлов и даты создания. Реализовать функцию просмотра содержания файла.

#include <Windows.h>
#include <iostream>
#include <vector>
#include <io.h>
#include <fcntl.h>

using namespace std;


struct SearchFile
{
	wstring filename = wstring();
	SYSTEMTIME creationTime = SYSTEMTIME();
};

vector<SearchFile> get_Files(CONST WCHAR path[]);
VOID search();

int main()
{
	_setmode(_fileno(stdout), _O_U16TEXT);


	int userInput;
	do {
		search();
		wcout << L"Again? 0 to exit > "; wcin >> userInput;
		wcin.ignore();
		system("cls");
	} while (userInput);

	system("PAUSE");
	return 0;
}

vector<SearchFile> get_Files(CONST WCHAR path[])
{
	WIN32_FIND_DATA fd;

	wstring modifiedPath = path;
	if (path[wcslen(path) - 1] == L'\\') {
		modifiedPath = modifiedPath + L"*.txt";
	}
	else {
		modifiedPath = modifiedPath + L"\\*.txt";
	}
	HANDLE hFile = FindFirstFile(modifiedPath.c_str(), &fd);
	vector<SearchFile> filesFound;

	if (hFile != INVALID_HANDLE_VALUE) {
		int count = 0;
		do {
			SYSTEMTIME creationTime;
			FileTimeToSystemTime(&fd.ftCreationTime, &creationTime);
			wcout << count << L": " << fd.cFileName << L" " << creationTime.wHour << L":" << creationTime.wMinute <<
				L":" << creationTime.wSecond << L" " << creationTime.wDay << L"/" << creationTime.wMonth <<
				L"/" << creationTime.wYear << endl;

			SearchFile sf;
			sf.creationTime = creationTime;
			sf.filename = fd.cFileName;

			filesFound.push_back(sf);
			count++;

		} while (FindNextFile(hFile, &fd) && count < 10);
	}
	else {
		wcout << L"INVALID_HANDLE_VALUE" << endl;
	}
	FindClose(hFile);
	return filesFound;
}

VOID search()
{
	WCHAR searchPath[260];
	wcout << L"Please, enter path for search > ";
	wcin.getline(searchPath, 260);

	wcout << L"Searching in " << searchPath << endl;
	vector<SearchFile> filesFound = get_Files(searchPath);

	wcout << endl << L"Creating FIND directory on C drive" << endl;
	if (CreateDirectory(L"C:\\FIND", NULL)) {
		wcout << L"FIND directory is created succesfully" << endl;
	}
	else {
		wcout << L"Already exist" << endl;
	}

	wcout << endl << L"Creating find.dat file in FIND directory" << endl;
	HANDLE hFindDat = CreateFile(L"C:\\FIND\\find.dat", GENERIC_READ | GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	int fileSize = GetFileSize(hFindDat, NULL);
	if (fileSize == 0) {
		wcout << L"find.dat is created" << endl;
	}

	wcout << endl << L"Writing to find.dat file" << endl;
	SetFilePointer(hFindDat, 0, 0, FILE_BEGIN);
	if (filesFound.size() > 0) {
		for (int i = 0; i < filesFound.size(); i++) {
			DWORD written_bytes;
			WriteFile(hFindDat, &filesFound[i], sizeof(SearchFile), &written_bytes, NULL);
		}
	}
	else {
		wcout << L"No files found" << endl;
	}

	wcout << endl << L"Reading find.dat file" << endl;
	fileSize = GetFileSize(hFindDat, NULL);
	SetFilePointer(hFindDat, 0, 0, FILE_BEGIN);
	if (fileSize == 0) {
		wcout << L"No files found" << endl;
	}
	else {
		DWORD size = fileSize / sizeof(SearchFile);
		wcout << "size=" << size << ", fileSize=" << fileSize << endl;
		SearchFile* files = new SearchFile[size];
		DWORD bytes;
		if (ReadFile(hFindDat, files, sizeof(SearchFile) * size, &bytes, NULL)) {
			for (int i = 0; i < size; i++) {
				wcout << i << L": " << files[i].filename << L" " << files[i].creationTime.wHour << L":" << files[i].creationTime.wMinute <<
					L":" << files[i].creationTime.wSecond << L" " << files[i].creationTime.wDay << L"/" << files[i].creationTime.wMonth <<
					L"/" << files[i].creationTime.wYear << endl;
			}
		}
		else {
			wcout << "Something went wrong" << endl;
		}

	}
	CloseHandle(hFindDat);
}

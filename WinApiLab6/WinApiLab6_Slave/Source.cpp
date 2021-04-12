#include <iostream>
#include <windows.h>

using namespace std;

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		cout << "Not enough arguments\n";
		return 0;
	}

	LPCSTR fileName = argv[1];
	cout << "fileName = " << fileName << endl;
	HANDLE hFile = CreateFileA(fileName, GENERIC_READ, NULL /*FILE_SHARE_READ*/, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(hFile, 0, 0, FILE_BEGIN);
		DWORD sizeInBytes = GetFileSize(hFile, NULL);
		for (int i = 0; i < sizeInBytes; i++)
		{
			BYTE currentByte;
			DWORD numberOfWrittenBytes;
			if (ReadFile(hFile, &currentByte, sizeof(BYTE), &numberOfWrittenBytes, NULL))
			{
				cout << currentByte;
			}
			else
			{
				cout << "Error while reading the file\n";
			}
		}
		cout << endl;
		CloseHandle(hFile);
	}
	else
	{
		cout << "Error while opening the file. Check spelling of a file name\n";
	}

	//system("PAUSE");
	return 0;
}
#include <iostream>
#include <windows.h>

using namespace std;

VOID Separator();

int main(int argc, char** argv)
{
	cout << "\n--- SLAVE INFO ---\n";
	if (argc < 2)
	{
		cout << "Not enough arguments. You need to pass the absolute path to the .txt file.\n";
		return 1;
	}
	else if (argc == 3)
	{
		cout << "SLAVE has been started from MASTER process.\n";
		cout << "MASTER PID: " << argv[2] << endl << endl;
	}
	else
	{
		cout << "SLAVE has been started by itself.\n";
	}

	LPCSTR fileName = argv[1];
	cout << "fileName = " << fileName;
	Separator();
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
		CloseHandle(hFile);
	}
	else
	{
		cout << "Error while opening the file. Check spelling of a file name\n";
		return 1;
	}
	Separator();
	cout << endl;
	system("PAUSE");
	return 0;
}

VOID Separator()
{
	cout << "\n------------------\n";
}

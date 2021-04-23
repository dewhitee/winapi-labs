#include <iostream>
#include <windows.h>

using namespace std;

int main(int argc, char** argv)
{
	/*cout << "argc = " << argc << endl;
	for (int i = 0; i < argc; i++)
	{
		cout << argv[i] << endl;
	}*/

	if (argc == 2)
	{
		HANDLE hFile = CreateFileA(argv[1], GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			cout << "ERROR\n";
		}
		else
		{
			cout << "File is opened\n";

			char c = 'A';
			char space = ' ';
			DWORD bytesRead;
			for (int i = 0; i < 30; i++)
			{
				WriteFile(hFile, &c, sizeof(char), &bytesRead, NULL);
				WriteFile(hFile, &space, sizeof(char), &bytesRead, NULL);
				c++; // B, C, D ...
			}
			
			cout << "File is closed\n";
			CloseHandle(hFile);
		}

		system("pause");
		return 0;
	}
	else
	{
		return 1;
	}
}
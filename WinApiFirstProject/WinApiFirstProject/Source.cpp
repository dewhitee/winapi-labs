#include <iostream>
#include <Windows.h>
#include <string>

using namespace std;

int main()
{
	cout << "start app" << endl;

	HANDLE hFile = CreateFile(L"test.txt", GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE) // FFFFFFFF
		cout << "ERROR\n";
	else
		cout << "hFile= " << hFile << endl;
	CloseHandle(hFile);

	system("pause");
	return 0;
}
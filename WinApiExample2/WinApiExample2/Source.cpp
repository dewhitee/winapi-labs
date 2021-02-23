#include <iostream>
#include <Windows.h>

using namespace std;

int main()
{
	auto fileName = L"a.txt";

	DWORD fileAttributes = GetFileAttributes(fileName);
	cout << "file attributes = " << fileAttributes << endl;

	// Checking if chosen file is readonly
	// FILE_ATTRIBUTE_READONLY = 1
	cout << (fileAttributes & FILE_ATTRIBUTE_READONLY ? "Readonly\n" : "Not readonly\n");

	// Checking if chosen file is hidden
	// FILE_ATTRIBUTE_HIDDEN = 2
	cout << (fileAttributes & FILE_ATTRIBUTE_HIDDEN ? "Hidden\n" : "Not hidden\n");

	// Toggle hidden attribute of chosen file
	fileAttributes ^= FILE_ATTRIBUTE_HIDDEN; // 2(10) = 10(2)

	// Set modified file attributes as the new file attributes of chosen file
	SetFileAttributes(fileName, fileAttributes);

	system("pause");
	return 0;
}
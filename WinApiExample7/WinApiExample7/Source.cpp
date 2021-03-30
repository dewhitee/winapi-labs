#include <windows.h>
#include <iostream>
#include <conio.h>
#include <tchar.h>
#include <ctime>
#include<Shlobj.h>
using namespace std;

enum SortAtr { ASC, DES };

void printArray(int* arr, int arrSize);
int ascComp(const void* a, const void* b);
int desComp(const void* pa, const void* pb);
void regSortSet(SortAtr sortAttr);
SortAtr regSortGet();


int main(int argc, wchar_t** argv)
{

	HANDLE hFile;
	if (argc > 1)
		hFile = CreateFile(argv[1], GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	else
		hFile = CreateFile(L"new_test.mmm", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	int fileSize = GetFileSize(hFile, NULL);
	if (!fileSize)
	{
		cout << "File is empty!\n";
		srand(time(NULL));
		for (int i = 0; i < 10; i++)
		{
			int a = rand() % 100;
			WriteFile(hFile, &a, sizeof(int), NULL, NULL);
		}
		cout << "\nDATA IS ADDED\n";
	}
	int arrSize = fileSize / sizeof(int);
	int* arr = new int[arrSize];
	SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	DWORD rb;
	ReadFile(hFile, arr, fileSize, &rb, NULL);
	CloseHandle(hFile);

	SortAtr sortAttr = regSortGet();
	if (sortAttr == SortAtr::ASC)
		qsort(arr, arrSize, sizeof(int), ascComp);
	else
		qsort(arr, arrSize, sizeof(int), desComp);
	printArray(arr, arrSize);
	int choise;
	do
	{
		cout << "SORTING:\n1.ASC\n2.DES\n0. EXIT\n\n";
		cout << "DO YOUR CHOISE> ";	cin >> choise;
		switch (choise)
		{
		case 1:
			qsort(arr, arrSize, sizeof(int), ascComp);
			printArray(arr, arrSize);
			regSortSet(SortAtr::ASC);
			break;
		case 2:
			qsort(arr, arrSize, sizeof(int), desComp);
			printArray(arr, arrSize);
			regSortSet(SortAtr::DES);
			break;
		case 0: cout << "BUY BUY!\n"; break;
		default: cout << "Error!!!\n";
		}
	} while (choise);

	system("PAUSE");
	return 0;
}

int ascComp(const void* pa, const void* pb)
{
	int a = *(int*)pa;
	int b = *(int*)pb;
	return a - b;
}
int desComp(const void* pa, const void* pb)
{
	int a = *(int*)pa;
	int b = *(int*)pb;
	return b - a;
}
void printArray(int* arr, int arrSize)
{
	cout << "--------------ARRAY IS:-------------\n";
	for (int i = 0; i < arrSize; i++)
		cout << arr[i] << endl;

}
void regSortSet(SortAtr sortAttr)
{
	HKEY hKey; DWORD dwD;
	LONG a = RegCreateKeyEx(HKEY_CURRENT_USER, L"MYAPP",
		0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,
		&dwD);

	a = RegSetValueExA(hKey, "SortAttribute", 0, REG_BINARY, (BYTE*)&sortAttr, sizeof(int));
	RegCloseKey(hKey);
}
SortAtr regSortGet()
{
	SortAtr a; DWORD BufferSize;
	RegGetValueA(HKEY_CURRENT_USER, "MYAPP", "SortAttribute", RRF_RT_ANY, NULL, (PVOID)&a, &BufferSize);
	return a;
}
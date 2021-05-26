#include <iostream>
#include <windows.h>

using namespace std;

// reserve 10 pages
// exception
// commit 1 page 1024 int
// 1025 exception
// commit 1 page 1024 int

int main()
{
	int* a; // begin address
	const int size = 10000;

	int pagenumber = 0;
	a = (int*)VirtualAlloc(
		NULL, 
		10 * 4096, // b
		MEM_RESERVE, 
		PAGE_READWRITE); // reserve 10 pages - address

	if (!a)
	{
		cout << "Virtual allocation failed." << endl;
		system("pause");
		return GetLastError();
	}

	cout << "Virtual memory address: " << a << endl;
	int* b = a;

	for (int i = 0; i < size; i++)
	{
		__try
		{
			*b = i;
			b++;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			cout << "EXCEPTION\n";
			cout << "b = " << b << ", i = " << i << endl;
			b = (int*)VirtualAlloc(b,
				4096,
				MEM_COMMIT,
				PAGE_READWRITE);

			pagenumber++;
			cout << pagenumber << " PAGE ADDRESS = " << b << endl;
			i--;
			system("pause");
		}
	}

	for (int i = 0; i < size; i++)
	{
		cout << *b << " ";
		b++;
	}

	if (!VirtualFree(a, 0, MEM_RELEASE))
	{
		cout << "Memory release failed." << endl;
		system("pause");
		return GetLastError();
	}
	system("pause");
	return 0;
}
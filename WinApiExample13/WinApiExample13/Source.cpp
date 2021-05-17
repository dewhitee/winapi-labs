#include <iostream>
#include <windows.h>

using namespace std;

int main()
{
	//HANDLE hMutex = CreateMutex(NULL, FALSE, "MTX");
	HANDLE hSem = CreateSemaphore(NULL, 3, 3, "SEM");
	cout << GetCurrentProcessId() << endl;
	//cout << "MUTEX IS CREATED\n";
	cout << "SEMAPHORE IS CREATED\n";
	//cout << "WE ARE WAITING A MUTEX\n";
	cout << "WE ARE WAITING A SEM\n";
	//WaitForSingleObject(hMutex, INFINITE);
	WaitForSingleObject(hSem, INFINITE);
	//MessageBox(0, "Acquired mutex", "MUTEX", MB_OK);
	MessageBox(0, "Acquired sem", "SEM", MB_OK);
	//ReleaseMutex(hMutex);
	ReleaseSemaphore(hSem, 1, 0);
	//cout << "MUTEX IS RELEASED\n";
	cout << "SEM IS RELEASED\n";
	//CloseHandle(hMutex);
	CloseHandle(hSem);
	system("pause");
	return 0;
}
#include <iostream>
#include <windows.h>
#include <atomic>

using namespace std;

//atomic<int> n;
int n;
HANDLE hMutex;
CRITICAL_SECTION cs;
//LONG n = 0;

void threadInc()
{
	for (int i = 0; i < 100; i++)
	{
		//EnterCriticalSection(&cs);

		WaitForSingleObject(hMutex, INFINITE);

		///InterlockedIncrement(&n);
		n++; // critical section
		//LeaveCriticalSection(&cs);

		ReleaseMutex(hMutex);
	}
}

void threadDec()
{
	for (int i = 0; i < 100; i++)
	{
		//EnterCriticalSection(&cs);

		WaitForSingleObject(hMutex, INFINITE);

		///InterlockedDecrement(&n);
		n--; // critical section
		//LeaveCriticalSection(&cs);

		ReleaseMutex(hMutex);
	}
}

int main()
{
	//InitializeCriticalSection(&cs);
	hMutex = CreateMutex(NULL, FALSE, NULL);

	for (int i = 0; i < 10000; i++)
	{
		n = 0;

		DWORD tid1, tid2;
		HANDLE hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadInc, NULL, 0, &tid1);
		HANDLE hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadDec, NULL, 0, &tid2);
		HANDLE h[2] = { hThread1, hThread2 };
		//h[0] = hThread1; h[1] = hThread2;
		WaitForMultipleObjects(2, h, TRUE, INFINITE);
		if (n != 0) cout << "n = " << n << endl;

		//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_IDLE);
		//SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
		//for (int i = 0; i < 100; i++)
		//{
		//	cout << "MAIN: " << i << endl;
		//}
		//WaitForSingleObject(hThread, INFINITE);


		CloseHandle(hThread1);
		CloseHandle(hThread2);
	}

	DeleteCriticalSection(&cs);
	system("PAUSE");
	return 0;
}
#include <iostream>
#include <windows.h>

using namespace std;

void thread(int* n)
{
	for (int i = *n; i <= 0; i++)
	{
		cout << "THREAD" << i << endl;
	}
}

int main()
{
	DWORD tid;
	int n = -999;
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread, &n, 0/*CREATE_SUSPENDED*/, &tid);
	for (int i = 0; i < 100; i++)
	{
		//if (i > 50)
		//{
		//	ResumeThread(hThread);
		//}
		cout << "MAIN: " << i << endl;
	}
	CloseHandle(hThread);
	system("PAUSE");
	return 0;
}
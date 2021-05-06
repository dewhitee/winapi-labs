#include <iostream>
#include <thread>
#include <windows.h>

using namespace std;

void Worker();

template<class T>
void BubbleSort(T* arr, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size - i - 1; j++)
		{
			if (arr[j] > arr[j + 1])
				swap(&arr[j], &arr[j + 1]);
		}
	}
}

int main()
{
	DWORD tid;
	int n = -999;
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Worker, &n, 0, &tid);
	//for ()
	return 0;
}

void Worker()
{

}

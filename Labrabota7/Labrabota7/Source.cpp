// 18). Поток worker должен найти значение количество четных элементов массива. Для синхронизации потоков использовать критическую секцию.
#include <thread>
#include <iostream>
#include <windows.h>

using namespace std;

int* a;
int n;
double sum;
double average;
int even_count;

CRITICAL_SECTION cs;

void worker(LPVOID);
bool check(int* test_arr);

int main(){
	srand(time(0));

	InitializeCriticalSection(&cs);

	cout << "Enter array size > "; cin >> n;

	if (n > 0){
		int tests_count = 5/*15000*/;
		// multiple iterations to test CRITICAL_SECTION 
		for (int test_it = 0; test_it < tests_count; test_it++){
			even_count = 0;
			average = 0;
			sum = 0;

			a = new int[n];
			int* test_a = new int[n];
			for (int i = 0; i < n; i++)
			{
				a[i] = rand() % 10 + 1;
				test_a[i] = a[i];
			}

			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)worker, NULL, 0, NULL);

			EnterCriticalSection(&cs);
			// sort array
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n - i - 1; j++)
				{
					if (a[j] > a[j + 1])
						swap(a[j], a[j + 1]);
				}
			}

			// print array
			cout << "Array sorted = ";
			for (int i = 0; i < n; i++)
				cout << a[i] << " ";
			cout << endl;
			LeaveCriticalSection(&cs);

			DWORD wait = WaitForSingleObject(hThread, INFINITE);

			// count elements that are larger than average of 'a' array
			int count = 0;
			for (int i = 0; i < n; i++)
			{
				if (a[i] > average)
					count++;
			}
			cout << "Count of elements larger than average = " << count << endl;
			cout << "Average = " << average << endl;
			cout << "Sum = " << sum << endl;
			cout << "Count of even elements = " << even_count << endl << endl;

			CloseHandle(hThread);

			if (!check(test_a))
				cout << "ERROR! test_it = " << test_it << endl;

			delete[] a;
		}

		DeleteCriticalSection(&cs);
	} else {
		cout << "Size of array need to be more than 0" << endl;
	}

	system("pause");
	return 0;
}

void worker(LPVOID)
{
	// average
	EnterCriticalSection(&cs);
	for (int i = 0; i < n; i++) {
		sum += a[i];
		Sleep(12);
	}
	average = sum / n;

	// count even elenents
	for (int i = 0; i < n; i++){
		if (a[i] % 2 == 0){
			even_count++;
		}
	}
	LeaveCriticalSection(&cs);
}

bool check(int* test_arr) // compare actual and expected values
{
	double test_sum = 0.0;
	for (int i = 0; i < n; i++)
		test_sum += test_arr[i];

	double test_average = test_sum / n;

	int test_even_count = 0;
	for (int i = 0; i < n; i++){
		if (test_arr[i] % 2 == 0)
			test_even_count++;
	}
	return average == test_average && even_count == test_even_count;
}

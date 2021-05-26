// 16). Поток worker должен найти значение факториала элементов массива. Для синхронизации потоков использовать мьютекс.

#include <iostream>
#include <thread>
#include <windows.h>

using namespace std;

HANDLE hMutex;

int* a;
int n;
float avg;
int* fact_arr;

struct TestArgs {
	float test_average;
	int* test_fact_arr;
}

void worker(LPVOID);
void sortArr();
void sortArr(int* arr);
bool check_factorials(int* a_arr, int* b_arr);
TestArgs get_test_args(int* test_arr);

int main() {
	srand(time(NULL));
	hMutex = CreateMutex(NULL, FALSE, NULL);
	while (n < 1){
		cout << "Enter array size: "; cin >> n;
	}	

	for (int i = 0; i < 10; i++) {
		
		avg = 0;
		
		a = new int[n];
		fact_arr = new int[n];

		int* test_arr = new int[n];
		
		for (int j = 0; j < n; j++){
			a[j] = rand() % 10 + 1;
			test_arr[j] = a[j];
		}

		TestArgs test_args = get_test_args(test_arr);

		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)worker, NULL, 0, NULL);
		sortArr();
		
		WaitForSingleObject(hMutex, INFINITE);
		cout << "i: " << i << " ------- \nsorted: ";
		for (int j = 0; j < n; j++) {
			cout << a[j] << " ";
		}
		cout << endl;
		ReleaseMutex(hMutex);

		DWORD result = WaitForSingleObject(hThread, INFINITE);

		int count = 0;
		for (int i = 0; i < n; i++) {
			if (a[i] > avg) {
				count++;
			}
		}
		cout << "avg = " << avg << ", count of > avg: " << count << "\n\n";

		if (avg != test_args.test_average || !check_factorials(fact_arr, test_args.test_fact_arr))
		{
			cout << "\n ------------- ERROR! ---------------\n";
		}

		delete[] a;
		delete[] fact_arr;

		CloseHandle(hThread);
	}
	CloseHandle(hMutex);
	system("pause");
	return 0;
}

void worker(LPVOID lpVoid) {
	// average
	float arr_sum = 0;
	WaitForSingleObject(hMutex, INFINITE);
	for (int i = 0; i < n; i++) {
		arr_sum += a[i];
		Sleep(12);
	}
	avg = arr_sum / n;
	ReleaseMutex(hMutex);

	// factorial
	cout << "factorial array: ";
	WaitForSingleObject(hMutex, INFINITE);
	for (int i = 0; i < n; i++) {
		fact_arr[i] = 1;
		for (int j = 1; j <= a[i]; j++) {
			fact_arr[i] *= j;
		}
		cout << fact_arr[i] << " ";
	}
	ReleaseMutex(hMutex);
	cout << endl;
}

void sortArr() {
	WaitForSingleObject(hMutex, INFINITE);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (a[j] > a[j + 1])
				swap(a[j], a[j + 1]);
		}
	}
	ReleaseMutex(hMutex);
}

void sortArr(int* arr)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (arr[j] > arr[j + 1])
				swap(arr[j], arr[j + 1]);
		}
	}
}

bool check_factorials(int* a_arr, int* b_arr)
{
	for (int i = 0; i < n; i++) {
		if (a_arr[i] != b_arr[i])
			return false;
	}
	return true;
}

TestArgs get_test_args(int *test_arr){
	TestArgs out_args;
	sortArr(test_arr);
	cout << "Sorted test: ";
	for (int j = 0; j < n; j++)
		cout << test_arr[j] << " ";

	// average
	float test_sum = 0.0;
	for (int j = 0; j < n; j++)
		test_sum += test_arr[j];
	float test_average = test_sum / n;

	// factorial
	int *test_fact_arr = new int[n];
	cout << "fact test: ";
	for (int j = 0; j < n; j++)
	{
		test_fact_arr[j] = 1;
		for (int k = 1; k <= test_arr[j]; k++)
			test_fact_arr[j] *= k;
		cout << test_fact_arr[j] << " ";
	}
	cout << endl;

	out_args.test_average = test_average;
	out_args.test_fact_arr = test_fact_arr;
	return out_args;
}

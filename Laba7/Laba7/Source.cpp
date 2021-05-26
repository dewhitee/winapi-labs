//20). Поток worker должен найти значение суммы нечетных элементов массива. Для синхронизации потоков использовать семафор.
#include <iostream>
#include <windows.h>
#include <thread>

int* Array;
int ArraySize;

float ArrayAverage;
float ArraySum;
float OddElementsSum;

HANDLE hSem;

VOID worker_ThreadFunction();
VOID bubble_Sort(int* ToSort, int ArrSize);
VOID compare_Data(int* TestArray, int CurrentIteration);
int count_ElementsAboveAverage();

using namespace std;

int main()
{
	srand(time(0));

	cout << "Please, enter ArraySize: "; 
	cin >> ArraySize;

	if (ArraySize > 0) {
		int TestingIterationMax = 10; // Max count of testing iterations

		// Iterate several times to check if Semaphore is working properly and no error occur
		for (int TestingIteration = 0; TestingIteration < TestingIterationMax; TestingIteration++){

			hSem = CreateSemaphore(NULL, 1, 1, L"SEM");

			// Zero data after each testing iteration
			ArrayAverage = 0;
			ArraySum = 0;
			OddElementsSum = 0;

			// Initialize Array and copy its values to TestingArray
			Array = new int[ArraySize];
			int* TestingArray = new int[ArraySize];
			for (int i = 0; i < ArraySize; i++){
				Array[i] = rand() % 10;
				TestingArray[i] = Array[i];
			}

			// Creating worker
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)worker_ThreadFunction, NULL, 0, NULL);

			DWORD SemWaitResult = WaitForSingleObject(hSem, INFINITE);
			if (SemWaitResult == WAIT_OBJECT_0){
				// Sorting Array using bubble sort
				bubble_Sort(Array, ArraySize);

				// Displaying array after sort
				cout << "Sorted Array: ";
				for (int i = 0; i < ArraySize; i++) {
					cout << Array[i] << " ";
				}
				cout << "\n";
				ReleaseSemaphore(hSem, 1, NULL);			
			}
			else {
				cout << "Semaphore error on sorting array!\n";
			}

			DWORD WaitResult = WaitForSingleObject(hThread, INFINITE);
			if (WaitResult == 0) {
				cout << "ArrayAverage = " << ArrayAverage;
				cout << "\nSum of all elements in Array = " << ArraySum;
				cout << "\nCount of elements larger than average = " << count_ElementsAboveAverage();
				cout << "\nOdd elements sum = " << OddElementsSum;
				compare_Data(TestingArray, TestingIteration);
				cout << "\n------------------------------------\n\n";
			}
			else {
				cout << "ERROR or TIMEOUT!\n";
			}

			CloseHandle(hThread);
			CloseHandle(hSem);

			delete[] Array;
			delete[] TestingArray;
		}
	}
	else
	{
		cout << "Size of array need to be more than 0" << endl;
	}

	system("pause");
	return 0;
}

VOID compare_Data(int* TestArray, int CurrentIteration)
{
	float TestingArrayOddElementsSum = 0;
	for (int i = 0; i < ArraySize; i++)
		if (TestArray[i] % 2 != 0)
			TestingArrayOddElementsSum += TestArray[i];

	float TestingSum = 0.0;
	for (int i = 0; i < ArraySize; i++)
		TestingSum += TestArray[i];
	float TestingAverage = TestingSum / ArraySize;


	if (ArrayAverage != TestingAverage || OddElementsSum != TestingArrayOddElementsSum) {
		cout << "\nOn CurrentIteration = " << CurrentIteration << ", error occured!\n";
		cout << "ArrayAverage = " << ArrayAverage << ", TestingAverage = " << TestingAverage;
		cout << "\nOddElementsSum = " << OddElementsSum << ", TestingArrayOddElementsSum = " << TestingArrayOddElementsSum << "\n";
	}
}

int count_ElementsAboveAverage()
{
	int AboveAverageCount = 0;
	for (int i = 0; i < ArraySize; i++)
		if (Array[i] > ArrayAverage)
			AboveAverageCount++;
	return AboveAverageCount;
}

VOID worker_ThreadFunction()
{
	// Calculating average of Array elements
	DWORD WaitResult = WaitForSingleObject(hSem, INFINITE);
	if (WaitResult == WAIT_OBJECT_0) {

		for (int i = 0; i < ArraySize; i++)
		{
			ArraySum += Array[i];
			Sleep(12);
		}
		ArrayAverage = ArraySum / ArraySize;

		// Calculating sum of odd elements of Array
		for (int i = 0; i < ArraySize; i++)
		{
			if (Array[i] % 2 != 0)
				OddElementsSum += Array[i];
		}
		if (!ReleaseSemaphore(hSem, 1, NULL))
			cout << "\nSemaphore ERROR in worker thread\n";
	}
}

VOID bubble_Sort(int* ToSort, int ArrSize)
{
	for (int i = 0; i < ArrSize; i++){
		for (int j = 0; j < ArrSize - i - 1; j++){
			if (ToSort[j] > ToSort[j + 1])
				swap(ToSort[j], ToSort[j + 1]);
		}
	}
}

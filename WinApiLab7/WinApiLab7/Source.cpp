﻿// 2). Поток worker должен найти значение суммы четных элементов массива. Для синхронизации потоков использовать критическую секцию.

#include <iostream>
#include <thread>
#include <windows.h>

// Deniss Belovs 4801BD

using namespace std;

struct WorkerArrayParams
{
	int* arr = nullptr;
	size_t size = 0;
} params;

float average;
int evenSum;
CRITICAL_SECTION criticalSection;

bool debugEnabled = false;
int successCount = 0;

void WorkerThreadFunc();

void BubbleSort();
void Print(int* arr, size_t size);
unsigned CountLargerThanAverage(int* arr, size_t size, float inAverage);
void CalculateAverageValue();
float GetAverageValueTest(int* arr, size_t size);
void CalculateEvenSum();
int GetEvenSumTest(int* arr, size_t size);
int GetActualSum(int* arr, size_t size);
void PrintTest(int currentIteration, int* expectedArr, int* actualArr);

int main()
{
	srand(time(NULL));
	InitializeCriticalSection(&criticalSection);

	do
	{
		cout << "Enter size of an array > ";
		cin >> params.size;
	} while (params.size <= 0);

	int maxIterationCount = (debugEnabled ? 100000 : 10);
	for (int iteration = 0; iteration < maxIterationCount; iteration++)
	{
		if (!debugEnabled)
		{
			cout << "\n---------------------------------------------------\n";
			cout << iteration << "-iteration" << endl;
		}

		average = 0.f;
		evenSum = 0;
		
		params.arr = new int[params.size];
		int* testArr = new int[params.size];
		
		for (size_t i = 0; i < params.size; i++)
		{
			params.arr[i] = rand() % 10;
			testArr[i] = params.arr[i];
		}

		DWORD workerThreadId;
		HANDLE hWorkerThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkerThreadFunc, NULL, 0, &workerThreadId);
		if (hWorkerThread != INVALID_HANDLE_VALUE)
		{
			BubbleSort();

			if (!debugEnabled)
			{
				EnterCriticalSection(&criticalSection);
				cout << "Sorted array: ";
				Print(params.arr, params.size);
				LeaveCriticalSection(&criticalSection);
			}

			DWORD waitResult = WaitForSingleObject(hWorkerThread, INFINITE);

			if (!debugEnabled)
			{
				cout << "Average: " << average << ", Sum: " << GetActualSum(params.arr, params.size) << ", Array length: " << params.size << endl;
				cout << "Count of elements larger than average: " << CountLargerThanAverage(params.arr, params.size, average) << endl;
				cout << "Sum of even elements: " << evenSum << endl;
			}
			
			if (waitResult == 0)
			{
				if (!debugEnabled)
				{
					cout << "(Worker thread id: " << workerThreadId << ") Thread wait result: Worker is closed\n";
				}
				PrintTest(iteration, testArr, params.arr);
			}
			else if (waitResult == WAIT_TIMEOUT)
			{
				cout << "(Worker thread id: " << workerThreadId << ") Thread wait result: TIMEOUT\n";
				PrintTest(iteration, testArr, params.arr);
			}
			else
			{
				cout << "(Worker thread id: " << workerThreadId << ") Thread wait result: ERROR\n";
				PrintTest(iteration, testArr, params.arr);
			}
			CloseHandle(hWorkerThread);
		}
		else
		{
			cout << "Error on CreateThread\n";
		}

		delete[] params.arr;
		delete[] testArr;
	}
	DeleteCriticalSection(&criticalSection);

	cout << "\nProgram has finished. Total success iterations count: " << successCount << " (from max of " << maxIterationCount << ")\n";
	system("pause");
	return 0;
}

void WorkerThreadFunc()
{
	CalculateAverageValue();
	CalculateEvenSum();
}

void BubbleSort()
{
	EnterCriticalSection(&criticalSection);
	for (size_t i = 0; i < params.size; i++)
	{
		for (size_t j = 0; j < params.size - i - 1; j++)
		{
			if (params.arr[j] > params.arr[j + 1])
				swap(params.arr[j], params.arr[j + 1]);
		}
	}
	LeaveCriticalSection(&criticalSection);
}

void Print(int* arr, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
}

unsigned CountLargerThanAverage(int* arr, size_t size, float inAverage)
{
	unsigned outCount = 0;
	for (size_t i = 0; i < size; i++)
	{
		if (arr[i] > inAverage)
			outCount++;
	}
	return outCount;
}

void CalculateAverageValue()
{
	float sum = 0.f;
	EnterCriticalSection(&criticalSection);
	for (size_t i = 0; i < params.size; i++)
	{
		sum += params.arr[i];
		Sleep(12);
	}
	average = sum / (float)params.size;
	LeaveCriticalSection(&criticalSection);
}

float GetAverageValueTest(int* arr, size_t size)
{
	float sum = 0.f;
	for (size_t i = 0; i < size; i++)
		sum += arr[i];
	return sum / (float)size;
}

void CalculateEvenSum()
{
	EnterCriticalSection(&criticalSection);
	for (size_t i = 0; i < params.size; i++)
	{
		if (params.arr[i] % 2 == 0)
			evenSum += params.arr[i];
	}
	LeaveCriticalSection(&criticalSection);
}

int GetEvenSumTest(int* arr, size_t size)
{
	int sum = 0;
	for (size_t i = 0; i < size; i++)
	{
		if (arr[i] % 2 == 0)
			sum += arr[i];
	}
	return sum;
}

int GetActualSum(int* arr, size_t size)
{
	float sum = 0.f;
	for (size_t i = 0; i < size; i++)
		sum += arr[i];
	return sum;
}

void PrintTest(int currentIteration, int* expectedArr, int* actualArr)
{
	float testAverage = GetAverageValueTest(expectedArr, params.size);
	int testEvenSum = GetEvenSumTest(expectedArr, params.size);
	int testCountOfLargeThanAverage = CountLargerThanAverage(expectedArr, params.size, testAverage);

	int actualCountOfLargeThanAverage = CountLargerThanAverage(actualArr, params.size, average);
	if (average != testAverage || evenSum != testEvenSum || testCountOfLargeThanAverage != actualCountOfLargeThanAverage)
	{
		printf("(Iteration: %d) ERROR! %s: expected=%.4f | actual=%.4f, %s: expected=%d | actual=%d, %s: expected=%d | actual=%d \n",
			currentIteration,
			"average", testAverage, average,
			"evenSum", testEvenSum, evenSum,
			"countOfLargeThanAverage", testCountOfLargeThanAverage, actualCountOfLargeThanAverage);
	}
	else
	{
		successCount++;
	}
}


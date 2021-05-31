/* 4. Создать структуру, содержащую массив int размером 500, массив double
размером 100 и число short;
Создать массив структуры выше размером 8, заполнить массив int индексом
структуры (в массиве структуры), массив double индексом делённым на 100, а
число short индексом массива, который находится перед ним в виртуальной
памяти (подробнее ниже) (-1 для первой структуры);
Создать вспомогательный массив short размера 8;
Записать структуры в память в рандомном порядке, записав порядок 
(индексы) в вспомогательный массив;
При считывании из виртуальной памяти, считывать массив структур в
правильном (0-7) порядке, используя вспомогательный массив;
Вывести на экран 10-ый элемент обоих массивов всех структур и всех чисел
short.
*/

#include <iostream>
#include <windows.h>

struct Data {
	int int_arr[500];
	double double_arr[100];
	short short_val;
};

using namespace std;

bool arrContains(short a, short* arr, int n);

int main(){
	srand(time(0));

	// 1)
	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	cout << "dwPageSize = " << system_info.dwPageSize << endl;
	cout << "dwAllocationGranularity = " << system_info.dwAllocationGranularity << endl;

	// 2)
	Data* a = (Data*)VirtualAlloc(NULL, system_info.dwAllocationGranularity, MEM_RESERVE, PAGE_READWRITE);
	a = (Data*)VirtualAlloc(a, system_info.dwPageSize, MEM_COMMIT, PAGE_READWRITE);
	if (!a){
		cout << "Virtual allocation failed" << endl;
		system("PAUSE");
		return GetLastError();
	}

	// 3)
	Data* arr = new Data[8];
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 500; j++){
			arr[i].int_arr[j] = i;
		}

		for (int j = 0; j < 100; j++) {
			arr[i].double_arr[j] = i != 0.0 ? i / 100.0 : 0.0;
		}

		arr[i].short_val = i - 1;
	}

	cout << endl << "------- Initial array ------" << endl;
	for (int i = 0; i < 8; i++){
		cout << "int_arr = " << arr[i].int_arr[10] << endl;
		cout << "double_arr = " << arr[i].double_arr[10] << endl;
		cout << "short_val = " << arr[i].short_val << endl << endl;
	}

	short* short_arr = new short[8];

	// Writing data
	cout << "short_arr (SUPPORT INDEXES) : ";
	for (int i = 0; i < 8; i++)
	{
		short index = rand() % 8;
		while (arrContains(index, short_arr, 8)){
			index = rand() % 8;
		}
		short_arr[i] = index;
		cout << short_arr[i] << " ";
	}

	cout << endl << endl << "------- Writing data ------" << endl;
	Data* b = a;
	for (int i = 0; i < 8; i++){
		Data* temp = b;
		__try {
			*b = arr[short_arr[i]];
			cout << "i = " << i << ", Written Data at address = " << b << endl;
			cout << "short_arr[" << i << "] (SUPPORT INDEX) = " << short_arr[i] << endl;
			b++;
			cout << "Next address = " << b << endl << endl;
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			cout << "EXCEPTION, i = " << i << endl;
			b = (Data*)VirtualAlloc(b, system_info.dwPageSize, MEM_COMMIT, PAGE_READWRITE);
			cout << "Created new page at " << b << " address" << endl;
			i--;
			b = temp;
		}
	}

	// Reading data
	cout << endl << "------- Reading data ------" << endl;
	Data* c = a;
	for (int i = 0; i < 8; i++){
		cout << "short_arr[" << i << "] (SUPPORT INDEX) = " << short_arr[i] << endl;
		Data* data = (Data*)((char*)c + short_arr[i] * sizeof(Data));
		cout << "data pointer = " << data << endl;
		cout << "int_arr = " << data->int_arr[10] << endl;
		cout << "double_arr = " << data->double_arr[10] << endl;
		cout << "short_val = " << data->short_val << endl << endl;
	}

	// Free memory
	if (!VirtualFree(a, 0, MEM_RELEASE))
	{
		cout << "Memory release failed." << endl;
		system("pause");
		return GetLastError();
	}
	system("pause");
	return 0;
}

bool arrContains(short a, short* arr, int n)
{
	for(int i = 0; i < n; i++){
		if (arr[i] == a)
			return true;
	}
	return false;
}

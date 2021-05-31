// 2. Создать массив char, содержащий все заглавные буквы латинского алфавита (A - Z);
// Создать массив long long случайного размера от 500 до 1000, где нулевой
// элемент будет заполнен максимальным числом этого типа каждый
// последующий элемент массива будет на 333 меньше предыдущего;
// Записать чередуя массивы(char[0], long long[0], char[1], long long[1]…), пока не
// кончится массив long long, а массив char начинать с нулевого элемента при
// достижении конца массива;
// Вывести на экран массив char(изначального размера) и последние 5
// элементов массива long long.

#include <windows.h>
#include <iostream>
#include <vector>
#include <algorithm>

// Deniss Belovs 4801BD

using namespace std;

pair<PCHAR, DWORD> GetCharArray();
pair<PLONGLONG, DWORD> GetLongLongArray();
VOID WriteToPage(pair<PCHAR, DWORD>& charArray, pair<PLONGLONG, DWORD>& longLongArray, const LPVOID& pagePtr, SYSTEM_INFO systemInfo, vector<int>& outExceptionIndices, LPVOID& outIterationPtr);
VOID ReadFromPage(pair<PCHAR, DWORD>& charArray, pair<PLONGLONG, DWORD>& longLongArray, const LPVOID& pagePtr, const vector<int>& inExceptionIndices, const LPVOID& inIterationPtr);
int ReleaseMemory(LPVOID beginAddress);

int main()
{
	srand(time(NULL));

	// 1.
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	DWORD pageCount = systemInfo.dwAllocationGranularity / systemInfo.dwPageSize;
	cout << "Page size: " << systemInfo.dwPageSize
		<< "\nAllocation granularity: " << systemInfo.dwAllocationGranularity 
		<< "\nPage count: " << pageCount
		<< endl;

	// 2. Reserve memory address region of minimal allowed size for one page.
	LPVOID beginAddress = VirtualAlloc(
		NULL,
		/*USN_PAGE_SIZE*/systemInfo.dwAllocationGranularity,
		MEM_RESERVE,
		PAGE_READWRITE
	);

	// Commiting one page
	beginAddress = VirtualAlloc(beginAddress, systemInfo.dwPageSize, MEM_COMMIT, PAGE_READWRITE);

	if (!beginAddress)
	{
		cout << "Virtual allocation failed.\n";
		system("pause");
		return GetLastError();
	}

	// Get variant data
	pair<PCHAR, DWORD> lpCharArray = GetCharArray();
	pair<PLONGLONG, DWORD> lpDataArray = GetLongLongArray();

	// 3. Write variant data at the beginning of a reserved page
	cout << "Virtual memory address: " << beginAddress << endl;
	vector<int> exceptionIndices;
	LPVOID iterationPtr;
	WriteToPage(lpCharArray, lpDataArray, beginAddress, systemInfo, exceptionIndices, iterationPtr);
	ReadFromPage(lpCharArray, lpDataArray, beginAddress, exceptionIndices, iterationPtr);

	return ReleaseMemory(beginAddress);
}

pair<PCHAR, DWORD> GetCharArray()
{
	cout << "\nCreating char array:\n";
	const DWORD dwCharArraySize = 'Z' - 'A' + 1;
	cout << "dwCharArraySize: " << dwCharArraySize << endl;
	PCHAR lpCharArray = new CHAR[dwCharArraySize + sizeof(CHAR)];
	for (CHAR i = 0; i < dwCharArraySize; i++)
	{
		lpCharArray[i] = i + 'A';
		cout << "i: " << (int)i << " " << lpCharArray[i] << endl;
	}
	lpCharArray[dwCharArraySize] = '\0';
	cout << "lpCharArray = " << lpCharArray << ", address = " << (LPVOID)lpCharArray << ", strlen(lpCharArray) = " << strlen(lpCharArray) << endl;
	return make_pair(lpCharArray, dwCharArraySize);
}

pair<PLONGLONG, DWORD> GetLongLongArray()
{
	cout << "\nCreating long long array:\n";
	const DWORD dwLongLongArraySize = rand() % 1000 + 500;
	cout << "dwLongLongArraySize: " << dwLongLongArraySize << endl;
	PLONGLONG lpLongLongArray = new LONGLONG[dwLongLongArraySize];
	for (int i = 0; i < dwLongLongArraySize; i++)
		lpLongLongArray[i] = MAXLONGLONG - 333 * static_cast<long long>(i);

	cout << "Last five long long elements:\n";
	for (int i = 0; i < 5; i++)
		cout << "i: " << dwLongLongArraySize - 1 - i << ", " << lpLongLongArray[dwLongLongArraySize - 1 - i] << endl;

	cout << endl;
	return make_pair(lpLongLongArray, dwLongLongArraySize);
}

VOID WriteToPage(pair<PCHAR, DWORD>& charArray, pair<PLONGLONG, DWORD>& longLongArray, const LPVOID& pagePtr, SYSTEM_INFO systemInfo, vector<int>& outExceptionIndices, LPVOID& outIterationPtr)
{
	DWORD commitedPageCount = 1;
	LPVOID iterationPtr = pagePtr;
	cout << "Press any button to start writing to virtual memory!\n";
	system("pause");
	cout << "--- WRITING TO VIRTUAL MEMORY:\n\nINITIAL DATA: "
		 << "pagePtr: " << pagePtr
		 << ", iterationPtr: " << iterationPtr
		 << ", char array: " << charArray.first
		 << endl;

	for (int i = 0; i < longLongArray.second; i++)
	{
		__try
		{
			PCHAR chPagePtr = (PCHAR)iterationPtr;
			cout << "\ni: " << i;
			cout << ", Char index: " << i % charArray.second;
			CHAR currentChar = charArray.first[i % charArray.second];
			cout << ", Current char: " << currentChar;
			*chPagePtr = (CHAR)*(charArray.first + (i % charArray.second));
			cout << "\n(WROTE " << currentChar << " AT LOCATION " << (LPVOID)chPagePtr << ") ";
			chPagePtr++;
			cout << "Current pointer location: " << (LPVOID)chPagePtr << endl;
			
			PLONGLONG llPagePtr = (PLONGLONG)chPagePtr;
			*llPagePtr = *(longLongArray.first + i);
			cout << "(WROTE " << *llPagePtr << " AT LOCATION " << llPagePtr << ") ";
			llPagePtr++;
			cout << "Current pointer location: " << llPagePtr << " (shifted by " << (llPagePtr - (LPVOID)chPagePtr) * 8 << " bytes)" << endl;
			iterationPtr = llPagePtr;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			cout << "\n------------------------ EXCEPTION OCCURED --------------------------\n";
			cout << "iterationPtr = " << iterationPtr << ", i = " << i << endl;
			PBYTE bytePtr = (PBYTE)pagePtr + systemInfo.dwPageSize * commitedPageCount++;

			bytePtr = (PBYTE)VirtualAlloc(
				bytePtr,
				systemInfo.dwPageSize,
				MEM_COMMIT,
				PAGE_READWRITE
			);
			cout << "New virtual page (of size " << systemInfo.dwPageSize << ") added, commitedPageCount: " << commitedPageCount << ", page address: " << (LPVOID)bytePtr << endl;
			outExceptionIndices.push_back(i);
			i--;	
			system("pause");
		}
	}
	cout << endl;
	outIterationPtr = iterationPtr;
}

VOID ReadFromPage(pair<PCHAR, DWORD>& charArray, pair<PLONGLONG, DWORD>& longLongArray, const LPVOID& pagePtr, const vector<int>& inExceptionIndices, const LPVOID& inIterationPtr)
{
	cout << "\n --- READING FROM VIRTUAL MEMORY:\n";
	cout << "Exceptions occured at: ";
	for (int i : inExceptionIndices)
		cout << i << " ";
	cout << "indices.";

	PCHAR charIterationPtr = (PCHAR)pagePtr;
	cout << "\nChar array: \n";
	DWORD currentByteShift = 0;
	for (int i = 0; i < charArray.second; i++)
	{
		cout << "(at address " << (LPVOID)(charIterationPtr - currentByteShift) << ") " << *(charIterationPtr + currentByteShift) << endl;
		currentByteShift += sizeof(CHAR) + sizeof(LONGLONG);
	}

	PBYTE llIterationPtr = (PBYTE)inIterationPtr - sizeof(LONGLONG);
	cout << "\nFive last elements of long long array: \n";
	currentByteShift = 0;
	for (int i = 0; i < 5; i++)
	{
		cout << "(at address " << (LPVOID)(llIterationPtr - currentByteShift) << ") " << *(PLONGLONG)(llIterationPtr - currentByteShift) << endl;
		currentByteShift += sizeof(CHAR) + sizeof(LONGLONG);
	}
	cout << endl;

	// Read all written data from virtual memory
	//LPVOID iterationPtr = pagePtr;
	//for (int i = 0; i < longLongArray.second; i++)
	//{
	//	if (count(inExceptionIndices.begin(), inExceptionIndices.end(), i))
	//		cout << " ------- (EXCEPTION OCCURED HERE) ";
	//
	//	PCHAR chPagePtr = (PCHAR)iterationPtr;
	//	cout << "iteration: " << i << ", (address: " << (LPVOID)chPagePtr << ") CHAR: " << *chPagePtr << ", ";
	//
	//	PLONGLONG llPagePtr = (PLONGLONG)++chPagePtr;
	//	cout << "(address: " << llPagePtr << ") LONG LONG: " << *llPagePtr << endl;
	//
	//	iterationPtr = ++llPagePtr;
	//}
}

int ReleaseMemory(LPVOID beginAddress)
{
	if (!VirtualFree(beginAddress, 0, MEM_RELEASE))
	{
		cout << "Memory release failed." << endl;
		system("pause");
		return GetLastError();
	}
	system("pause");
	return 0;
}

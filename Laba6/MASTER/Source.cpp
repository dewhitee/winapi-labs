#include <iostream>
#include <windows.h>

//Вариант 10.
//1. Написать программу SLAVE, которая генерирует двумерный массив вещественных чисел
//размерности mxn.Значения m и n программа получает в качетсве параметров.Отсортировать
//значения массива в строках в порядке убывания с выводом на экран.Если параметры не
//переданы, вывести об этом сообщение и закончить программу.Если программа запущена из
//приложения MASTER, вывести на экран уникальный идентификатор приложения MASTER.
//Если программа запущена сама по себе, вывести об этом информацию.
//2. Написать приложение MASTER которое :
//а) получает в качестве параметра имя исполняемого файла и запускает данное
//приложение;
//б) если MASTER запущена без параметра, то программа генерирует 2 целых числа,
//запускает приложение SLAVE и передает в качестве параметров сгенерированные
//числа и идентификатор процесса(MASTER).При завершении программы SLAVE
//завершается MASTER
//3. Продемонстрировать совместную работу двух приложений.Для этого на экран должна
//выводиться достаточно подробная информация о ходе выполнения каждого из процессов.

using namespace std;

void new_Process(char* command);

int main(int argc, char** argv){
	srand(time(0));
	cout << "Starting MASTER" << endl;
	if (argc < 2){
		cout << "Executable name is required as parameter " << endl;
		cout << "Default process started" << endl;
		int num1 = rand() % 10 + 1;
		int num2 = rand() % 10 + 1;
		
		char command[200];
		sprintf_s(command, "%s %d %d %d", "SLAVE.exe", num1, num2, GetCurrentProcessId());
		
		cout << "Command = " << command << endl;
		new_Process(command);
	}
	else{
		cout << "Executable name = " << argv[1] << endl;
		new_Process(argv[1]);
	}
	cout << "Ending MASTER" << endl;
	return 0;
}


void new_Process(char* command)
{
	cout << "PID of MASTER = " << GetCurrentProcessId() << endl;

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;

	if (CreateProcess(NULL, command, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi))
	{
		cout << "SLAVE PID = " << pi.dwProcessId << endl;
		cout << "SLAVE TID = " << pi.dwThreadId << endl;
		DWORD WaitRes = WaitForSingleObject(pi.hProcess, 10000);
		if (WaitRes == 0)
		{
			cout << "SLAVE IS CLOSED." << endl;
		}
		else if (WaitRes == WAIT_TIMEOUT)
		{
			cout << "TIMEOUT " << endl;
		}
		else
		{
			cout << "ERROR!" << endl;;
		}

		CloseHandle(pi.hProcess);
	}
	else
	{
		cout << "ERROR when creating process" << endl;
	}
}

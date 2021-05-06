#include <iostream>
#include <thread>
#include <windows.h>

//Вариант 8.
//1. Написать программу SLAVE , которая выводит на экран содержимое текстового файла,
//имя которого передается ей в параметре. Если параметр не передан, вывести об этом
//сообщение и закончить программу. Если программа запущена из приложения MASTER,
//вывести на экран уникальный идентификатор приложения MASTER. Если программа
//запущена сама по себе, вывести об этом информацию.
//2. Написать приложение MASTER которое:
//а) генерирует массив целых чисел (100 элементов)
//б) сохраняет массив в текстовом файле
//в) запускает приложение SLAVE и передает в качестве параметров имя текстового
//файла и идентификатор процесса (MASTERA). При завершении программы SLAVE
//завершается MASTER
//3. Продемонстрировать совместную работу двух приложений. Для этого на экран должна
//выводиться достаточно подробная информация о ходе выполнения каждого из процессов.
//Вариант 9.
//1. Написать программу SLAVE , которая генерирует одномерный массив целых чисел
//размерности, равной числу, полученному в качестве параметра, и сортирует его в порядке
//возрастания с выводом на экран. Если параметр не передан, вывести об этом сообщение и
//закончить программу. Если программа запущена из приложения MASTER, вывести на экран

using namespace std;

int main(int argc, char** argv) {
	int* arr = new int[100];
	for (int i = 0; i < 100; i++)
	{
		arr[i] = i;
	}

	char file_name[] = "my_array_file.txt";

	HANDLE file = CreateFile(file_name, GENERIC_WRITE | GENERIC_READ, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	cout << "my_array_file.txt was created" << endl;

	SetFilePointer(file, 0, 0, FILE_BEGIN);
	for (int i = 0; i < 100; i++){
		WriteFile(file, &arr[i], sizeof(int), 0, 0);
	}
	CloseHandle(file);

	DWORD exit_code;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;

	int master_pid = GetCurrentProcessId();
	cout << "master_pid: " << master_pid << endl;

	char cmd[250];
	sprintf_s(cmd, "%s %s %d", "SLAVE.exe", file_name, master_pid);
	cout << "cmd: " << cmd << endl;
	if (CreateProcess(0, cmd, 0, 0, FALSE, 0, 0, 0, &si, &pi)) {
		cout << "slave process tid: " << pi.dwThreadId << " pid: " << pi.dwProcessId << endl;
		DWORD wait_res = WaitForSingleObject(pi.hProcess, 12000);
		if (wait_res == 0) {
			cout << "SLAVE closed" << endl;
		} else if (wait_res == WAIT_TIMEOUT) {
			cout << "TIMEOUT" << endl;
		} else {
			cout << "ERROR" << endl;
		}
		CloseHandle(pi.hProcess);
	} else {
		cout << "CreateProcess ERROR" << endl;
	}
	cout << "MASTER closed" << endl;
	return 0;
}
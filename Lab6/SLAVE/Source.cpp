#include <iostream>
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

int main(int argc, char** argv){
	if (argc == 3)
	{
		cout << "Starting by MASTER - MASTER pid: " << argv[2] << endl;
	}
	else
	{
		cout << "Starting by self" << endl;
	}

	if (argc > 1) {
		cout << "file name: " << argv[1] << endl;
		HANDLE file = CreateFile(argv[1], GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (file != INVALID_HANDLE_VALUE) {
			cout << "Read as text: " << endl;
			SetFilePointer(file, 0, 0, FILE_BEGIN);
			DWORD size = GetFileSize(file, 0);
			for (DWORD i = 0; i < size; i++) {
				char ch;
				if (ReadFile(file, &ch, sizeof(char), 0, 0)) {
					cout << ch;
				} else {
					cout << "ERROR!" << endl;
				}
			}
			cout << endl << "Read as integers: " << endl;
			SetFilePointer(file, 0, 0, FILE_BEGIN);
			size = size / sizeof(int);
			for (DWORD i = 0; i < size; i++){
				int number;
				if (ReadFile(file, &i, sizeof(int), 0, 0)) {
					cout << i << endl;
				} else {
					cout << "ERROR!" << endl;
				}
			}
			CloseHandle(file);
		} else {
			cout << "ERROR with file" << endl;
		}
	} else {
		cout << "Not enough arguments" << endl;
		return 1;
	}
	system("pause");
	return 0;
}
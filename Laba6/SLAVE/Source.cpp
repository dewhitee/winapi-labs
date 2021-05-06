#include <iostream>
#include <algorithm>

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

void print_mat(float** mat, int m, int n);

int main(int argc, char** argv) {
	cout << "Starting SLAVE" << endl;
	if (argc > 2) {
		if (argc == 4){
			cout << "SLAVE Process started from MASTER" << endl;
			cout << "(SLAVE) MASTER PID = " << argv[3] << endl;
			cout << "Parameters: " << argv[0] << " " << argv[1] << " " << argv[2] << " " << argv[3] << endl;
		}
		else{
			cout << "Process started by itself" << endl;
			cout << "Parameters: " << argv[0] << " " << argv[1] << " " << argv[2] << endl;
		}

		if (isdigit(argv[1][0]) && isdigit(argv[2][0])){
			int m = atoi(argv[1]);
			int n = atoi(argv[2]);
			
			cout << "m = " << m << ", n = " << n << endl;
			float** mat = new float*[m];

			// init
			for (int i = 0; i < m; i++) {
				mat[i] = new float[n];
				for (int j = 0; j < n; j++) {
					mat[i][j] = rand() % 10 * 0.1;
				}
			}

			//print initialized
			cout << endl << "Initial 2d array: " << endl;
			print_mat(mat, m, n);

			// sort
			for (int i = 0; i < m; i++){
				sort(mat[i], mat[i] + n, greater<float>());
			}

			// print sorted
			cout << endl << "Sorted 2d array: " << endl;
			print_mat(mat, m, n);

			// delete
			for (int i = 0; i < m; i++){
				delete mat[i];
			}
			delete[] mat;
		} 
		else {
			cout << "Both parameters must be float numbers" << endl;
			return 1;
		}
	}
	else{
		cout << "Not enough arguments" << endl;
		return 1;
	}
	system("pause");
	cout << "Ending SLAVE" << endl;
	return 0;
}

void print_mat(float** mat, int m, int n)
{
	for (int i = 0; i < m; i++) {
		cout << i << "-row: ";
		for (int j = 0; j < n; j++) {
			cout << mat[i][j] << " ";
		}
		cout << endl;
	}
}

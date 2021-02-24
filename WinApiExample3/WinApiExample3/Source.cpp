#include <Windows.h>
#include <iostream>
#include <iomanip>

using namespace std;

void DisplayBits(unsigned int);

int main()
{
	unsigned int number1, number2, mask, setBits;
	char text[] = "Input data:";

	//--- Bitwise AND(&) -----------------------------
	number1 = 65535; mask = 1;
	cout << endl << text << endl;
	DisplayBits(number1);
	DisplayBits(mask);
	cout << "Result of & (AND) operation:" << endl;
	DisplayBits(number1 & mask);
	//--- Bitwise OR (|) ----------------------------
	number1 = 15;
	setBits = 241;
	cout << endl << text << endl;
	DisplayBits(number1); DisplayBits(setBits);
	cout << "Result of | (OR) operation:" << endl;
	DisplayBits(number1 | setBits);
	//--- Bitwise EXCLUSIVE OR- XOR(^) ---------------
	number1 = 139; number2 = 199;
	cout << endl << text << endl;
	DisplayBits(number1);
	DisplayBits(number2);
	cout << "Result of ^ (XOR) operation:" << endl;
	DisplayBits(number1 ^ number2);
	//--- Bitwise NOT (~) ------------------------
	number1 = 21845;
	cout << endl << text << endl;
	DisplayBits(number1);
	cout << "Result of ~ (NOT) operation:" << endl;
	DisplayBits(~number1);
	system("PAUSE");
	return 0;

}

void DisplayBits(unsigned int value)
{
	unsigned int c = 0;
	unsigned int displayMask = 1 << 31;

	cout << setw(12) << value << " = ";

	for (c = 1; c <= 32; c++)
	{
		cout << (value & displayMask ? '1' : '0');
		displayMask >>= 1;
		if (c % 8 == 0)
		{
			cout << ' ';
		}
	}
	cout << endl;
}


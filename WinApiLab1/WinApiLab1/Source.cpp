#include <Windows.h>
#include <iostream>
#include <iomanip>

using namespace std;

void DisplayBits(int);
void DisplayAsDecimal(int);
void CycleShift(unsigned int);
void PackDate(); // unsigned ints
void UnpackDate(unsigned int packedDate);
void CountOnes(unsigned int value);

int main()
{



	system("PAUSE");
	return 0;
}

void DisplayBits(int value)
{
	// 1000... 31 bits
	int displayMask = 1 << 31;

	cout << setw(12) << value << " = ";

	for (int c = 1; c <= 32; c++)
	{
		// Output current bit value
		cout << (value & displayMask ? '1' : '0');

		// Shifting mask to the right
		displayMask >>= 1;

		// Adding space every 8 bits
		if (c % 8 == 0)
		{
			cout << ' ';
		}
		cout << endl;
	}
}

void DisplayAsDecimal(int)
{

}

void CycleShift(unsigned int value)
{
	unsigned int shift = 8;

	//value = 0b01011111 00001111 01010101 00001111;
	value >>= 8;
	// 00000000 01011111 00001111 01010101

	// left shift
	//value = 0b01011111 00001111 01010101 00001111;
	// shift = 8
	unsigned int mask = 1 << 31; // 1000... 31 bits

	// if value first bit is 1 we need to add 1 to the value
	if (value & mask)
	{
		value <<= 1;
		value |= 1;
	}
	else
	{
		value <<= 1;
	}
}

void PackDate()
{
	// 13:01.5

	// 5 bits for hours
	// 6 bits for minutes
	// 6 bits for seconds
	// 5 bits for days
	// 4 bit for months

	unsigned int date = 0;
	unsigned int month, day, hours, minutes, seconds;

	cout << "Enter month"; cin >> month;
	cout << "Enter day"; cin >> day;
	cout << "Enter hour"; cin >> hours;
	cout << "Enter minutes"; cin >> minutes;
	cout << "Enter seconds"; cin >> seconds;

	date = seconds;
	date |= (minutes << 6);
	date |= (hours << 11);
	date |= (day << 16);
	date |= (month << 21);

	// Adding hours
	///date = hours;

	// Adding minutes
	///date |= (minutes << 5);

	// Adding seconds
	///date |= (seconds << 11);

	// Print date value as decimal


	// Print date value as binary
	DisplayBits(date);
}

void UnpackDate(unsigned int packedDate)
{
	unsigned int date = 0;

	unsigned int hours = date & 31;
	// 63 = 00111111
	unsigned int minutes = (date & (63 << 5)) >> 5;
	unsigned int seconds = (date & (63 << 11)) >> 11;

	// Print as decimal

	// Print as binary
}

void CountOnes(unsigned int value)
{
	unsigned int mask = 1 << 31;
	
	for (int i = 1; i < 32; i++)
	{
		// checking mask
	}

	// Hard method

	unsigned char bitArray[256];

	bitArray[0] = 0;
	bitArray[1] = 1;
	// ...
	bitArray[255] = 8;

	// given a value == 10100100 00000000 11110000 10101010		-- our value
	//				   &00000000 00000000 00000000 11111111		-- 255 mask
	//				    -----------------------------------
	//					00000000 00000000 00000000 10101010 == index

	// 
	bitArray[value & 255]
		+ bitArray[(value >> 8) & 255] // or; bitArray[(value & (255 << 8)) >> 8]
		+ bitArray[(value >> 16) & 255] // checking third byte
		+ bitArray[(value >> 24) & 255]; // checking fourth byte
}

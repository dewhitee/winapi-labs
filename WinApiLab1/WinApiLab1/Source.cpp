#include <Windows.h>
#include <iostream>
#include <iomanip>

// Deniss Belovs / variant 2

using namespace std;

const unsigned char bitsPerMonth = 4;
const unsigned char bitsPerDays = 5;
const unsigned char bitsPerHours = 5;
const unsigned char bitsPerMinutes = 6;
const unsigned char bitsPerSeconds = 6;

// 7.3 function
void DisplayBits(unsigned int);

// 7.4 function
unsigned int CycleShift(unsigned int);

// 7.5 functions
unsigned int PackDate();
unsigned int PackDate(unsigned month, unsigned day, unsigned hours, unsigned minutes, unsigned seconds);
void UnpackDate(unsigned int packedDate);

// Returns ones count in byte for specified value
unsigned int GetOnesCountInByte(unsigned int value);

// Count ones variants for testing
unsigned int CountOnes(unsigned int value);
unsigned int CountOnes2(unsigned int value);
unsigned int CountOnes3(unsigned int value);

// 7.6 functions
unsigned int CountOnesInByte();
unsigned int CountOnesInByte2();

// Helpers
void Separator();
void InitialValuePrint(unsigned value);

int main()
{
	// Testing values
	int val = 2000000000;
	int val2 = 1;
	int val3 = 0xffffffff;

	// 7.3
	cout << "(7.3) Display bits:\n";
	DisplayBits(val);
	Separator();

	// 7.4
	cout << "(7.4) Right shift by one bit:\n";
	CycleShift(val);
	Separator();

	// 7.5.1
	cout << "(7.5) Pack date:\n";
	unsigned int outDate = PackDate();
	Separator();

	// 7.5.2
	cout << "(7.5.1) Unpack date:\n";
	UnpackDate(outDate);
	Separator();

	// 7.6
	cout << "(7.6) Counting ones:\n";
	CountOnesInByte();
	cout << "\n(7.6 first version) Count of ones:\n";
	Separator();
	cout << "(7.6 second version) Count of ones:\n";
	CountOnesInByte2();
	Separator();

	system("PAUSE");
	return 0;
}

void DisplayBits(unsigned int value)
{
	// 1000... 31 bits
	unsigned int displayMask = 1 << 31;

	cout << setw(12) << value << " = ";

	for (unsigned int c = 1; c <= 32; c++)
	{
		// Output current bit value
		cout << (value & displayMask ? '1' : '0');

		// Shifting mask to the right
		displayMask >>= 1;

		// Adding space every 8 bits
		if (c % 8 == 0)
			cout << ' ';
	}
	cout << endl;
}

unsigned int CycleShift(unsigned int value)
{
	InitialValuePrint(value);

	unsigned int shift = 8;
	unsigned int mask = 1;
	unsigned int rightShift = 1;

	// if value's first bit is 1 we need to add 1 to the value
	if (value & mask)
	{
		value >>= rightShift;

		// Push lower bit to the higher bit
		value |= (1 << 31);
	}
	else
	{
		value >>= rightShift;
	}
	
	cout << "Result: " << endl;
	DisplayBits(value);
	return value;
}

// Returns date
unsigned int PackDate()
{
	unsigned int date = 0;
	unsigned int month, day, hours, minutes, seconds;

	cout << "\nEnter month > "; cin >> month;
	cout << "Enter day > "; cin >> day;
	cout << "Enter hour > "; cin >> hours;
	cout << "Enter minutes > "; cin >> minutes;
	cout << "Enter seconds > "; cin >> seconds;

	return PackDate(month, day, hours, minutes, seconds);
}

unsigned int PackDate(unsigned month, unsigned day, unsigned hours, unsigned minutes, unsigned seconds)
{
	unsigned date = seconds;
	date |= (minutes << bitsPerSeconds);
	date |= (hours << (bitsPerSeconds + bitsPerMinutes));
	date |= (day << (bitsPerSeconds + bitsPerMinutes + bitsPerHours));
	date |= (month << (bitsPerSeconds + bitsPerMinutes + bitsPerHours + bitsPerDays));

	cout << "\nPack result:";
	cout << "\nMonth: " << month << ", Days: " << day << ", Hours: " << hours << ", Minutes: " << minutes << ", Seconds: " << seconds << endl;

	DisplayBits(date);
	return date;
}

void UnpackDate(unsigned int packedDate)
{
	const bool displayAllBits = false;

	// Retrieves bits value for the specified reserved count of bits shifted to the left by some value
	auto unpackFor = [packedDate, displayAllBits](unsigned int reservedBits, unsigned int shift) {
		unsigned int outValue = (packedDate & (reservedBits << shift)) >> shift;
		if (displayAllBits)
		{
			cout << "Packed date value:\n";
			DisplayBits(packedDate);
			cout << "Reserved bits << shift:\n";
			DisplayBits(reservedBits << shift);
			cout << "Out value:\n";
			DisplayBits(outValue);
		}
		return outValue;
	};

	InitialValuePrint(packedDate);

	unsigned int seconds = unpackFor(63, 0);
	unsigned int minutes = unpackFor(63, bitsPerSeconds);
	unsigned int hours = unpackFor(31, bitsPerSeconds + bitsPerMinutes);
	unsigned int day = unpackFor(31, bitsPerSeconds + bitsPerMinutes + bitsPerHours);
	unsigned int month = unpackFor(31, bitsPerSeconds + bitsPerMinutes + bitsPerHours + bitsPerDays);

	// Print as decimal
	cout << "\nUnpack result:";
	cout << "\nMonth: " << month << ", Days: " << day << ", Hours: " << hours << ", Minutes: " << minutes << ", Seconds: " << seconds << endl;

	// Print as binary
	PackDate(month, day, hours, minutes, seconds);
}

unsigned int GetOnesCountInByte(unsigned int value)
{
	unsigned int outCount = 0;
	for (unsigned char i = 0; i < 8; i++)
		outCount += ((value >> i) & 1);

	return outCount;
}

unsigned int CountOnes(unsigned int value)
{
	// Hard method
	unsigned char bitArray[256];

	// Fill bit array
	for (unsigned int i = 0; i < 256; i++)
		bitArray[i] = GetOnesCountInByte(i);

	unsigned int firstByteOnes = bitArray[value & 255];
	unsigned int secondByteOnes = bitArray[(value >> 8) & 255];  // or; bitArray[(value & (255 << 8)) >> 8]
	unsigned int thirdByteOnes = bitArray[(value >> 16) & 255];  // checking third byte
	unsigned int fourthByteOnes = bitArray[(value >> 24) & 255];  // checking fourth byte
	unsigned int outResult = firstByteOnes + secondByteOnes + thirdByteOnes + fourthByteOnes;

	cout << "\nCount of ones by byte:\nFirst byte: "
		<< firstByteOnes << ", Second byte: "
		<< secondByteOnes << ", Third byte: "
		<< thirdByteOnes << ", Fourth byte: "
		<< fourthByteOnes << endl;

	InitialValuePrint(value);
	cout << "\nTotal count of ones: " << outResult << endl;
	return outResult;
}

unsigned int CountOnesInByte()
{
	unsigned char bitArray[256];
	for (unsigned int i = 0; i < 256; i++)
		bitArray[i] = GetOnesCountInByte(i);

	int value;
	cout << "Enter value to count it's 1 bits > "; cin >> value;
	unsigned int byteIndex;
	cout << "Enter byte index in which count 1 bits (choose from 1 (lower) to 4 (higher)) > "; cin >> byteIndex;

	unsigned outCount = 0;
	switch (byteIndex)
	{
		case 1: 
			outCount = bitArray[value & 255];
			break;
		case 2: 
			outCount = bitArray[(value >> 8) & 255];
			break;
		case 3: 
			outCount = bitArray[(value >> 16) & 255];
			break;
		case 4: 
			outCount = bitArray[(value >> 24) & 255];
			break;
		default:
		{
			cout << "You have specified wrong byte index.\n";
		}
	}
	InitialValuePrint(value);
	cout << "\nTotal count of ones: " << outCount << endl;
	return outCount;
}

unsigned int CountOnesInByte2()
{
	int value;
	cout << "Enter value to count it's 1 bits > "; cin >> value;
	unsigned int byteIndex;
	cout << "Enter byte index in which count 1 bits (choose from 1 (lower) to 4 (higher)) > "; cin >> byteIndex;

	unsigned int maskedValue = 0;

	switch (byteIndex)
	{
		case 1: 
			maskedValue = value & (255 << 0);
			break;
		case 2: 
			maskedValue = value & (255 << 8);
			break;
		case 3: 
			maskedValue = value & (255 << 16);
			break;
		case 4: 
			maskedValue = value & (255 << 24);
			break;
		default:
		{
			cout << "You have specified wrong byte index.\n";
		}
	}

	unsigned int outCount = 0;
	unsigned int mask = 1 << 31;
	
	for (unsigned int i = 1; i <= 32; i++)
	{
		outCount += (bool)(maskedValue & mask);
		mask >>= 1;
	}

	InitialValuePrint(value);
	cout << "\nTotal count of ones: " << outCount << endl;
	return outCount;
}

unsigned int CountOnes2(unsigned int value)
{
	unsigned int outCount = 0;

	for (int i = 0; i < 32; i++)
		outCount += ((value >> i) & 1);

	InitialValuePrint(value);
	cout << "\nTotal count of ones: " << outCount << endl;
	return outCount;
}

unsigned int CountOnes3(unsigned int value)
{
	unsigned int outCount = 0;
	unsigned int mask = 1 << 31;
	for (unsigned int i = 1; i <= 32; i++)
	{
		outCount += (bool)(value & mask);
		mask >>= 1;
	}

	InitialValuePrint(value);
	cout << "\nTotal count of ones: " << outCount << endl;
	return outCount;
}

void Separator()
{
	cout << "\n----------------------------------\n";
}

void InitialValuePrint(unsigned value)
{
	cout << "Initial value:\n";
	DisplayBits(value);
}

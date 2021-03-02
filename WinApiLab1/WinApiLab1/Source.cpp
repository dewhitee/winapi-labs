#include <Windows.h>
#include <iostream>
#include <iomanip>

using namespace std;

const unsigned char bitsPerMonth = 4;
const unsigned char bitsPerDays = 5;
const unsigned char bitsPerHours = 5;
const unsigned char bitsPerMinutes = 6;
const unsigned char bitsPerSeconds = 6;

void DisplayBits(unsigned int);
void DisplayAsDecimal(unsigned int);
unsigned int CycleShift(unsigned int);
unsigned int PackDate();
unsigned int PackDate(unsigned month, unsigned day, unsigned hours, unsigned minutes, unsigned seconds);
void UnpackDate(unsigned int packedDate);
unsigned int CountOnes(unsigned int value);
unsigned int CountOnes2(unsigned int value);
unsigned int CountOnes3(unsigned int value);
void Separator();
void InitialValuePrint(unsigned value);

int main()
{
	int val = 2000000000;
	int val2 = 1;
	int val3 = 0xffffffff;
	int shift = 3;

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
	cout << "\n(7.6 first version) Count of ones:\n";
	CountOnes(val);
	Separator();
	cout << "(7.6 second version) Count of ones:\n";
	CountOnes2(val);
	Separator();
	cout << "(7.6 third version) Count of ones:\n";
	CountOnes3(val);

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

void DisplayAsDecimal(unsigned int)
{

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
	//cout << "\nDate (added seconds)\n";
	//DisplayBits(date);
	date |= (minutes << bitsPerSeconds);
	//cout << "Date (added minutes) = " << date << endl;
	//DisplayBits(date);
	date |= (hours << (bitsPerSeconds + bitsPerMinutes));
	//cout << "Date (added hours) = " << date << endl;
	//DisplayBits(date);
	date |= (day << (bitsPerSeconds + bitsPerMinutes + bitsPerHours));
	//cout << "Date (added days)\n";
	//DisplayBits(date);
	date |= (month << (bitsPerSeconds + bitsPerMinutes + bitsPerHours + bitsPerDays));
	//cout << "Date (added month)\n";
	//DisplayBits(date);

	// Print date value as decimal
	cout << "\nPack result:";
	cout << "\nMonth: " << month << ", Days: " << day << ", Hours: " << hours << ", Minutes: " << minutes << ", Seconds: " << seconds << endl;

	// Print date value as binary
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

unsigned int CountOnes(unsigned int value)
{
	// Hard method
	// Returns ones count for specified value
	auto getOnesCount = [](unsigned char val) {
		unsigned int outCount = 0;
		for (unsigned char i = 0; i < 8; i++)
			outCount += ((val >> i) & 1);

		return outCount;
	};

	unsigned char bitArray[256];

	// Fill bit array
	for (unsigned int i = 0; i < 256; i++)
	{
		bitArray[i] = getOnesCount(i);
		//cout << "Value = " << i << ", count of bits = " << (int)bitArray[i] << endl;
	}

	// given a value == 10100100 00000000 11110000 10101010		-- our value
	//				   &00000000 00000000 00000000 11111111		-- 255 mask
	//				    -----------------------------------
	//					00000000 00000000 00000000 10101010 == index

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

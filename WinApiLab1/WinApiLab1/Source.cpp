#include <Windows.h>
#include <iostream>
#include <iomanip>

// Only for testing purposes
#include <bitset>

using namespace std;

void DisplayBits(unsigned int);
void DisplayAsDecimal(unsigned int);
unsigned int CycleShift(unsigned int);
unsigned int PackDate(); // unsigned ints
void UnpackDate(unsigned int packedDate);
unsigned char CountOnes(unsigned int value);

int main()
{
	int val = 2000000000;
	int shift = 3;
	

	// 7.3
	DisplayBits(val);

	// 7.4
	cout << "(7.4) Shifted: " << CycleShift(val) << endl;

	// 7.5.1
	unsigned int outDate = PackDate();
	cout << "(7.5) Packed date: " << outDate << endl;

	// 7.5.2
	UnpackDate(outDate);

	// 7.6
	cout << "(7.6) Count of ones: " << (int)CountOnes(val) << endl;

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
		{
			cout << ' ';
		}
	}
	cout << endl;
}

void DisplayAsDecimal(unsigned int)
{

}

unsigned int CycleShift(unsigned int value)
{
	unsigned int shift = 8;

	//value = 0b01011111 00001111 01010101 00001111;
	///value >>= 8;
	// 00000000 01011111 00001111 01010101

	// left shift
	//value = 0b01011111 00001111 01010101 00001111;
	// shift = 8
	unsigned int mask = 1 << 31; // 1000... 31 bits
	unsigned int rightShift = 1;
	//value >>= rightShift;
	//value >>= 1;

	// if value's first bit is 1 we need to add 1 to the value
	if (value & mask)
	{
		//value <<= 1;
		value >>= rightShift;
		value |= 1;
	}
	else
	{
		//value <<= 1;
		value >>= rightShift;
	}
	
	return value;
}

const unsigned char bitsPerMonth = 4;
const unsigned char bitsPerDays = 5;
const unsigned char bitsPerHours = 5;
const unsigned char bitsPerMinutes = 6;
const unsigned char bitsPerSeconds = 6;

// Returns date
unsigned int PackDate()
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
	cout << "Date (added seconds)\n";
	DisplayBits(date);
	date |= (minutes << bitsPerSeconds);
	cout << "Date (added minutes) = " << date << endl;
	DisplayBits(date);
	date |= (hours << (bitsPerSeconds + bitsPerMinutes));
	cout << "Date (added hours) = " << date << endl;
	DisplayBits(date);
	date |= (day << (bitsPerSeconds + bitsPerMinutes + bitsPerHours));
	cout << "Date (added days)\n";
	DisplayBits(date);
	date |= (month << (bitsPerSeconds + bitsPerMinutes + bitsPerHours + bitsPerDays));
	cout << "Date (added month)\n";

	// Adding hours
	///date = hours;

	// Adding minutes
	///date |= (minutes << 5);

	// Adding seconds
	///date |= (seconds << 11);

	// Print date value as decimal


	// Print date value as binary
	DisplayBits(date);

	return date;
}

void UnpackDate(unsigned int packedDate)
{
	const bool displayAllBits = false;

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

	//unsigned int date = 0;

	///unsigned int hours = date & 31;
	// 31 = 0001 1111
	// 63 = 0011 1111
	///unsigned int minutes = (date & (63 << 5)) >> 5; // 5 as we need 5 bits for HOURS
	///unsigned int seconds = (date & (63 << 11)) >> 11; // 6 as we need 6 bits for MINUTES

	cout << "\n\nUnpacking date:\n";
	DisplayBits(packedDate);

	/*const unsigned int secondsReservedBits = 63;
	const unsigned int seconds = packedDate & secondsReservedBits;
	cout << "Seconds = " << packedDate << " & " << secondsReservedBits << " = " << seconds << endl;
	DisplayBits(packedDate);
	DisplayBits(secondsReservedBits);
	DisplayBits(seconds);*/
	unsigned int seconds = unpackFor(63, 0);
	unsigned int minutes = unpackFor(63, bitsPerSeconds);
	unsigned int hours = unpackFor(31, bitsPerSeconds + bitsPerMinutes);
	unsigned int days = unpackFor(31, bitsPerSeconds + bitsPerMinutes + bitsPerHours);
	unsigned int month = unpackFor(31, bitsPerSeconds + bitsPerMinutes + bitsPerHours + bitsPerDays);

	//const unsigned int minutesShift = bitsPerSeconds;
	//const unsigned int minutesReservedBits = 63;
	//const unsigned int minutes = (packedDate & (minutesReservedBits << minutesShift)) >> minutesShift;
	//cout << "Minutes = " << packedDate << " & (" << minutesReservedBits << " << " << minutesShift << ") >> " << minutesShift << " = " << minutes << endl;
	//DisplayBits(packedDate);
	//DisplayBits((minutesReservedBits << minutesShift));
	//DisplayBits(minutes);

	//const unsigned int hoursShift = bitsPerSeconds + bitsPerMinutes;
	//const unsigned int hoursReservedBits = 31;
	//const unsigned int hours = (packedDate & (hoursReservedBits << hoursShift)) >> hoursShift;
	//cout << "Hours = " << packedDate << " & (" << hoursReservedBits << " << " << hoursShift << ") = " << hours << endl;
	//DisplayBits(packedDate);
	//DisplayBits((hoursReservedBits << hoursShift));
	//DisplayBits(hours);

	//const unsigned int daysShift = bitsPerSeconds + bitsPerMinutes + bitsPerHours;
	//const unsigned int daysReservedBits = 31;
	//const unsigned int days = (packedDate & (daysReservedBits << daysShift)) >> daysShift;
	//cout << "Days = " << packedDate << " & (" << daysReservedBits << " << " << daysShift << ") >> " << daysShift << " = " << days << endl;
	//DisplayBits(packedDate);
	//DisplayBits((daysReservedBits << (daysShift)));
	//DisplayBits(days);

	//const unsigned int monthShift = bitsPerSeconds + bitsPerMinutes + bitsPerHours + bitsPerDays;
	//const unsigned int monthReservedBits = 31;
	//const unsigned int month = (packedDate & (monthReservedBits << monthShift)) >> monthShift;
	//cout << "Months = " << packedDate << " & (" << monthReservedBits << " << " << monthShift << ") >> " << monthShift << " = " << month << endl;
	//DisplayBits(packedDate);
	//DisplayBits((monthReservedBits << (monthShift)));
	//DisplayBits(month);

	// Print as decimal
	cout << "\nUnpack result:\n";
	cout << "Month: " << month << ", Days: " << days << ", Hours: " << hours << ", Minutes: " << minutes << ", Seconds: " << seconds << endl;

	// Print as binary
}

unsigned char CountOnes(unsigned int value)
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
	return bitArray[value & 255]
		+ bitArray[(value >> 8) & 255] // or; bitArray[(value & (255 << 8)) >> 8]
		+ bitArray[(value >> 16) & 255] // checking third byte
		+ bitArray[(value >> 24) & 255]; // checking fourth byte
}

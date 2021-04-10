#include <iostream>
#include <iomanip>
#include <windows.h>
#include <string>
#include <system_error>
#include <vector>
#include <algorithm>

// Deniss Belovs 4801BD

using namespace std;

struct RegistryKeys
{
	HKEY rootKey;
	CONST WCHAR* subKey;
	WCHAR* name;
};

struct AdditionalArgs
{
	BOOL printSorted = FALSE;
	BOOL printOnlyFiltered = FALSE;
	BOOL cutTooLargeData = TRUE;
};

struct RegistryKeyContent
{
	wstring valueName;
	wstring keyContent;
	size_t filterNamePosition = wstring::npos;
	size_t filterNameLen = 0;
};

enum HKeys : UCHAR
{
	INPUT_HKEY_CLASSES_ROOT,
	INPUT_HKEY_CURRENT_USER,
	INPUT_HKEY_LOCAL_MACHINE,
	INPUT_HKEY_USERS,
	INPUT_HKEY_CURRENT_CONFIG,
};

HKEY InputGetHKEY();
RegistryKeys InputRegistryKeys();
AdditionalArgs InputAdditionalArgs();

wstring GetRegistryKeyContent(PHKEY hKey, LPWSTR valueName, DWORD& statusCode, CONST AdditionalArgs& additional);
VOID EnumerateRegistryValues(RegistryKeys keys, CONST AdditionalArgs& additional);
VOID PrintRegGetValueStatus(BOOL printCondition, DWORD& status);
VOID PrintRegEnumValueStatus(BOOL printCondition, DWORD& status);
VOID PrintRegistryKeysContents(vector<RegistryKeyContent>& registryKeysContents, CONST AdditionalArgs& additional, BOOL filterValues);
BOOL PrintRegistryRowStart(DWORD index, LPWSTR valueName, BOOL highlightFilterValue, CONST AdditionalArgs& additional, size_t filterPosition = wstring::npos, size_t filterNameLen = 0);
wstring GetRootKeyString(HKEY hKey);
wstring ToLowerCase(LPWSTR lpWstr);

int main()
{
	int input = 0;
	do
	{
		RegistryKeys registryKeys = InputRegistryKeys();
		AdditionalArgs additional = InputAdditionalArgs();
		wcout << "\nRoot key: " << GetRootKeyString(registryKeys.rootKey) 
			<< ",\nSubkey: " << registryKeys.subKey 
			<< ",\nValue name must include: " << (wstring(registryKeys.name).empty() ? L"" : registryKeys.name)
			<< ",\nSorted: " << (additional.printSorted ? L"true" : L"false") 
			<< ",\nOnly filtered: " << (additional.printOnlyFiltered ? L"true" : L"false")
			<< ",\nCut too long values: " << (additional.cutTooLargeData ? L"true" : L"false")
			<< endl << endl;
		EnumerateRegistryValues(registryKeys, additional);
		cout << "\n --- Again? (0 to exit) > "; cin >> input;
		system("cls");
	} while (input);

	system("pause");
	return 0;
}

HKEY InputGetHKEY()
{
	ULONG rootKeyValue;
	wcout << "Choose the root (hive) key: \n"
		<< INPUT_HKEY_CLASSES_ROOT << " = HKEY_CLASSES_ROOT\n"
		<< INPUT_HKEY_CURRENT_USER << " = HKEY_CURRENT_USER\n"
		<< INPUT_HKEY_LOCAL_MACHINE << " = HKEY_LOCAL_MACHINE\n"
		<< INPUT_HKEY_USERS << " = HKEY_USERS\n"
		<< INPUT_HKEY_CURRENT_CONFIG << " = HKEY_CURRENT_CONFIG\n"
		<< " > "; cin >> rootKeyValue;
	cin.ignore();
	switch (rootKeyValue)
	{
	case INPUT_HKEY_CLASSES_ROOT:
		return HKEY_CLASSES_ROOT;
	case INPUT_HKEY_CURRENT_USER:
		return HKEY_CURRENT_USER;
	case INPUT_HKEY_LOCAL_MACHINE:
		return HKEY_LOCAL_MACHINE;
	case INPUT_HKEY_USERS:
		return HKEY_USERS;
	case INPUT_HKEY_CURRENT_CONFIG:
		return HKEY_CURRENT_CONFIG;
	default:
		return 0;
	}
}

RegistryKeys InputRegistryKeys()
{
	const HKEY rootKey = InputGetHKEY();
	constexpr UINT8 len = 255;
	WCHAR* searchKey = new WCHAR[len], *name = new WCHAR[len];

	wcout << "Type the subkey. Hint: type the path with backslashes like 'Control Panel\\Desktop'.\n > "; wcin.getline(searchKey, len);
	wcout << "Type the full or partial name of a value to filter search results (optional, press ENTER to skip)\n > "; wcin.getline(name, len);
	return RegistryKeys{ rootKey, searchKey, name };
}

AdditionalArgs InputAdditionalArgs()
{
	AdditionalArgs outArgs;
	AdditionalArgs defaultArgs;

	WCHAR printSorted;
	wcout << "Print sorted? (default is false, press ENTER to default or 't' to enable sorting)\n > "; 
	wcin.get(printSorted);
	outArgs.printSorted = towlower(printSorted) == L't' ? !defaultArgs.printSorted : defaultArgs.printSorted;
	if (outArgs.printSorted)
		wcin.ignore();

	WCHAR printOnlyFiltered;
	wcout << "Print only filtered? (default is false, press ENTER to default or 't' to enable printing only filtered)\n > "; 
	wcin.get(printOnlyFiltered);
	outArgs.printOnlyFiltered = towlower(printOnlyFiltered) == L't' ? !defaultArgs.printOnlyFiltered : defaultArgs.printOnlyFiltered;
	if (outArgs.printOnlyFiltered)
		wcin.ignore();

	WCHAR cutTooLargeData;
	wcout << "Cut too large data? (default is true, press ENTER to default or 'f' to disable cutting too large data)\n > "; 
	wcin.get(cutTooLargeData);
	outArgs.cutTooLargeData = towlower(cutTooLargeData) == L'f' ? !defaultArgs.cutTooLargeData : defaultArgs.cutTooLargeData;

	return outArgs;
}

wstring GetRegistryKeyContent(PHKEY hKey, LPWSTR valueName, DWORD& statusCode, CONST AdditionalArgs& additional)
{
	constexpr UINT16 valueLength = 1024*2;
	constexpr UINT8 valueTypeNameLength = 14;
	constexpr UINT8 valueStringLength = 60;

	WCHAR registryString[valueLength * 2] = L"";
	WCHAR valueBuffer[valueLength];
	PVOID dataPtr = valueBuffer;
	DWORD dataType;
	DWORD bufferSize = sizeof(valueBuffer);

	DWORD flags = RRF_RT_ANY	// Set no restriction to value type
		| RRF_RT_REG_EXPAND_SZ	// Need for REG_EXPAND_SZ type to work. Without those flags REG_SZ
		| RRF_NOEXPAND;			// value type is used only.

	if ((statusCode = RegGetValue(*hKey, NULL, valueName, flags, &dataType, dataPtr, &bufferSize)) == ERROR_SUCCESS)
	{
		switch (dataType)
		{
			case REG_DWORD:
			{
				DWORD dataValue = *(PDWORD)dataPtr;
				swprintf_s(registryString, L"%*s | 0x%0*x (%u)\n", valueTypeNameLength, L"REG_DWORD", 8, dataValue, dataValue);
				break;
			}
			case REG_QWORD:
			{
				DWORD64 dataValue = *(PDWORD64)dataPtr;
				swprintf_s(registryString, L"%*s | 0x%0*llx (%llu)\n", valueTypeNameLength, L"REG_QWORD", 8, dataValue, dataValue);
				break;
			}
			case REG_MULTI_SZ:
			{
				PWSTR ptr = (PWSTR)dataPtr;
				wstring wstr;
				while (*ptr)
				{
					wstring currentWstr = wstring(ptr);
					wstr.append(currentWstr + L" ");
					ptr += currentWstr.size() + 1;
				}
				if (!additional.cutTooLargeData)
				{
					swprintf_s(registryString, L"%*s | %s\n", valueTypeNameLength, L"REG_MULTI_SZ", 
						wstr.c_str());
				}
				else
				{
					swprintf_s(registryString, L"%*s | %.*s\n", valueTypeNameLength, L"REG_MULTI_SZ",
						valueStringLength, wstr.c_str());
				}
				break;
			}
			case REG_EXPAND_SZ:
			{
				if (!additional.cutTooLargeData)
				{
					swprintf_s(registryString, L"%*s | (size=%d) %s\n", valueTypeNameLength, L"REG_EXPAND_SZ", bufferSize, 
						(PWSTR)dataPtr);
				}
				else
				{
					swprintf_s(registryString, L"%*s | (size=%d) %.*s\n", valueTypeNameLength, L"REG_EXPAND_SZ", bufferSize, 
						valueStringLength, (PWSTR)dataPtr);
				}
				break;
			}
			case REG_SZ:
			{
				if (!additional.cutTooLargeData)
				{
					swprintf_s(registryString, L"%*s | %s\n", valueTypeNameLength, L"REG_SZ", (PWSTR)dataPtr);
				}
				else
				{
					swprintf_s(registryString, L"%*s | %.*s\n", valueTypeNameLength, L"REG_SZ",
						valueStringLength, (PWSTR)dataPtr);
				}
				break;
			}
			case REG_BINARY:
			{
				LPBYTE ptr = (LPBYTE)dataPtr;
				wstring wstr;
				for (size_t i = 0; i < bufferSize; i++)
				{
					WCHAR s[3];
					swprintf_s(s, L"%0*x", 2, (BYTE)*ptr++);
					wstr.append(wstring(s) + L" ");
				}
				if (!additional.cutTooLargeData)
				{
					swprintf_s(registryString, L"%*s | (size=%d) %s\n", valueTypeNameLength, L"REG_BINARY", bufferSize, 
						wstr.c_str());
				}
				else
				{
					swprintf_s(registryString, L"%*s | (size=%d) %.*s\n", valueTypeNameLength, L"REG_BINARY", bufferSize,
						valueStringLength, wstr.c_str());
				}
				break;
			}
			default:
			{
				swprintf_s(registryString, L"%*s | %s\n", valueTypeNameLength, L"UNKNOWN", L"UNKNOWN");
				break;
			}
		}
	}
	else
	{
		return L"Required buffer size is " + to_wstring(bufferSize) + L",";
	}

	return wstring(registryString);
}

VOID EnumerateRegistryValues(RegistryKeys keys, CONST AdditionalArgs& additional)
{
	HKEY hKey = NULL;
	if (RegOpenKeyEx(keys.rootKey, keys.subKey, NULL, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
	{
		cout << "Error! Bad subkey (search key)!\n";
		return;
	}

	const WCHAR line[] = L"----------------------------------------------------------------------------------------";
	wprintf(L"%*s | %*s | %*s | Value data\n%s\n", 6, L"Index", 42, L"Value name", 14, L"Value type", line);

	wstring filterValueName = ToLowerCase(keys.name);
	BOOL filterValues = !filterValueName.empty();

	constexpr int bytesPerValueName = 261;
	LPWSTR valueName = (LPWSTR)calloc(bytesPerValueName, sizeof(WCHAR));

	DWORD valueIndex = 0;
	DWORD valueNameBuffer = bytesPerValueName;
	DWORD typeCode;
	DWORD regEnumValueCode;

	vector<RegistryKeyContent> registryKeysContents;

	while ((regEnumValueCode = RegEnumValue(hKey, valueIndex, valueName, &valueNameBuffer, NULL, &typeCode, NULL, NULL)) != ERROR_NO_MORE_ITEMS)
	{
		DWORD regGetValueCode;
		wstring content = GetRegistryKeyContent(&hKey, valueName, regGetValueCode, additional);
		if (additional.printSorted || filterValues)
		{
			registryKeysContents.push_back(RegistryKeyContent
			{ 
				valueName, 
				content, 
				filterValues ? ToLowerCase(valueName).find(filterValueName) : wstring::npos,
				filterValueName.size()
			});
		}
		else
		{
			PrintRegistryRowStart(valueIndex, valueName, FALSE, AdditionalArgs{});
			wprintf(content.c_str());
		}

		valueNameBuffer += bytesPerValueName;
		valueName = (LPWSTR)realloc(valueName, valueNameBuffer * sizeof(WCHAR));

		PrintRegGetValueStatus(regGetValueCode != ERROR_SUCCESS, regGetValueCode);
		PrintRegEnumValueStatus(regEnumValueCode != ERROR_SUCCESS, regEnumValueCode);

		valueIndex++;
	}

	PrintRegistryKeysContents(registryKeysContents, additional, filterValues);

	wcout << line << endl;
	PrintRegEnumValueStatus(true, regEnumValueCode);

	if (valueName != NULL)
		free(valueName);

	RegCloseKey(hKey);
}

VOID PrintRegGetValueStatus(BOOL printCondition, DWORD& status)
{
	if (status != ERROR_SUCCESS)
		cout << "\tRegGetValueStatusCode = " << system_category().message(status) << endl;
}

VOID PrintRegEnumValueStatus(BOOL printCondition, DWORD& status)
{
	if (status != ERROR_SUCCESS)
		cout << "\tRegEnumValueStatusCode = " << system_category().message(status) << endl;
}

VOID PrintRegistryKeysContents(vector<RegistryKeyContent>& registryKeysContents, CONST AdditionalArgs& additional, BOOL filterValues)
{
	if (!registryKeysContents.empty())
	{
		if (additional.printSorted)
		{
			sort(registryKeysContents.begin(), registryKeysContents.end(),
				[](CONST RegistryKeyContent& lRkc, CONST RegistryKeyContent& rRkc)
				{
					return lRkc.valueName.compare(rRkc.valueName) < 0; // ascended
				}
			);
		}
		for (size_t i = 0; i < registryKeysContents.size(); i++)
		{
			BOOL success = PrintRegistryRowStart(i,
				const_cast<LPWSTR>(registryKeysContents[i].valueName.c_str()),
				filterValues,
				additional,
				registryKeysContents[i].filterNamePosition,
				registryKeysContents[i].filterNameLen);

			if (success)
				wprintf(registryKeysContents[i].keyContent.c_str());
		}
	}
}

BOOL PrintRegistryRowStart(DWORD index, LPWSTR valueName, BOOL highlightFilterValue, CONST AdditionalArgs& additional, size_t filterPosition, size_t filterNameLen)
{
	constexpr UINT8 indexWidth = 6;
	constexpr UINT8 valueNameWidth = 42;

	if (highlightFilterValue && filterPosition != wstring::npos && filterNameLen > 0)
	{
		HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);

		wprintf(L"%*d | ", indexWidth, index);

		wstring wstr = wstring(valueName);
		size_t colorIndex = filterPosition;
		size_t colorLen = 1;
		wprintf(L"%*s", valueNameWidth - wstr.size(), L"");
		for (size_t i = 0; i < wstr.size(); i++)
		{
			if (i == colorIndex)
			{				
				SetConsoleTextAttribute(color, FOREGROUND_GREEN);
				if (colorLen < filterNameLen)
				{
					colorIndex++;
					colorLen++;
				}
			}
			wprintf(L"%c", wstr[i]);
			SetConsoleTextAttribute(color, 7);
		}
		wprintf(L" | ");
		return TRUE;
	}
	else if (!additional.printOnlyFiltered)
	{
		if (!additional.cutTooLargeData)
		{
			wprintf(L"%*d | %*s | ", indexWidth, index, valueNameWidth, valueName);
		}
		else
		{
			wprintf(L"%*d | %*.*s | ", indexWidth, index, valueNameWidth, valueNameWidth, valueName);
		}
		return TRUE;
	}
	return FALSE;
}

wstring GetRootKeyString(HKEY hKey)
{
	switch ((ULONG)hKey)
	{
	case (ULONG)HKEY_CLASSES_ROOT:
		return L"HKEY_CLASSES_ROOT";
	case (ULONG)HKEY_CURRENT_USER:
		return L"HKEY_CURRENT_USER";
	case (ULONG)HKEY_LOCAL_MACHINE:
		return L"HKEY_LOCAL_MACHINE";
	case (ULONG)HKEY_USERS:
		return L"HKEY_USERS";
	case (ULONG)HKEY_CURRENT_CONFIG:
		return L"HKEY_CURRENT_CONFIG";
	default:
		return L"UNSPECIFIED_ROOT_KEY";
	}
}

wstring ToLowerCase(LPWSTR lpWstr)
{
	wstring wstr = wstring(lpWstr);
	transform(wstr.begin(), wstr.end(), wstr.begin(), towlower);
	return wstr;
}

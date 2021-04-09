#include <iostream>
#include <iomanip>
#include <windows.h>
#include <string>
#include <system_error>
#include <vector>
#include <algorithm>
#include <bitset>
#include <codecvt> // for conversion from string to wstring

// Deniss Belovs 4801BD

using namespace std;

struct RegistryKeys
{
	HKEY rootKey;
	const WCHAR* subKey;
	WCHAR* name;
};

struct AdditionalArgs
{
	bool printSorted = false;
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

wstring GetRegistryKeyContent(RegistryKeys keys, PHKEY hKey, LPWSTR valueName, DWORD& statusCode);
VOID EnumerateRegistryValues(RegistryKeys keys, const AdditionalArgs& additional);
VOID PrintRegGetValueStatus(BOOL printCondition, DWORD& status);
VOID PrintRegEnumValueStatus(BOOL printCondition, DWORD& status);
VOID PrintRegistryRowStart(DWORD index, LPWSTR valueName);
wstring GetRootKeyString(HKEY hKey);

int main()
{

	int input = 0;
	do
	{
		RegistryKeys registryKeys = InputRegistryKeys();
		AdditionalArgs additional = InputAdditionalArgs();
		wcout << "\nRoot key: " << GetRootKeyString(registryKeys.rootKey) 
			<< ",\nSubkey: " << registryKeys.subKey 
			<< ",\nField: " << (wstring(registryKeys.name).empty() ? L"All fields" : registryKeys.name)
			<< ",\nSorted: " << (additional.printSorted ? L"true" : L"false") << endl << endl;
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
	wcout << "Type the root (hive) key: \n"
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

	wcout << "Type the subkey > "; wcin.getline(searchKey, len);
	wcout << "Type the name of a value (optional, press ENTER to skip)\n > "; wcin.getline(name, len);
	return RegistryKeys{ rootKey, searchKey, name };
}

AdditionalArgs InputAdditionalArgs()
{
	AdditionalArgs outArgs;
	AdditionalArgs defaultArgs;
	WCHAR printSorted;

	wcout << "Print sorted? (default is false, press ENTER to default or 't' to enable sorting)\n > "; wcin.get(printSorted);
	outArgs.printSorted = towlower(printSorted) == L't' ? true : defaultArgs.printSorted;

	return outArgs;
}

wstring GetRegistryKeyContent(RegistryKeys keys, PHKEY hKey, LPWSTR valueName, DWORD& statusCode)
{
	constexpr UINT16 valueLength = 1024;
	WCHAR registryString[valueLength * 2] = L"";
	WCHAR value[valueLength];
	PVOID data = value;
	DWORD dataType;
	DWORD bufferSize = sizeof(value);

	DWORD flags = RRF_RT_ANY	// Set no restriction to value type
		| RRF_RT_REG_EXPAND_SZ	// Need for REG_EXPAND_SZ type to work. Without those flags REG_SZ
		| RRF_NOEXPAND;			// value type is used only.

	if ((statusCode = RegGetValue(*hKey, NULL, valueName, flags, &dataType, data, &bufferSize)) == ERROR_SUCCESS)
	{
		switch (dataType)
		{
		case REG_DWORD:
			swprintf_s(registryString, L"%*s | 0x%0*x (%u)\n", 16, L"REG_DWORD", 8, *(DWORD*)data, *(DWORD*)data);
			break;
		case REG_EXPAND_SZ:
			swprintf_s(registryString, L"%*s | (size=%d) %s\n", 16, L"REG_EXPAND_SZ", bufferSize, (PWSTR)data);
			break;
		case REG_SZ:
			swprintf_s(registryString, L"%*s | %s\n", 16, L"REG_SZ", (PWSTR)data);
			break;
		case REG_BINARY:
			string str = bitset<16>(*(BYTE*)data).to_string();
			wstring wstr = wstring_convert<codecvt_utf8<WCHAR>>().from_bytes(str);
			swprintf_s(registryString, L"%*s | %s\n", 16, L"REG_BINARY", const_cast<WCHAR*>(wstr.c_str()));
			break;
		}
	}
	else
	{
		return L"Required buffer size is " + to_wstring(bufferSize) + L",";
	}
	return wstring(registryString);
}

VOID EnumerateRegistryValues(RegistryKeys keys, const AdditionalArgs& additional)
{
	HKEY hKey = NULL;
	if (RegOpenKeyEx(keys.rootKey, keys.subKey, NULL, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
	{
		cout << "Error! Bad subkey (search key)!\n";
		return;
	}

	const WCHAR line[] = L"----------------------------------------------------------------------------------------";
	wprintf(L"%*s | %*s | %*s | Value data\n%s\n", 6, L"Index", 42, L"Value name", 16, L"Value type", line);

	if (!wstring(keys.name).empty())
	{
		DWORD regGetValueCode;
		PrintRegistryRowStart(0, keys.name);
		wprintf(GetRegistryKeyContent(keys, &hKey, keys.name, regGetValueCode).c_str());
		wcout << line << endl;
		PrintRegGetValueStatus(true, regGetValueCode);
	}
	else
	{
		constexpr int totalBytes = 261;
		constexpr int byteIncrement = 261;
		LPWSTR valueName = (LPWSTR)calloc(totalBytes, sizeof(WCHAR));

		DWORD valueIndex = 0;
		DWORD valueNameBuffer = totalBytes;
		DWORD typeCode;
		DWORD regEnumValueCode;

		vector<pair<wstring, wstring>> registryKeysContents;

		while ((regEnumValueCode = RegEnumValue(hKey, valueIndex, valueName, &valueNameBuffer, NULL, &typeCode, NULL, NULL)) != ERROR_NO_MORE_ITEMS)
		{
			DWORD regGetValueCode;
			wstring content = GetRegistryKeyContent(keys, &hKey, valueName, regGetValueCode);
			if (additional.printSorted)
			{
				registryKeysContents.push_back(make_pair(valueName, content));
			}
			else
			{
				PrintRegistryRowStart(valueIndex, valueName);
				wprintf(content.c_str());
			}

			valueNameBuffer += byteIncrement;
			valueName = (LPWSTR)realloc(valueName, valueNameBuffer * sizeof(WCHAR));

			PrintRegGetValueStatus(regGetValueCode != ERROR_SUCCESS, regGetValueCode);
			PrintRegEnumValueStatus(regEnumValueCode != ERROR_SUCCESS, regEnumValueCode);

			valueIndex++;
		}

		if (!registryKeysContents.empty())
		{
			sort(registryKeysContents.begin(), registryKeysContents.end());
			for (int i = 0; i < registryKeysContents.size(); i++)
			{
				PrintRegistryRowStart(i, const_cast<LPWSTR>(registryKeysContents[i].first.c_str()));
				wprintf(registryKeysContents[i].second.c_str());
			}
		}

		wcout << line << endl;
		PrintRegEnumValueStatus(true, regEnumValueCode);
		valueIndex = 0;

		if (valueName != NULL)
			free(valueName);
	}
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

VOID PrintRegistryRowStart(DWORD index, LPWSTR valueName)
{
	wprintf(L"%*d | %*s | ", 6, index, 42, valueName);
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

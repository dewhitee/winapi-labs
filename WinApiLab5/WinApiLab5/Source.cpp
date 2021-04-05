#include <iostream>
#include <iomanip>
#include <windows.h>
#include <string>
#include <system_error>

// Deniss Belovs 4801BD

using namespace std;

struct RegistryKeys
{
	HKEY rootKey;
	const WCHAR* searchKey;
	const WCHAR* name;
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
VOID PrintRegistryVariables(wstring keyValues);
wstring GetRegistryKeyContent(RegistryKeys keys);
VOID EnumerateRegistryValues(RegistryKeys keys);

int main()
{

	int input = 0;
	do
	{
		RegistryKeys registryKeys = InputRegistryKeys();
		//EnumerateRegistryValues(registryKeys);
		//PrintRegistryVariables(GetRegistryKeyContent(registryKeys));
		EnumerateRegistryValues(registryKeys);
		cout << "\n --- Again? (0 to exit) > "; cin >> input;
		system("cls");
	} while (input);

	system("pause");
	return 0;
}

HKEY InputGetHKEY()
{
	int rootKeyValue;
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
	HKEY rootKey = InputGetHKEY();
	constexpr UINT8 len = 255;
	WCHAR* searchKey = new WCHAR[len], *name = new WCHAR[len];

	wcout << "Type the search key > "; wcin.getline(searchKey, len);
	wcout << "Type the name of field > "; wcin.getline(name, len);

	wcout << "\nRoot: " << rootKey << ", Search: " << searchKey << ", Field: " << name << endl << endl;
	return RegistryKeys{ rootKey, searchKey, name };
}

VOID PrintRegistryVariables(wstring keyValues)
{
	wcout << "Key value: " << keyValues << endl;
}

wstring GetRegistryKeyContent(RegistryKeys keys)
{
	DWORD bufferSize;
	wstring outData;
	//CHAR* data;
	DWORD data;
	//RegGetValue(keys.rootKey, keys.searchKey, L"FontSize", RRF_RT_ANY, NULL, (PVOID)&outData, &bufferSize);
	RegGetValue(keys.rootKey, keys.searchKey, keys.name, RRF_RT_ANY, NULL, (PVOID)&data, &bufferSize);
	wcout << data << endl;

	outData = to_wstring(data);
	delete keys.searchKey, keys.name;

	return outData;
}

VOID EnumerateRegistryValues(RegistryKeys keys)
{
	HKEY hKey = NULL;
	if (RegOpenKeyEx(keys.rootKey, keys.searchKey, NULL, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
	{
		cout << "Error! Bad subkey (search key)!\n";
		return;
	}

	//DWORD index = 0;
	//WCHAR valueName[16383];
	//DWORD valueNameBuffer;
	//DWORD type;
	//BYTE* data = new BYTE[2048];
	//DWORD dataBuffer;

	constexpr int totalBytes = 261;
	constexpr int byteIncrement = 261;

	DWORD valueIndex = 0;
	LPWSTR valueName = (LPWSTR)calloc(totalBytes, sizeof(WCHAR));
	//LPWSTR valueName = WCHAR[totalBytes];
	DWORD valueNameBuffer = totalBytes;
	DWORD typeCode;
	DWORD regEnumValueCode;

	const WCHAR line[] = L"----------------------------------------------------------------------------------------";
	wprintf(L"%*s | %*s | %*s | valueData\n%s\n", 6, L"Index", 15, L"valueNameBuffer", 32, L"valueName", line);
	//wcout << L"valueIndex | valueNameBuffer | valueName | valueData\n\n";

	while ((regEnumValueCode = RegEnumValue(hKey, valueIndex, valueName, &valueNameBuffer, NULL, &typeCode, NULL, NULL)) != ERROR_NO_MORE_ITEMS)
	{
		//wcout << "\tvalueIndex = " << valueIndex << ", valueNameBuffer = " << valueNameBuffer << " valueName = " << valueName << endl;
		//wcout << setw(16) << valueIndex << " | " << setw(16) << valueNameBuffer << " | " << setw(16) << valueName << " | ";
		wprintf(L"%*d | %*d | %*s | ", 6, valueIndex, 15, valueNameBuffer, 32, valueName);

		WCHAR value[255];
		PVOID pvData = value;
		DWORD dataType;
		DWORD size = sizeof(value);
		DWORD regGetValueCode;

		//wcout << "keys.searchKey = " << keys.searchKey << ", valueName = " << valueName << endl;
		if ((regGetValueCode = RegGetValue(hKey, NULL, valueName, RRF_RT_ANY, &dataType, pvData, &size)) == ERROR_SUCCESS)
		{
			//wcout << "\tdata = " << data << endl;
			switch (dataType)
			{
			case REG_DWORD:
				wprintf(L"%x\n", *(DWORD*)pvData);
				break;
			case REG_SZ:
				wprintf(L"%s\n", (PWSTR)pvData);
				break;
			}
		}
		//else
		//{
		//	wcout << "\tERROR! valueIndex = " << valueIndex << ", hKey = " << hKey << ", keyName = " << keys.searchKey << ", valueName = " << valueName << endl;
		//	cout << "dataLength = " << size << endl;
		//}

		valueNameBuffer += byteIncrement;
		valueName = (LPWSTR)realloc(valueName, valueNameBuffer * sizeof(WCHAR));

		if (regGetValueCode != ERROR_SUCCESS)
			cout << "\titeration regGetValueCode = " << system_category().message(regGetValueCode) << endl;

		if (regEnumValueCode != ERROR_SUCCESS)
			cout << "\titeration regEnumValueCode = " << system_category().message(regEnumValueCode) << endl << endl;

		valueIndex++;
	}
	cout << "\tregEnumValueCode = " << system_category().message(regEnumValueCode) << endl;
	wcout << "\tvalueIndex = " << valueIndex << ", valueName = " << valueName << ", valueNameBuffer = " << valueNameBuffer
		<< ", typeCode = " << typeCode << endl;
	valueIndex = 0;

	if (valueName != NULL)
		free(valueName);

	//delete[] valueName;

	/*DWORD index = 0;
	LPWSTR keyName = new WCHAR[261];
	while (RegEnumKey(hKey, index, keyName, 261) == ERROR_SUCCESS)
	{
		wcout << "Index = " << index << ", keyName = " << keyName << endl;
		DWORD valueIndex = 0;
		LPWSTR valueName = new WCHAR[8192];
		DWORD valueNameBuffer;
		DWORD typeCode;
		DWORD regEnumValueCode;
		while ((regEnumValueCode = RegEnumValue(hKey, valueIndex, valueName, &valueNameBuffer, NULL, &typeCode, NULL, NULL)) == ERROR_SUCCESS)
		{
			LPBYTE data = new BYTE[8192];
			DWORD dataLength;
			DWORD regGetValueCode;
			if ((regGetValueCode = RegGetValue(hKey, keyName, valueName, RRF_RT_ANY, NULL, (PVOID)&data, &dataLength)) == ERROR_SUCCESS)
			{
				wcout << "\tdata = " << data << endl;
			}
			else
			{
				wcout << "\tvalueIndex = " << valueIndex << ", hKey = " << hKey << ", keyName = " << keyName << ", valueName = " << valueName << endl;
				cout << "\tregGetValueCode = " << system_category().message(regGetValueCode) << endl;
				cout << "dataLength = " << dataLength << endl;
			}
			delete[] data;
			valueIndex++;
		}
		valueIndex = 0;
		cout << "\tregEnumValueCode = " << system_category().message(regEnumValueCode) << endl;
		cout << "valueNameBuffer = " << valueNameBuffer << endl;
		index++;
		delete[] valueName;
	}*/
	RegCloseKey(hKey);

	//while (DWORD status = RegEnumValue(keys.rootKey, index, valueName, &valueNameBuffer, NULL, &type, data, &dataBuffer) != ERROR_NO_MORE_ITEMS)
	//{
	//	if (status != ERROR_SUCCESS)
	//	{
	//		cout << "ERROR!" << endl;
	//		
	//		break;
	//	}
	//	wcout << "Index = " << index << ", valueName = " << valueName << ", data = " << data << endl;
	//	index++;
	//}
	//delete data;
}

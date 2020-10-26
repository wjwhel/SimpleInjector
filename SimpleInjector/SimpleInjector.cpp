#include <Windows.h>
#include <tlhelp32.h>
#include <iostream>

int main()
{
	const wchar_t* process = L"csgo.exe";	// Process to inject into
	const wchar_t* file = L"InHop.dll";		// Dll to inject

	DWORD processId = 0u;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		std::cout << "Invalid Handle";
		return 0;
	}

	PROCESSENTRY32 processEntry;
	processEntry.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnapshot, &processEntry))
	{
		do
		{
			if (!wcscmp(processEntry.szExeFile, process))
			{
				processId = processEntry.th32ProcessID;
				CloseHandle(hSnapshot);
				break;
			}
		} while (Process32Next(hSnapshot, &processEntry));
	}




















	CloseHandle(hSnapshot);
	return 0;
}
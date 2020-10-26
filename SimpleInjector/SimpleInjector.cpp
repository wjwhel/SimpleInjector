#include <Windows.h>
#include <tlhelp32.h>
#include <iostream>

int main()
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		std::cout << "Invalid Handle";
		return 0;
	}

	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnapshot, &pEntry))
	{

	}
}
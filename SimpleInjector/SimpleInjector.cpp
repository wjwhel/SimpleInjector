#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

DWORD getProcessId(const wchar_t* process)
{
	DWORD procID = 0;
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
				procID = processEntry.th32ProcessID;
				break;
			}
		} while (Process32Next(hSnapshot, &processEntry));
	}
	CloseHandle(hSnapshot);
	return procID;
}

int main()
{
	const wchar_t* process = L"csgo.exe";		// Process to inject into
	const wchar_t* dllPath = L"InHop.dll";		// Dll to inject

	// WIP

	DWORD processId = getProcessId(process);

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

	auto baseAddr = VirtualAllocEx(hProcess, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	WriteProcessMemory(hProcess, baseAddr, dllPath, (wcslen(dllPath) + 1) * sizeof(wchar_t), nullptr);

	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryW, baseAddr, 0, NULL);

	// WIP




	return 0;
}
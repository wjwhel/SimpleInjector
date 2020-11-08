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
		Sleep(2000);
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
	constexpr auto process = L"csgo.exe";		// Process to inject into
	constexpr auto dllPath = L"C:\\Users\\warre\\source\\repos\\SimpleInjector\\Debug\\InHop.dll";		// Dll to inject

	DWORD processId = getProcessId(process);

	if (!processId)
	{
		std::cout << "Process is not running.";
		Sleep(2000);
		return 0;
	}

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

	if (hProcess == nullptr)
	{
		std::cout << "Failed to open process.";
		Sleep(2000);
		return 0;
	}

	LPVOID baseAddr = VirtualAllocEx(hProcess, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	if (!baseAddr)
	{
		std::cout << "Failed to inject DLL.";
		Sleep(2000);
		return 0;
	}

	auto wpm = WriteProcessMemory(hProcess, baseAddr, dllPath, (wcslen(dllPath) + 1) * sizeof(wchar_t), nullptr);

	if (!wpm)
	{
		std::cout << "Failed to inject DLL.";
		Sleep(2000);
		if (baseAddr)
			VirtualFreeEx(hProcess, baseAddr, 0, MEM_RELEASE);

		return 0;
	}

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryW, baseAddr, 0, NULL);

	if (!hThread)
	{
		std::cout << "Failed to inject DLL.";
		Sleep(2000);
		if (baseAddr)
			VirtualFreeEx(hProcess, baseAddr, 0, MEM_RELEASE);

		return 0;
	}

	return 0;
}
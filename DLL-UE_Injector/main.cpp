#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

DWORD GetProcessID(const wchar_t* processName) {
    DWORD processID = 0;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(entry);

    if (Process32First(snap, &entry)) {
        do {
            if (!_wcsicmp(entry.szExeFile, processName)) {
                processID = entry.th32ProcessID;
                break;
            }
        } while (Process32Next(snap, &entry));
    }
    CloseHandle(snap);
    return processID;
}

void InjectDLL(DWORD processID, const char* dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    if (hProcess && processID) {
        void* allocatedMemory = VirtualAllocEx(hProcess, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        WriteProcessMemory(hProcess, allocatedMemory, dllPath, strlen(dllPath) + 1, nullptr);
        HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, allocatedMemory, 0, nullptr);
        if (hThread) {
            CloseHandle(hThread);
        }
        CloseHandle(hProcess);
    }
}

int main() {
    const wchar_t* processName = L"RooftopsandAlleys-Win64-Shipping.exe";  // Game's process name
    const char* dllPath = "C:\\Users\\lpmag\\Bureau\\DLL-Test.dll";  // Path of DLL

    DWORD processID = GetProcessID(processName);
    if (processID) {
        InjectDLL(processID, dllPath);
    }
    else {
        std::cout << "Process not found";
    }
    return 0;
}

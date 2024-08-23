#include <Windows.h> // Required for too much things
#include <TlHelp32.h>
#include <string> // Required for strings..
#include <commdlg.h>  // Required for GetOpenFileName

// Function prototypes
DWORD GetProcessID(const std::wstring& exePath);
void InjectDLL(DWORD processID, const std::wstring& dllPath);
std::wstring SelectFile(bool isDll);

int main() {
    // Message box to prompt for executable path
    MessageBox(nullptr, L"Select the executable file you want to inject your DLL into. \n \n Exemple: Game.exe", L"Executable Selection", MB_OK);

    std::wstring exePath = SelectFile(false); // Use false for executable filter

    if (exePath.empty()) {
        MessageBox(nullptr, L"No executable selected, exiting.", L"Error", MB_OK | MB_ICONERROR);
        return 1;  // Exit if no executable file was selected
    }

    // Message box to prompt for DLL path
    MessageBox(nullptr, L"Select the DLL file you want to inject inside your executable. \n \n Exemple: DynamicLibrary.dll", L"DLL Selection", MB_OK);

    std::wstring dllPath = SelectFile(true); // Use true for DLL filter

    if (!dllPath.empty()) {
        DWORD processID = GetProcessID(exePath);
        if (processID) {
            InjectDLL(processID, dllPath);
            MessageBox(nullptr, L"DLL injected successfully.", L"Injection done!", MB_OK);
        }
        else {
            MessageBox(nullptr, L"The process was not found, make sure it's opened.", L"Error", MB_OK | MB_ICONERROR);
        }
    }
    return 0;
}

DWORD GetProcessID(const std::wstring& exePath) {
    DWORD processID = 0;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(entry);

    std::wstring exeName = exePath.substr(exePath.find_last_of(L"\\") + 1);

    if (Process32First(snap, &entry)) {
        do {
            if (!_wcsicmp(entry.szExeFile, exeName.c_str())) {
                processID = entry.th32ProcessID;
                break;
            }
        } while (Process32Next(snap, &entry));
    }
    CloseHandle(snap);
    return processID;
}

void InjectDLL(DWORD processID, const std::wstring& dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    if (hProcess && processID) {
        void* allocatedMemory = VirtualAllocEx(hProcess, nullptr, MAX_PATH * sizeof(wchar_t), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        WriteProcessMemory(hProcess, allocatedMemory, dllPath.c_str(), (dllPath.size() + 1) * sizeof(wchar_t), nullptr);
        HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)LoadLibraryW, allocatedMemory, 0, nullptr);
        if (hThread) {
            CloseHandle(hThread);
        }
        CloseHandle(hProcess);
    }
}

std::wstring SelectFile(bool isDll) {
    wchar_t filePath[MAX_PATH] = { 0 };

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = sizeof(filePath) / sizeof(wchar_t);

    if (isDll) {
        ofn.lpstrFilter = L"Dynamic link library (.dll)\0*.dll\0"; // DLL filter
    }
    else {
        ofn.lpstrFilter = L"Executable (.exe)\0*.exe\0"; // Executable filter
    }
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

    if (GetOpenFileName(&ofn)) {
        return std::wstring(filePath);
    }
    else {
        return L"";
    }
}

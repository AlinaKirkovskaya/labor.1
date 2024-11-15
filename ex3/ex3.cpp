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

    const DWORD MAX_EXECUTION_TIME = 10000; // 10 секунд
    std::cout << "Process started. Will terminate after 10 seconds if still running..." << std::endl;

    // Очікування завершення процесу з таймаутом
    DWORD waitResult = WaitForSingleObject(pi.hProcess, MAX_EXECUTION_TIME);

    if (waitResult == WAIT_TIMEOUT) {
        std::cout << "Process exceeded time limit. Terminating..." << std::endl;
        if (TerminateProcess(pi.hProcess, 1)) {
            std::cout << "Process terminated successfully" << std::endl;
        }
        else {
            std::cout << "Failed to terminate process. Error: " << GetLastError() << std::endl;
        }
    }
    else if (waitResult == WAIT_OBJECT_0) {
        std::cout << "Process finished naturally within time limit" << std::endl;
    }
    else {
        std::cout << "Wait failed. Error: " << GetLastError() << std::endl;
    }

    // Закриття дескрипторів
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
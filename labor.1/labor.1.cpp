#include <windows.h>
#include <iostream>

int main()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Використовуємо LPCWSTR замість LPCTSTR і додаємо L перед рядком для wide-string
    LPCWSTR programPath = L"C:\\Windows\\System32\\notepad.exe";

    if (!CreateProcess(
        programPath,   // Шлях до програми
        NULL,         // Командний рядок
        NULL,         // Атрибути безпеки процесу
        NULL,         // Атрибути безпеки потоку
        FALSE,        // Наслідування дескрипторів
        0,            // Прапорці створення
        NULL,         // Середовище процесу
        NULL,         // Поточна директорія
        &si,          // Інформація про запуск
        &pi           // Інформація про процес
    )) {
        std::cout << "CreateProcess failed (" << GetLastError() << ")\n";
        return 1;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}
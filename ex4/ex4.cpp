#include <windows.h>
#include <iostream>
#include <vector>

const int MAX_PROCESSES = 3;


int main()
{
    std::vector<PROCESS_INFORMATION> processes(MAX_PROCESSES);
    std::vector<HANDLE> processHandles(MAX_PROCESSES);

    // Створення події для синхронізації
    HANDLE hEvent = CreateEvent(
        NULL,    // Атрибути безпеки за замовчуванням
        TRUE,    // Ручне скидання
        FALSE,   // Початковий стан - несигнальний
        L"ProcessSyncEvent"  // Ім'я події
    );

    if (hEvent == NULL) {
        std::cout << "CreateEvent failed. Error: " << GetLastError() << std::endl;
        return 1;
    }

    // Створення кількох процесів
    STARTUPINFOW si;
    const wchar_t* processNames[] = { L"notepad.exe", L"calc.exe", L"mspaint.exe" };

    for (int i = 0; i < MAX_PROCESSES; i++) {
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&processes[i], sizeof(PROCESS_INFORMATION));

        if (!CreateProcessW(
            processNames[i],
            NULL, NULL, NULL, FALSE, 0, NULL, NULL,
            &si, &processes[i])) {
            std::cout << "Failed to create process " << i << ". Error: "
                << GetLastError() << std::endl;
            continue;
        }

        processHandles[i] = processes[i].hProcess;
        std::cout << "Created process " << i << " with ID: "
            << processes[i].dwProcessId << std::endl;
    }

    // Очікування завершення всіх процесів
    std::cout << "Waiting for all processes to complete..." << std::endl;
    DWORD waitResult = WaitForMultipleObjects(
        MAX_PROCESSES,    // Кількість об'єктів
        processHandles.data(),  // Масив дескрипторів
        TRUE,            // Очікувати всі процеси
        INFINITE         // Нескінченне очікування
    );

    if (waitResult == WAIT_FAILED) {
        std::cout << "Wait failed. Error: " << GetLastError() << std::endl;
    }
    else {
        std::cout << "All processes completed" << std::endl;

        // Отримання кодів завершення
        for (int i = 0; i < MAX_PROCESSES; i++) {
            DWORD exitCode;
            if (GetExitCodeProcess(processes[i].hProcess, &exitCode)) {
                std::cout << "Process " << i << " exit code: " << exitCode << std::endl;
            }
        }
    }

    // Встановлення події для синхронізації
    SetEvent(hEvent);

    // Закриття дескрипторів
    for (int i = 0; i < MAX_PROCESSES; i++) {
        CloseHandle(processes[i].hProcess);
        CloseHandle(processes[i].hThread);
    }
    CloseHandle(hEvent);

    return 0;
}

#include "memory.h"
#include <TlHelp32.h>

bool memory::attatch(const WCHAR* process_name)
{
    if (bAttached)
        return true;

    HWND hWnd = FindWindow(NULL, process_name);
    if (hWnd == NULL)
        return false;

    if (!GetWindowThreadProcessId(hWnd, &dwProcessId))
        return false;

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
    if (hProcess == NULL)
        return false;

    bAttached = true;
    return true;
}

void memory::detatch()
{
    if (!bAttached)
        return;

    CloseHandle(hProcess);
    hProcess = NULL;
    dwProcessId = 0;
    bAttached = false;
}


DWORD64 memory::get_module_base(const WCHAR* module_name)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessId);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return 0;

    MODULEENTRY32W me32;
    me32.dwSize = sizeof(MODULEENTRY32W);

    if (Module32FirstW(hSnapshot, &me32))
    {
        do
        {
            if (wcscmp(me32.szModule, module_name) == 0)
            {
                CloseHandle(hSnapshot);
                return (DWORD64)me32.modBaseAddr;
            }
        } while (Module32NextW(hSnapshot, &me32));
    }

    CloseHandle(hSnapshot);
    return 0;
}

DWORD64 memory::read_chain(DWORD64 base, std::vector<DWORD64> offsets)
{
    DWORD64 ptr = 0;
    read(base, ptr);
    for (int i = 0; i < offsets.size() - 1; i++)
        read(ptr + offsets[i], ptr);
    return ptr + offsets.back();
}
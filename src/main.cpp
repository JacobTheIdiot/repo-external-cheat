#include <Windows.h>
#include <iostream>
#include "cheat/mem/memory.h"

bool bInit = false;

static bool init()
{
    if (bInit)
        return true;

    std::cout << "[*] Attaching to R.E.P.O...\n";

    if (!g_memory.attatch(L"R.E.P.O."))
    {
        std::cout << "[-] could not attach to R.E.P.O, is it open?\n";
        std::cin.get(); 
        return false;
    }

    std::cout << "[+] attached successfully!\n";
    bInit = true;
    return true;
}

int main(int argc, char** argv)
{
    if (!init())
        return 1;

    DWORD64 moduleBase = g_memory.get_module_base(L"mono-2.0-bdwgc.dll");
    bool m_bGodMode = false;

    while (true)
    {
        DWORD64 godModeAddr = g_memory.read_chain(moduleBase + 0x0074D1D8,
            { 0x68, 0x290, 0x0, 0x70, 0x0, 0x68, 0xC0 });

        if (GetAsyncKeyState(VK_F1) & 1)
        {
            m_bGodMode = !m_bGodMode;
            std::cout << "[*] God mode: " << (m_bGodMode ? "ON" : "OFF") << "\n";
        }

        g_memory.write(godModeAddr, m_bGodMode);

        Sleep(1);
    }
}
#pragma once
#include <Windows.h>
#include <vector>
class memory
{
private:
	bool bAttached = false;
	HANDLE hProcess = 0;
	DWORD dwProcessId = 0;
	DWORD64 dwModuleAddress = 0;
public:
	bool attatch(const WCHAR* process_name);
	void detatch();
	DWORD64 get_module_base(const WCHAR* module_name);
	DWORD64 read_chain(DWORD64 base, std::vector<DWORD64> offsets);

	template <typename type>
	bool read(DWORD64 address, type& value)
	{
		if (!bAttached)
			return false;
		return ReadProcessMemory(hProcess, (LPCVOID)address, &value, sizeof(type), nullptr);
	}

	template <typename type>
	bool write(DWORD64 address, type& value)
	{
		if (!bAttached)
			return false;
		return WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(type), nullptr);
	}
};
inline memory g_memory;
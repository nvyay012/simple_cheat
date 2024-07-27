#include <Windows.h>
#include <T1Help32.h>
#include <iostream>
#include <tchar.h>
#include <vector>
#include <stdlib.h>

using namespace std;

DWORD GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
    DWORD dwModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
    MODULEENTRY32 ModuleEntry32 = { 0 };
    ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
 
    if (Module32First(hSnapshot, &ModuleEntry32))
    {
        do {
            if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) // if Found Module matches Module we look for -> done!
            {
                dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnapshot, &ModuleEntry32)); // go through Module entries in Snapshot and store in ModuleEntry32
 
 
    }
    CloseHandle(hSnapshot);
    return dwModuleBaseAddress;
}
 
DWORD GetPointerAddress(HWND hwnd, DWORD gameBaseAddr, DWORD address, vector<DWORD> offsets)
{
    DWORD pID = NULL; // Game process ID
    GetWindowThreadProcessId(hwnd, &pID);
    HANDLE phandle = NULL;
    phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (phandle == INVALID_HANDLE_VALUE || phandle == NULL);
 
    DWORD offset_null = NULL;
    ReadProcessMemory(phandle, (LPVOID*)(gameBaseAddr + address), &offset_null, sizeof(offset_null), 0);
    DWORD pointeraddress = offset_null; // the address we need
    for (int i = 0; i < offsets.size() - 1; i++) // we dont want to change the last offset value so we do -1
    {
        ReadProcessMemory(phandle, (LPVOID*)(pointeraddress + offsets.at(i)), &pointeraddress, sizeof(pointeraddress), 0);
    }
    return pointeraddress += offsets.at(offsets.size() - 1); // adding the last offset
}

int main ()
{
    HWND hwnd_AC = FindWindowA(NULL, "Assaultcube");  // getting the handle to he window
    if (hund_AC != FALSE)
    DWORD pID = NULL;
    GetWindowThreadProcessId(hwnd_AC, &pID);
    HANDLE phandle = NULL;   // process handle
    phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (phandle == INVALID_HANDLE_VALUE || phandle == NULL)

    char gamemodule1[] = "ac_client.exe";
    DWORD gamebaseaddress1 = GetModuleBaseAddress(_T(gamemodule1), pID);

    //ammo
    DWORD ammoAddr = 0x000FAB48;
    vector<DWORD> ammoOffsets(0x22C, 0xEC, 0x390, 0x3C8);
    DWORD ammoPtrAddr = GetPointerAddress(hwnd_AC, ganebaseaddress1, ammoAddr);
    // writing Memory 
    while (1)
    {
        int ammo = 1000;
        WriteProcessMemory(phandle, (LPVOID*)(ammoPtrAddr), &ammo, 4, 0);
    }

}

#include <iostream>
#include <ctime>
#include <iomanip>
#include <Windows.h>
#include <string>
#include <tchar.h>
#include <intrin.h>
using namespace std;
#include "hash/md5.hpp"
#include "hash/sha1.hpp"

void GetCpu(string& output)
{
    try
    {
        int CPUInfo[4] = { -1 };
        __cpuid(CPUInfo, 0x80000000);
        unsigned int nExIds = CPUInfo[0];

        char CPUBrandString[0x40] = { 0 };
        for (unsigned int i = 0x80000000; i <= nExIds; ++i)
        {
            __cpuid(CPUInfo, i);
            if (i == 0x80000002)
            {
                memcpy(CPUBrandString,
                    CPUInfo,
                    sizeof(CPUInfo));
            }
            else if (i == 0x80000003)
            {
                memcpy(CPUBrandString + 16,
                    CPUInfo,
                    sizeof(CPUInfo));
            }
            else if (i == 0x80000004)
            {
                memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
            }
        }

        output = CPUBrandString;
    }
    catch (...) { return; }
}


int main()
{

    int cpuinfo[4] = { 0, 0, 0, 0 };
    __cpuid(cpuinfo, 0);
    char16_t hash = 0;
    char16_t* ptr = (char16_t*)(&cpuinfo[0]);
    for (char32_t i = 0; i < 8; i++)
        hash += ptr[i];

    std::cout << "CPU Hash: " << std::to_string(hash).c_str() << std::endl;

    HKEY rKey;
    char Reget[256];
    DWORD RegetPath = sizeof(Reget);
    RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", NULL, KEY_QUERY_VALUE, &rKey);
    RegQueryValueExA(rKey, "BIOSVersion", NULL, NULL, (LPBYTE)&Reget, &RegetPath);
    RegCloseKey(rKey);
    
    std::cout << "Bios version: " <<  md5(Reget).c_str() << std::endl;


    DISPLAY_DEVICEA dd;
    dd.cb = sizeof(DISPLAY_DEVICEA);
    EnumDisplayDevicesA(NULL, 0, &dd, EDD_GET_DEVICE_INTERFACE_NAME);

    string str = string(dd.DeviceString);

    std::cout << "Video: " << str << std::endl;


    string str_1 = "";
    GetCpu(str_1);

    std::cout << "Processor: " << str_1 << std::endl;


    string all = std::to_string(hash).c_str() + str + str_1;

    all = all + md5(Reget).c_str();
    std::cout << "HWID: " << md5(all) << std::endl;
    system("pause");

    return 0;
}


// DriveFunctions.cpp
#include "getLogicalDrives.h"
#include <windows.h>
#include <iostream>

bool isLocalDisk(wchar_t driveLetter) {
    wchar_t drivePath[4] = { driveLetter, L':', L'\\', L'\0' };
    UINT driveType = GetDriveType(drivePath);
    return driveType == DRIVE_FIXED;
}

bool isDVDDrive(wchar_t driveLetter) {
    wchar_t drivePath[4] = { driveLetter, L':', L'\\', L'\0' };
    UINT driveType = GetDriveType(drivePath);
    return driveType == DRIVE_CDROM;
}

bool isUSBDrive(wchar_t driveLetter) {
    wchar_t drivePath[4] = { driveLetter, L':', L'\\', L'\0' };
    UINT driveType = GetDriveType(drivePath);
    return driveType == DRIVE_REMOVABLE;
}

void printDriveInfo() {
    DWORD drives = GetLogicalDrives();

    for (wchar_t c = L'A'; c <= L'Z'; ++c) {
        if (drives & 1) {
            std::wcout << c << L":\\ ";

            if (isLocalDisk(c)) {
                std::wcout << L"(Local Disk)";
            }
            else if (isDVDDrive(c)) {
                std::wcout << L"(DVD Drive)";
            }
            else if (isUSBDrive(c)) {
                std::wcout << L"(USB Drive)";
            }

            std::wcout << L"\n";
        }
        drives >>= 1;
    }
}

std::vector<DriveInfo> getAllDrives() {
    DWORD drives = GetLogicalDrives();
    std::vector<DriveInfo> driveList;

    for (wchar_t c = L'A'; c <= L'Z'; ++c) {
        
        if (drives & 1) {
            DriveInfo info;
            std::wcout << c << L":\\ ";
            info.driveLetter = c;
            if (isLocalDisk(c)) {
                std::wcout << L"(Local Disk)";
                info.driveType = "Local Disk";
            }
            else if (isDVDDrive(c)) {
                std::wcout << L"(DVD Drive)";
                info.driveType = "DVD Drive";
            }
            else if (isUSBDrive(c)) {
                std::wcout << L"(USB Drive)";
                info.driveType = "Removable Disk";
            }

            std::wcout << L"\n";
            driveList.push_back(info);

        }
        drives >>= 1;
    }
    return driveList;

}
#pragma once

#include <windows.h>
#include <string>
#include <vector>

struct DriveInfo {
    std::wstring driveLetter;
    std::string driveType;
};
bool isDVDDrive(wchar_t driveLetter);

bool isUSBDrive(wchar_t driveLetter);
bool isLocalDisk(wchar_t driveLetter);
std::vector<DriveInfo> getAllDrives();
void printDriveInfo();

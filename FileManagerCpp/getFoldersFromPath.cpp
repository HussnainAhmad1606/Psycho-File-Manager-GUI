#include <windows.h>
#include <iostream>
#include "getLogicalDrives.h"
#include "getFoldersFromPath.h"
std::vector<DriveInfo> listFolders(const std::wstring& drivePath) {
    std::vector<DriveInfo> driveList;
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((drivePath + L"\\*").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Error opening directory\n";
        return driveList;
    }

    do {
        if (findFileData.dwFileAttributes) {
            if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0) {
                DriveInfo info;
                // std::wcout << findFileData.cFileName << L"\n";
                info.driveLetter = findFileData.cFileName;
                info.isDirectory = (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;


                driveList.push_back(info);

            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    return driveList;
}

//int main() {
    // Specify the drive letter (e.g., C:)
//    std::wstring driveLetter = L"D:\\Entertainment\\Forza Horizon\\Forza Horizon 4 [FitGirl Repack]";

//    listFolders(driveLetter);
//
//   return 0;
//}

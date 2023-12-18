#include "createNewFolder.h"
#include <Windows.h>
bool createNewFolder(std::wstring folderName, std::wstring currentDirectory) {

    std::wstring folderPath = currentDirectory + L"\\" + folderName;
    if (CreateDirectory(folderPath.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
        std::wcout << L"Folder created successfully: " << currentDirectory << std::endl;
        return true;
    }
    else {
        std::cerr << "Failed to create folder. Error code: " << GetLastError() << std::endl;
        return false;
    }
}
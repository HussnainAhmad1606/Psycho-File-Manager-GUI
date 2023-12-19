#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <iostream>
#include <experimental/filesystem>
#include <string>
#include "copyFiles.h"

namespace fs = std::experimental::filesystem;

void copyFiles(const std::wstring& sourceFolder, const std::wstring& destinationFolder) {
    std::wstring sourcePath = sourceFolder;

    // Create a filesystem path object
    fs::path filePath(sourcePath);

    // Extract the filename from the path
    std::wstring fileName = filePath.filename();
    std::wcout << L"Copying start..." << std::endl;

   
            // Copy the entire file

    std::wstring dest = destinationFolder + L"\\" + fileName;
            std::wcout << dest << std::endl;
            fs::copy_file(sourceFolder, dest, fs::copy_options::overwrite_existing);
            std::cout << "Done";
    

    std::wcout << L"Copying completed." << std::endl;
}

//int main() {
//    std::wstring source = L"D:\\Backup\\Softs1.PNG";
//    std::wstring destination = L"D:\\Backup\\AHmad\\Softs1.PNG";
//    copyFiles(source, destination);
//
//}
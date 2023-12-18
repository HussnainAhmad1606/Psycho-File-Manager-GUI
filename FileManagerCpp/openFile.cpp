#include <windows.h>
#include <iostream>
#include <string>
#include "openFile.h"

void openFile(std::string fileName, std::string currentDirectory) {
    std::cout << "Current Directory Coming: " << currentDirectory << std::endl;
    std::cout << "Current File Name Coming: " << fileName << std::endl;

    // Find the position of the last dot in the file name
    size_t dotPosition = fileName.find_last_of('.');

    if (dotPosition != std::string::npos) {
        // Extract the file extension
        std::string fileExtension = fileName.substr(dotPosition + 1);

        // Print the file extension
        std::cout << "File Extension: " << fileExtension << std::endl;

        if (fileExtension == "mp4" || fileExtension == "MP4") {
            std::string command;
            if (currentDirectory.length() == 1) {
           command = "\"C:\\Program Files\\VideoLAN\\VLC\\vlc.exe\" \"" + currentDirectory  + ":\\" + fileName + "\"";
            }
            else {
           command = "\"C:\\Program Files\\VideoLAN\\VLC\\vlc.exe\" \"" + currentDirectory  + "\\" + fileName + "\"";

            }
           std:: cout << command;
            LPWSTR commandCharArray = new wchar_t[command.size() + 1];
            std::copy(command.begin(), command.end(), commandCharArray);
            commandCharArray[command.size()] = L'\0';
            STARTUPINFO si;
            PROCESS_INFORMATION pi;

            // Set up the startup info structure
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));

            // Create the process
            if (CreateProcess(NULL, commandCharArray, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                // Wait for the process to finish (optional)
                WaitForSingleObject(pi.hProcess, INFINITE);

                // Close process and thread handles
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);

                std::cout << "VLC command executed successfully." << std::endl;
               
            }
            else {
                std::cerr << "VLC command failed to execute. Error code: " << GetLastError() << std::endl;
                
            }
           
        }
    }
    else {
        std::cout << "File has no extension." << std::endl;
    }

}

//int main() {
    // Specify the drive letter (e.g., C:)
//    std::wstring driveLetter = L"D:\\Entertainment\\Forza Horizon\\Forza Horizon 4 [FitGirl Repack]";

//    listFolders(driveLetter);
//
//   return 0;
//}

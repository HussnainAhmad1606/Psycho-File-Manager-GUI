#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include "propertiesAlert.h"

std::wstring SystemTimeToString(const SYSTEMTIME& systemTime) {
    // Format the date and time into a string
    wchar_t buffer[256];
    _snwprintf_s(buffer, _countof(buffer), L"%04d-%02d-%02d %02d:%02d:%02d",
        systemTime.wYear, systemTime.wMonth, systemTime.wDay,
        systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

    return buffer;
}

std::wstring GetFileTimeAsString(const FILETIME& fileTime) {
    SYSTEMTIME systemTime;
    FileTimeToSystemTime(&fileTime, &systemTime);

    return SystemTimeToString(systemTime);
}

void showProperties(std::wstring currentSelection, std::wstring currentDirectory) {
    WIN32_FIND_DATA findFileData;
        std::wstring filePath;
    if (currentDirectory.length() == 1) {
        filePath = currentDirectory + L":\\" + currentSelection;
    }
    else {

        filePath = currentDirectory + L"\\" + currentSelection;
    }
    LPCWSTR filePathPointer = filePath.c_str();
    HANDLE hFind = FindFirstFile(filePathPointer, &findFileData);
    std::wcout << filePathPointer;
    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Unable to find file" << std::endl;
        return;
    }
    std::wcout << "File Properties for: " << filePath << std::endl;
    std::cout << "File Size: " << findFileData.nFileSizeLow << " bytes" << std::endl;
    std::cout << "Creation Time: " << findFileData.ftCreationTime.dwLowDateTime << std::endl;
    std::cout << "Last Access Time: " << findFileData.ftLastAccessTime.dwLowDateTime << std::endl;
    std::cout << "Last Write Time: " << findFileData.ftLastWriteTime.dwLowDateTime << std::endl;

    FindClose(hFind);

    sf::RenderWindow window(sf::VideoMode(400, 250), "Properties");

    sf::Font font;
    font.loadFromFile("C:/Users/Psycho/Downloads/Compressed/roboto/Roboto-Bold.ttf");


    sf::Text alertText(currentSelection, font, 20);
    alertText.setFillColor(sf::Color::Black);
    alertText.setPosition(30, 30);

    std::wstring sizeOfFile = L"File Size: " + std::to_wstring(findFileData.nFileSizeLow) + L" Bytes";
    sf::Text name(sizeOfFile, font, 15);

    name.setFillColor(sf::Color::Black);
    name.setPosition(30, 70);

    std::wstring createdAt = L"Created Time: " + GetFileTimeAsString(findFileData.ftCreationTime);
    sf::Text version(createdAt, font, 15);
    version.setFillColor(sf::Color::Black);
    version.setPosition(30, 110);
    
    std::wstring lastAccessed = L"Last Accessed: " + GetFileTimeAsString(findFileData.ftLastAccessTime);
    sf::Text lastAccessedText(lastAccessed, font, 15);
    lastAccessedText.setFillColor(sf::Color::Black);
    lastAccessedText.setPosition(30, 150);
    
    std::wstring lastModified = L"Last Modified: " + GetFileTimeAsString(findFileData.ftLastWriteTime);
    sf::Text lastModifiedText(lastModified, font, 15);
    lastModifiedText.setFillColor(sf::Color::Black);
    lastModifiedText.setPosition(30, 190);

    bool showAlert = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::White);


        window.draw(alertText);
        window.draw(name);
        window.draw(version);
        window.draw(lastAccessedText);
        window.draw(lastModifiedText);
        window.display();


    }

}

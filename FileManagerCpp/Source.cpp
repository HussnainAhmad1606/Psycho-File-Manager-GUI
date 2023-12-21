#include <SFML/Graphics.hpp>
#include <iostream>

// Header Files
#include "getLogicalDrives.h"
#include "getFoldersFromPath.h"
#include "openFile.h"
#include "aboutAlert.h"
#include "newFolderInput.h"
#include "propertiesAlert.h"
#include "copyFiles.h"


#include <locale>
#include <codecvt>

#include <queue>   
#include <stack>
std::stack<std::wstring> directories;

std::wstring currentDirectory = L"";
std::wstring currentSelection = L"";

std::queue<std::wstring> copiedFilesData;
class ClickableElement {

public:
    ClickableElement(const sf::Texture& texture, const sf::Font& font, const DriveInfo& driveInfo, float position, float positionY = 300)
        : sprite(texture), driveInfo(driveInfo), isClicked(false) {
        // Set up sprite and text
        sprite.setPosition(sf::Vector2f(position, positionY));
        sprite.setScale(sf::Vector2f(1.5, 1.5));
        // Set up bounding box for click detection
        boundingBox = sprite.getGlobalBounds();

        // Set up text
        text.setFont(font);
        text.setCharacterSize(12); // Adjust the size as needed

        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::string str = converter.to_bytes(driveInfo.driveLetter);
        if (currentDirectory.length() != 0) {
            std::string stri = str + driveInfo.driveType;
            text.setString(stri.substr(0, 15) + "...");
        }
        else {

            text.setString(str + " :/ " + driveInfo.driveType);
        }

        text.setPosition(sf::Vector2f(position, positionY + 40)); // Adjust the position as needed
        text.setFillColor(sf::Color::Black);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
        window.draw(text);
    }

    bool isMouseOver(const sf::RenderWindow& window) const {
        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        return boundingBox.contains(mousePosition);
    }
    void singleClick() {
        currentSelection = driveInfo.driveLetter;
        std::wcout << driveInfo.driveLetter << std::endl;
        std::wcout << currentDirectory;

    }
    void handleMouseClick(sf::Texture& file, sf::Texture& normalDrive, sf::Texture& folder, sf::Font& font, std::vector<ClickableElement>& clickableElements, int maxFoldersToShow = 6) {
        std::cout << "Changed!";
        if (currentDirectory.length() == 0) {
            std::cout << "Zero\n";


        }
        else {
            std::cout << "Not Zero\n";

            std::cout << driveInfo.driveType;
        }

        isClicked = true;
        std::vector<DriveInfo> folders;
        if (currentDirectory.length() == 0) {
            folders = listFolders(driveInfo.driveLetter + L":");
            currentDirectory = driveInfo.driveLetter;

            // Pushing in stack
            directories.push(driveInfo.driveLetter);


        }
        else {

            size_t position = currentDirectory.find(L':');
            if (driveInfo.isDirectory) {
                if (position != std::wstring::npos) {

                    currentDirectory = currentDirectory + L"\\" + driveInfo.driveLetter;
                    std::wcout << "'W' found at position: " << position << std::endl;
                    directories.push(currentDirectory);
                }
                else {

                    currentDirectory = currentDirectory + L":\\" + driveInfo.driveLetter;

                    std::wcout << "'W' not found in the wstring." << std::endl;
                    directories.push(currentDirectory);


                }
                std::wcout << currentDirectory;
                folders = listFolders(currentDirectory);
            }









        }
        if (driveInfo.isDirectory != true) {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            std::string fileName = converter.to_bytes(driveInfo.driveLetter);
            std::string convertedDirectory = converter.to_bytes(currentDirectory);

            openFile(fileName, convertedDirectory);

        }
        std::cout << "Clicked Drive! -> " << driveInfo.driveType << std::endl;
        std::wcout << "Clicked! -> " << driveInfo.driveLetter << std::endl;

        if (driveInfo.isDirectory) {
            std::cout << "Updating contents";
            clickableElements.clear();
            // Create clickable elements for folders

            float folderPos = 10;
            float positionY = 200;
            for (int i = 0; i < static_cast<int>(folders.size()); ++i) {
                if (i > 0 && i % maxFoldersToShow == 0) {
                    // Start a new row
                    positionY += 100; // Adjust based on your desired spacing between rows
                    folderPos = 10;
                }

                if (currentDirectory.length() != 0) {
                    if (folders[i].isDirectory) {
                        ClickableElement folderElement(folder, font, folders[i], folderPos, positionY);
                        clickableElements.emplace_back(folderElement);
                    }
                    else {
                        ClickableElement folderElement(file, font, folders[i], folderPos, positionY);
                        clickableElements.emplace_back(folderElement);
                        
                    }

                }
                else {
                    ClickableElement folderElement(normalDrive, font, folders[i], folderPos, positionY);
                    clickableElements.emplace_back(folderElement);

                }
                folderPos += 130; // Adjust based on your window size and spacing
            }



        }



    }

    void handleMouseRelease() {
        isClicked = false;
    }

    bool wasClicked() const {
        return isClicked;
    }

private:
    sf::Sprite sprite;
    sf::Text text; // Make sure you have included #include <SFML/Graphics/Text.hpp>
    sf::FloatRect boundingBox;
    bool isClicked;
    DriveInfo driveInfo;    
    std::vector<ClickableElement> clickableElements; // Member vector to store elements

};


// If user back to home directory, draws all the drives
void loadHomeDirectory(sf::Texture& normalDrive, sf::Texture& windowsDirve, sf::Font& font, std::vector<ClickableElement>& clickableElements) {
    std::vector<DriveInfo> drives = getAllDrives();


    float pos = 10;

    clickableElements.clear();

    for (const auto& drive : drives) {
        // Create a ClickableElement using the correct constructor parameters

        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::string str = converter.to_bytes(drive.driveLetter);
        if (str == "C") {
            {
                ClickableElement element(windowsDirve, font, drive, pos);
                clickableElements.emplace_back(element);
            }
        }
        else {
            {
                ClickableElement element(normalDrive, font, drive, pos);
                clickableElements.emplace_back(element);
            }
        }
        pos += 100; // Adjust this increment based on your window size and desired spacing
    }
}


// Function to load all of the files again
void refreshDirectory(std::wstring dir, sf::Texture& file, sf::Texture& normalDrive, sf::Texture& folder, sf::Font& font, std::vector<ClickableElement>& clickableElements, int maxFoldersToShow = 6) {
    // Folder is created
    std::vector<DriveInfo> updatedFolders = listFolders(dir);
    clickableElements.clear();

    float folderPos = 10;
    float positionY = 150;
    for (int i = 0; i < static_cast<int>(updatedFolders.size()); ++i) {
        if (i > 0 && i % maxFoldersToShow == 0) {
            // Start a new row
            positionY += 100; // Adjust based on your desired spacing between rows
            folderPos = 10;
        }

        if (currentDirectory.length() != 0) {
            if (updatedFolders[i].isDirectory) {
                ClickableElement folderElement(folder, font, updatedFolders[i], folderPos, positionY);
                clickableElements.emplace_back(folderElement);
            }
            else {
                ClickableElement folderElement(file, font, updatedFolders[i], folderPos, positionY);
                clickableElements.emplace_back(folderElement);

            }

        }
        else {
            ClickableElement folderElement(normalDrive, font, updatedFolders[i], folderPos, positionY);
            clickableElements.emplace_back(folderElement);

        }
        folderPos += 130; // Adjust based on your window size and spacing
    }

}
int main() {
    // Looping and get all drives data

    // Loop through the data
    std::vector<DriveInfo> drives = getAllDrives();

    




    // Load texture and font
    sf::Texture texture;
    sf::Texture normalDrive;
    sf::Texture windowsDrive;
    sf::Texture folder;
    sf::Texture file;
    sf::Texture backButton;

    texture.setSmooth(false);
    normalDrive.setSmooth(false);
    windowsDrive.setSmooth(false);
    folder.setSmooth(true);
    file.setSmooth(true);

    if (!file.loadFromFile("C:/Users/Psycho/Downloads/BizzyKart/file.png")) {
        // Handle error
        return EXIT_FAILURE;
    }

    if (!backButton.loadFromFile("C:/Users/Psycho/Downloads/BizzyKart/backButton.png")) {
        // Handle error
        return EXIT_FAILURE;
    }
    
    if (!texture.loadFromFile("C:/Users/Psycho/Downloads/BizzyKart/thisPc.png")) {
        // Handle error
        return EXIT_FAILURE;
    }
    if (!folder.loadFromFile("C:/Users/Psycho/Downloads/BizzyKart/folder.png")) {
        // Handle error
        return EXIT_FAILURE;
    }
    if (!normalDrive.loadFromFile("C:/Users/Psycho/Downloads/BizzyKart/normalDrive.png")) {
        // Handle error
        return EXIT_FAILURE;
    }
    if (!windowsDrive.loadFromFile("C:/Users/Psycho/Downloads/BizzyKart/windowsDrive.png")) {
        // Handle error
        return EXIT_FAILURE;
    }

    sf::Font font;
    if (!font.loadFromFile("C:/Users/Psycho/Downloads/Compressed/roboto/Roboto-Bold.ttf")) {
        // Handle error
        return EXIT_FAILURE;
    }
    std::vector<ClickableElement> clickableElements;

    float pos = 10;
  

    for (const auto& drive : drives) {
        // Create a ClickableElement using the correct constructor parameters

        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::string str = converter.to_bytes(drive.driveLetter);
        if (str == "C") {
            {
                ClickableElement element(windowsDrive, font, drive, pos);
                clickableElements.emplace_back(element);
            }
        }
        else {
            {
                ClickableElement element(normalDrive, font, drive, pos);
                clickableElements.emplace_back(element);
            }
        }
        pos += 100; // Adjust this increment based on your window size and desired spacing
    }
    sf::RenderWindow window(sf::VideoMode(800, 600), "Psycho File Manager");

    // Set up a view for scrolling
    sf::View view(sf::FloatRect(0, 0, 800, 600));
    window.setView(view);

    // Create menu bar background
    sf::RectangleShape menuBar(sf::Vector2f(window.getSize().x, 40));
    menuBar.setFillColor(sf::Color(200, 200, 200)); // Light gray color
    menuBar.setPosition(0, 0);

    // Create menu options
   /* sf::Text fileOption("File", font, 15);
    fileOption.setPosition(10, 10);*/

    sf::Text aboutOption("About", font, 15);
    aboutOption.setPosition(10, 10);

    // Creating toolbar
    sf::RectangleShape toolBar(sf::Vector2f(window.getSize().x, 60));
    toolBar.setFillColor(sf::Color(255, 0, 0)); // Light gray color
    toolBar.setPosition(0, 40);

    
    
    sf::Text copyText("Copy", font, 15);
    copyText.setPosition(250, 60);
    
    sf::Text pasteText("Paste", font, 15);
    pasteText.setPosition(300, 60); 


    sf::Text propertiesText("Properties", font, 15);
    propertiesText.setPosition(70, 60);

    sf::Text newFolder("New Folder", font, 15);
    newFolder.setPosition(160, 60);
    

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string currentDire = converter.to_bytes(currentDirectory);
    sf::Text currentDirectoryText(currentDire, font, 15);
    currentDirectoryText.setFillColor(sf::Color::Black);
    currentDirectoryText.setPosition(280, 150);

    sf::RectangleShape backButtonText(sf::Vector2f(20, 20));
    backButtonText.setTexture(&backButton); // Set the texture
    backButtonText.setPosition(10, 60);

    // For Handling double click
    sf::Clock clickClock;
    sf::Time clickCooldown = sf::milliseconds(900); // Adjust the time threshold for a double click
    sf::Vector2i clickPosition;

    sf::Clock clock;

    bool mouseClicked = false;


    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        float scrollSpeed = 3000.0f;
        float scrollDistance = scrollSpeed * elapsed.asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (clickClock.getElapsedTime() < clickCooldown && clickPosition == sf::Mouse::getPosition(window)) {
                    // Double-click detected
                    std::cout << "Double Click!" << std::endl;
                    // Add your double-click handling code here

                    // Reset the clock and clear the position to prevent detecting more than one double-click
                    clickClock.restart();
                    clickPosition = sf::Vector2i(-1, -1);
                    for (auto& clickableElement : clickableElements) {

                        if (clickableElement.isMouseOver(window)) {

                            clickableElement.handleMouseClick(file, windowsDrive, folder, font, clickableElements);

                        }
                    }
                }
                else {
                    sf::Vector2f mousePosition(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                    if (copyText.getGlobalBounds().contains(mousePosition)) {
                        if (!mouseClicked) {
                            std::cout << "Copy clicked!" << std::endl;
                            std::wstring fileToCopy = currentDirectory + L"\\" + currentSelection;
                            copiedFilesData.push(fileToCopy);
                            std::cout << "File Data Copied";
                            // Add your cut handling code here
                            mouseClicked = true;
                            std::wstring frontElement = copiedFilesData.front();
                            std::wcout << "Queue Front: " << frontElement << std::endl;
                        }
                    }
                    else if (backButtonText.getGlobalBounds().contains(mousePosition)) {
                        if (!mouseClicked) {
                            std::cout << "Back Button clicked!" << std::endl;
                            // Add your cut handling code here
                            std::cout << "Stack: " << std::endl;
                           
                            mouseClicked = true;

                            if (directories.empty()) {
                                std::cout << "HOME";

                                loadHomeDirectory(normalDrive, windowsDrive,font, clickableElements);
                                currentDirectory = L"";
                               }
                            else {
                                    directories.pop();
                                if (!directories.empty()) {
                                    std::wstring dir = directories.top();
                                    std::wcout << std::endl << dir;
                                    if (dir.length() == 1) {
                                        std::wstring temp = dir + L":";
                                        currentDirectory = temp;
                                        refreshDirectory(temp, file, windowsDrive, folder, font, clickableElements);
                                    }
                                    else {
                                        currentDirectory = dir;
                                        refreshDirectory(dir, file, windowsDrive, folder, font, clickableElements);
                                    }
                                }
                            }

                        }
                    }
                    else if (pasteText.getGlobalBounds().contains(mousePosition)) {
                        if (!mouseClicked) {
                            std::cout << "Paste clicked!" << std::endl;
                            // Add your paste handling code here
                            mouseClicked = true;
                            std::wstring fileToCopy = copiedFilesData.front();
                            copiedFilesData.pop();
                            std::wcout << "File to copy: " << fileToCopy << std::endl;
                            std::wcout << "Current Directory: " << currentDirectory << std::endl;
                            
                            copyFiles(fileToCopy, currentDirectory);
                        }
                    }
                    else if (propertiesText.getGlobalBounds().contains(mousePosition)) {

                        // Showing Properties alert
                        showProperties(currentSelection, currentDirectory);

                    }
                    else if (aboutOption.getGlobalBounds().contains(mousePosition)) {
                        if (!mouseClicked) {
                            std::cout << "About Clicked!" << std::endl;
                            showAlert();
                            // Add your paste handling code here
                            mouseClicked = true;
                        }
                    }
                    else if (newFolder.getGlobalBounds().contains(mousePosition)) {
                        if (!mouseClicked) {
                            std::cout << "New Folder clicked!" << std::endl;
                            // Add your new folder handling code here
                            int maxFoldersToShow = 6;
                            bool isCreate = inputFolderName(currentDirectory);
                            if (isCreate) {
                                // Folder is created
                                std::vector<DriveInfo> updatedFolders = listFolders(currentDirectory);
                                clickableElements.clear();
                                // Create clickable elements for folders

                                float folderPos = 10;
                                float positionY = 150;
                                for (int i = 0; i < static_cast<int>(updatedFolders.size()); ++i) {
                                    if (i > 0 && i % maxFoldersToShow == 0) {
                                        // Start a new row
                                        positionY += 100; // Adjust based on your desired spacing between rows
                                        folderPos = 10;
                                    }

                                    if (currentDirectory.length() != 0) {
                                        if (updatedFolders[i].isDirectory) {
                                            ClickableElement folderElement(folder, font, updatedFolders[i], folderPos, positionY);
                                            clickableElements.emplace_back(folderElement);
                                        }
                                        else {
                                            ClickableElement folderElement(file, font, updatedFolders[i], folderPos, positionY);
                                            clickableElements.emplace_back(folderElement);

                                        }

                                    }
                                    else {
                                        ClickableElement folderElement(normalDrive, font, updatedFolders[i], folderPos, positionY);
                                        clickableElements.emplace_back(folderElement);

                                    }
                                    folderPos += 130; // Adjust based on your window size and spacing
                                }

                            }
                            mouseClicked = true;
                        }
                    }
                    for (auto& clickableElement : clickableElements) {
                      
                       

                        if (clickableElement.isMouseOver(window)) {

                            clickableElement.singleClick();

                        }
                    }
                    // Single click detected
                    std::cout << "Single Click!" << std::endl;
                    // Add your single-click handling code here

                    // Record the click position
                    clickPosition = sf::Mouse::getPosition(window);

                    // Reset the clock for the next click
                    clickClock.restart();


                }
                
               
                
                break;
            case sf::Event::MouseButtonReleased:

                for (auto& clickableElement : clickableElements) {
                    mouseClicked = false;
                    clickableElement.handleMouseRelease();
                }
                
                break;
                case sf::Event::MouseWheelScrolled:
                    //std::cout << "wheel movement: " << event.mouseWheel.delta << std::endl;
                    if (event.mouseWheelScroll.delta > 0) {
                        view.move(0, -scrollDistance);

                        // Scrolled up
                        // Add your code here
                    }
                    // Check if the scroll wheel is scrolled down
                    else if (event.mouseWheelScroll.delta < 0) {
                        view.move(0, scrollDistance);


                        // Scrolled down
                        // Add your code here
                    }

            }
        }


       

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            std::cout << "Scrolling up";
            view.move(0, -scrollDistance);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            std::cout << "Scrolling down";
            view.move(0, scrollDistance);
        }

        

        window.clear(sf::Color::White);
        window.draw(menuBar);
        //window.draw(fileOption);
        window.draw(aboutOption);
        window.draw(toolBar);
        window.draw(newFolder);
        window.draw(copyText);
        window.draw(pasteText);
        window.draw(propertiesText);
        window.draw(backButtonText);
        window.draw(currentDirectoryText);
       
        for (auto& clickableElement : clickableElements) {
        
            clickableElement.draw(window);
        }
      
        
        window.setView(view);
        window.display();
    }

    return EXIT_SUCCESS;
}
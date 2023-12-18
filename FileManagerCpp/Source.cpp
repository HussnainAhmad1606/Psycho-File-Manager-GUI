#include <SFML/Graphics.hpp>
#include <iostream>
#include "getLogicalDrives.h"
#include "getFoldersFromPath.h"
#include "openFile.h"

#include <locale>
#include <codecvt>
std::wstring currentDirectory = L"";
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
        text.setString(stri.substr(0, 15)+"...");
        }
        else {

        text.setString(str+":/ "+driveInfo.driveType);
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

    void handleMouseClick(sf::Texture &file, sf::Texture &normalDrive, sf::Texture& folder, sf::Font &font, std::vector<ClickableElement>& clickableElements, int maxFoldersToShow = 6) {
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


        }
        else {

            size_t position = currentDirectory.find(L':');
            if (driveInfo.isDirectory) {
                if (position != std::wstring::npos) {

                    currentDirectory = currentDirectory + L"\\" + driveInfo.driveLetter;
                    std::wcout << "'W' found at position: " << position << std::endl;

                }
                else {

                    currentDirectory = currentDirectory + L":\\" + driveInfo.driveLetter;

                    std::wcout << "'W' not found in the wstring." << std::endl;


                }
                std::wcout << currentDirectory;
                folders = listFolders(currentDirectory);
            }

          
            
        
           
              
      


        }
        if (driveInfo.isDirectory!=true) {
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
            float positionY = 50;
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

    texture.setSmooth(false);
    normalDrive.setSmooth(false);
    windowsDrive.setSmooth(false);
    folder.setSmooth(true);
    file.setSmooth(true);

    if (!file.loadFromFile("C:/Users/Psycho/Downloads/BizzyKart/file.png")) {
        // Handle error
        return EXIT_FAILURE;
    }if (!texture.loadFromFile("C:/Users/Psycho/Downloads/BizzyKart/thisPc.png")) {
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
    sf::Text fileOption("File", font, 15);
    fileOption.setPosition(10, 10);

    sf::Text aboutOption("About", font, 15);
    aboutOption.setPosition(70, 10);

    sf::Clock clock;
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
                for (auto& clickableElement : clickableElements) {
                    if (clickableElement.isMouseOver(window)) {
                        
                        clickableElement.handleMouseClick(file, windowsDrive, folder, font,  clickableElements);
                        
                    }
                }
                
                break;
            case sf::Event::MouseButtonReleased:
                for (auto& clickableElement : clickableElements) {
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
        window.draw(fileOption);
        window.draw(aboutOption);
       
        for (auto& clickableElement : clickableElements) {
        
            clickableElement.draw(window);
        }
      
        
        window.setView(view);
        window.display();
    }

    return EXIT_SUCCESS;
}
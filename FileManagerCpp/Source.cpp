#include <SFML/Graphics.hpp>
#include <iostream>
#include "getLogicalDrives.h"


#include <locale>
#include <codecvt>
class ClickableElement {
    
public:
    ClickableElement(const sf::Texture& texture, const sf::Font& font, const DriveInfo& driveInfo, float position)
        : sprite(texture), driveInfo(driveInfo), isClicked(false) {
        // Set up sprite and text
        sprite.setPosition(sf::Vector2f(position, 300));
        sprite.setScale(sf::Vector2f(1.5, 1.5));
        // Set up bounding box for click detection
        boundingBox = sprite.getGlobalBounds();

        // Set up text
        text.setFont(font);
        text.setCharacterSize(12); // Adjust the size as needed

        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::string str = converter.to_bytes(driveInfo.driveLetter);
  
        text.setString(str+":/ "+driveInfo.driveType);
        text.setPosition(sf::Vector2f(position, 300 + 30)); // Adjust the position as needed
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

    void handleMouseClick() {
        isClicked = true;

        std::wcout << "Clicked! -> " << driveInfo.driveLetter << std::endl;
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
};

int main() {
    // Looping and get all drives data

    // Loop through the data
    std::vector<DriveInfo> drives = getAllDrives();

    




    // Load texture and font
    sf::Texture texture;
    sf::Texture normalDrive;
    sf::Texture windowsDrive;
    texture.setSmooth(false);
    normalDrive.setSmooth(false);
    windowsDrive.setSmooth(false);
    if (!texture.loadFromFile("C:/Users/Psycho/Downloads/BizzyKart/thisPc.png")) {
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
            ClickableElement element(windowsDrive, font, drive, pos);
        clickableElements.emplace_back(element);
        }
        else {
            ClickableElement element(normalDrive, font, drive, pos);
        clickableElements.emplace_back(element);
        }
        pos += 100; // Adjust this increment based on your window size and desired spacing
    }
    sf::RenderWindow window(sf::VideoMode(800, 600), "Psycho File Manager");



    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                for (auto& clickableElement : clickableElements) {
                    if (clickableElement.isMouseOver(window)) {
                        clickableElement.handleMouseClick();
                    }
                }
                
                break;
            case sf::Event::MouseButtonReleased:
                for (auto& clickableElement : clickableElements) {
                    clickableElement.handleMouseRelease();
                }
                
                break;
            }
        }

        window.clear(sf::Color::White);
       
        for (auto& clickableElement : clickableElements) {
        
            clickableElement.draw(window);
        }
      
        

        window.display();
    }

    return EXIT_SUCCESS;
}
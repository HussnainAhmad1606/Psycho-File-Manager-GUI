#include <SFML/Graphics.hpp>
#include <iostream>
#include "getLogicalDrives.h"


#include <locale>
#include <codecvt>
class ClickableElement {
    
public:
    ClickableElement(const sf::Texture& texture, const sf::Font& font, const std::string& text, float position)
        : sprite(texture), text(text, font), isClicked(false)  {
        // Set up sprite and text
        sprite.setPosition(sf::Vector2f(position, 300));
        


        // Set up bounding box for click detection
        boundingBox = sprite.getGlobalBounds();
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
        std::cout << "Element clicked!" << std::endl;
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
};

int main() {
    // Looping and get all drives data

    // Loop through the data
    std::vector<DriveInfo> drives = getAllDrives();
    std::cout << "Test";
    




    // Load texture and font
    sf::Texture texture;
    if (!texture.loadFromFile("C:/Users/Psycho/Downloads/BizzyKart/thisPc.png")) {
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
        // Convert drive letter (wchar_t) to a wstring
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        std::string str = converter.to_bytes(drive.driveLetter);
        // Create a ClickableElement using the correct constructor parameters
        ClickableElement element(texture, font, str, pos);
        clickableElements.emplace_back(element);
        pos += 100;
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
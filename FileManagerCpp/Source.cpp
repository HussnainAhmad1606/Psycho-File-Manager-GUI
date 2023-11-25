#include <SFML/Graphics.hpp>
#include <iostream>
#include "getLogicalDrives.h"
class ClickableElement {
public:
    ClickableElement(const sf::Texture& texture, const sf::Font& font, const std::string& text)
        : sprite(texture), text(text, font), isClicked(false) {
        // Set up sprite and text
        sprite.setPosition(sf::Vector2f(100, 100)); // Adjust the position as needed
      

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
    for (const auto& drive : drives) {
        std::wcout << drive.driveLetter << L":\\ ";
        std::cout << drive.driveType << std::endl;
    }

 

    // Get desktop resolution
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    sf::RenderWindow window(desktopMode, "Psycho File Manager", sf::Style::Fullscreen);

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

    
    // Create a clickable element
    ClickableElement clickableElement(texture, font, "Click me!");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (clickableElement.isMouseOver(window)) {
                    clickableElement.handleMouseClick();
                }
                break;
            case sf::Event::MouseButtonReleased:
                clickableElement.handleMouseRelease();
                break;
            }
        }

        window.clear(sf::Color::White);

        // Draw the clickable element
        clickableElement.draw(window);

        window.display();
    }

    return EXIT_SUCCESS;
}

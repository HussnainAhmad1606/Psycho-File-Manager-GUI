#include <SFML/Graphics.hpp>
#include <iostream>
#include "aboutAlert.h"

void showAlert() {
    sf::RenderWindow window(sf::VideoMode(400, 200), "About Psycho File Manager");

    sf::Font font;
    font.loadFromFile("C:/Users/Psycho/Downloads/Compressed/roboto/Roboto-Bold.ttf");


    sf::Text alertText("Software: Psycho File Manager", font, 20);
    alertText.setFillColor(sf::Color::Black);
    alertText.setPosition(30, 30);
   
    sf::Text name("Made By: Psycho Coder", font, 20);
    name.setFillColor(sf::Color::Black);
    name.setPosition(30,70);
    sf::Text version("Version: 1.0", font, 20);
    version.setFillColor(sf::Color::Black);
    version.setPosition(30, 110);

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
        window.display();


    }
  
}

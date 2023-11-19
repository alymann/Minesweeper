#ifndef WELCOME_H
#define WELCOME_H

#include<iostream>
#include<SFML/Graphics.hpp>
using namespace std;

string welcomeScreen(int width, int height) {
    string userName;

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        cout << "Could not load font." << endl;
        return "Invalid Username!";
    }

    sf::Text welcomeText;
    welcomeText.setFont(font);
    welcomeText.setCharacterSize(24);
    welcomeText.setColor(sf::Color::White);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    welcomeText.setString("WELCOME TO MINESWEEPER!");

    sf::Text inputPrompt;
    inputPrompt.setFont(font);
    inputPrompt.setCharacterSize(20);
    inputPrompt.setColor(sf::Color::White);
    inputPrompt.setStyle(sf::Text::Bold);
    inputPrompt.setString("Enter your name:");

    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(18);
    inputText.setColor(sf::Color::Yellow);
    inputText.setStyle(sf::Text::Bold);

    sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return "Invalid Username!";
            }
            if (event.type == sf::Event::TextEntered) {
                if (isalpha(event.text.unicode)) {
                    if (userName.length() == 0) {
                        userName += toupper(event.text.unicode);
                    } else if (userName.length() < 10) {
                        userName += tolower(event.text.unicode);
                    }
                } else if (event.text.unicode == 8) {
                    if (userName.length() != 0) {
                        userName.pop_back();
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (userName.length() != 0) {
                    if (event.key.code == sf::Keyboard::Return) {
                        window.close();
                    }
                }
            }
        }
        window.clear(sf::Color::Blue);

        sf::FloatRect titleRect = welcomeText.getLocalBounds();
        welcomeText.setOrigin(titleRect.width / 2, titleRect.height / 2);
        welcomeText.setPosition(width / 2.0f, height / 2.0f - 150);
        window.draw(welcomeText);

        sf::FloatRect inputPromptRect = inputPrompt.getLocalBounds();
        inputPrompt.setOrigin(inputPromptRect.width / 2, inputPromptRect.height / 2);
        inputPrompt.setPosition(width / 2.0f, height / 2.0f - 75);
        window.draw(inputPrompt);

        sf::FloatRect inputTextRect = inputText.getLocalBounds();
        inputText.setOrigin(inputTextRect.width / 2, inputTextRect.height / 2);
        inputText.setPosition(width / 2.0f, height / 2.0f - 45);
        inputText.setString(userName + "|");
        window.draw(inputText);

        window.display();
    }
    return userName;
}

#endif

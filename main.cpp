#include<iostream>
#include<SFML/Graphics.hpp>
#include"welcome.h"
#include"tile.h"
#include"board.h"
#include<ctime>
#include<fstream>
using namespace std;

void drawNum(int num, int x, int y, sf::RenderWindow& window, int numDigits) {
    sf::Texture digits;
    if (!digits.loadFromFile("files/images/digits.png")) {
        cout << "FILE NOT FOUND!!" << endl;
    }
    if (num < 0) {
        num *= -1;
        sf::Sprite negative;
        negative.setTexture(digits);
        negative.setTextureRect(sf::IntRect(210, 0, 21, 32));
        negative.setPosition(x-21, y);
        window.draw(negative);
    }
    for (int i = numDigits-1; i >= 0; i--) {
        int currDigit = num % 10;
        num /= 10;
        sf::Sprite numSprite;
        numSprite.setTexture(digits);
        numSprite.setTextureRect(sf::IntRect(currDigit*21, 0, 21, 32));
        numSprite.setPosition(x+(21*i), y);
        window.draw(numSprite);
    }
}

vector<string> getLeaderBoard() {
    ifstream inFile("files/leaderboard.txt");
    vector<string> fileText;
    while (!inFile.eof()) {
        string curr;
        inFile >> curr;
        fileText.push_back(curr);
    }
    inFile.close();
    return fileText;
}

void updateLeaderBoard(int time, string username) {
    vector<string> leaderboard = getLeaderBoard();
    for (int i = 0; i < leaderboard.size(); i++) {
        string minuteS = leaderboard[i].substr(0,2);
        string secondS = leaderboard[i].substr(3,5);
        int currTime = stoi(minuteS)*60 + stoi(secondS);
        if (currTime > time) {
            string minutes = to_string(time/60);
            if (minutes.length() == 1) {
                minutes = "0"+minutes;
            }
            string seconds = to_string(time%60);
            if (seconds.length() == 1) {
                seconds = "0"+seconds;
            }
            string toBoard = minutes+":"+ seconds+","+username+"*";
            leaderboard.insert(leaderboard.begin()+i,toBoard);
            leaderboard.pop_back();
            break;
        }
    }
    ofstream outFile("files/leaderboard.txt");
    for (int i = 0; i < 5; i++) {
        outFile.write(leaderboard[i].c_str(),leaderboard[i].length());
        if (i < 4) {
            outFile.write("\n", 1);
        }
    }
    outFile.close();
}

void clearAsterisk() {
    vector<string> leaderboard = getLeaderBoard();
    for (int i = 0; i < leaderboard.size(); i++) {
        if (leaderboard[i][leaderboard[i].length()-1] == '*') {
            leaderboard[i].pop_back();
        }
    }
    ofstream outFile("files/leaderboard.txt");
    for (int i = 0; i < 5; i++) {
        outFile.write(leaderboard[i].c_str(),leaderboard[i].length());
        if (i < 4) {
            outFile.write("\n", 1);
        }
    }
    outFile.close();
}

int main() {
    int width = 800;
    int height = 600;

    string validUsername = welcomeScreen(width, height);
    if (validUsername == "Invalid Username!") {
        return 0;
    }

    ifstream boardConfig("files/board_config.cfg");
    int cols;
    boardConfig >> cols;
    int rows;
    boardConfig >> rows;
    int mines;
    boardConfig >> mines;
    mines = 4;

    width = cols * 32;
    height = (rows * 32) + 100;

    sf::Texture pauseText;
    if (!pauseText.loadFromFile("files/images/pause.png")) {
        cout << "FILE NOT FOUND!" << endl;
    }
    sf::Sprite pause;
    pause.setTexture(pauseText);
    pause.setPosition(cols*32-240, (rows+0.5f)*32);

    sf::Texture playText;
    if (!playText.loadFromFile("files/images/play.png")) {
        cout << "FILE NOT FOUND!" << endl;
    }
    sf::Sprite play;
    play.setTexture(playText);
    play.setPosition(cols*32-240, (rows+0.5f)*32);

    sf::Texture debugText;
    if (!debugText.loadFromFile("files/images/debug.png")) {
        cout << "FILE NOT FOUND!" << endl;
    }
    sf::Sprite debug;
    debug.setTexture(debugText);
    debug.setPosition(cols*32-304, (rows+0.5f)*32);

    sf::Texture happyFaceText;
    if (!happyFaceText.loadFromFile("files/images/face_happy.png")) {
        cout << "FILE NOT FOUND!" << endl;
    }
    sf::Sprite happyFace;
    happyFace.setTexture(happyFaceText);
    happyFace.setPosition((cols/2.0f)*32 - 32, (rows+0.5f)*32);

    sf::Texture winFaceText;
    if (!winFaceText.loadFromFile("files/images/face_win.png")) {
        cout << "FILE NOT FOUND!" << endl;
    }
    sf::Sprite winFace;
    winFace.setTexture(winFaceText);
    winFace.setPosition((cols/2.0f)*32 - 32, (rows+0.5f)*32);

    sf::Texture loseFaceText;
    if (!loseFaceText.loadFromFile("files/images/face_lose.png")) {
        cout << "FILE NOT FOUND!" << endl;
    }
    sf::Sprite loseFace;
    loseFace.setTexture(loseFaceText);
    loseFace.setPosition((cols/2.0f)*32 - 32, (rows+0.5f)*32);

    sf::Texture leaderboardText;
    if (!leaderboardText.loadFromFile("files/images/leaderboard.png")) {
        cout << "FILE NOT FOUND!" << endl;
    }
    sf::Sprite leaderboard;
    leaderboard.setTexture(leaderboardText);
    leaderboard.setPosition((cols * 32) - 176, (rows+0.5f)*32);

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        cout << "Could not load font." << endl;
        return -1;
    }

    bool gameOver = false;
    sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
    board b(cols, rows, mines);
    int startTime = time(nullptr);
    int startPause = 0;
    int finalTime = 0;
    bool wasPaused = false;
    sf::RenderWindow* leaderBoardWindow = nullptr;
    vector<string> leaderBoardStuff = getLeaderBoard();
    while (gameWindow.isOpen()) {
        sf::Event event;
        if (leaderBoardWindow != nullptr && leaderBoardWindow->isOpen()) {
            while (gameWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    gameWindow.close();
                    leaderBoardWindow->close();
                    return 0;
                }
            }
            while (leaderBoardWindow->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    leaderBoardWindow->close();
                    leaderBoardWindow = nullptr;
                    tile::paused = wasPaused;
                    break;
                }
            }
            if (leaderBoardWindow != nullptr) {
                leaderBoardWindow->clear(sf::Color::Blue);

                sf::Text leaderTitle;
                leaderTitle.setFont(font);
                leaderTitle.setCharacterSize(20);
                leaderTitle.setColor(sf::Color::White);
                leaderTitle.setStyle(sf::Text::Bold | sf::Text::Underlined);
                leaderTitle.setString("LEADERBOARD");

                sf::FloatRect titleRect = leaderTitle.getLocalBounds();
                leaderTitle.setOrigin(titleRect.width / 2, titleRect.height / 2);
                leaderTitle.setPosition(width/4, height/4 - 120);

                sf::Text leaderEntry;
                leaderEntry.setFont(font);
                leaderEntry.setCharacterSize(18);
                leaderEntry.setColor(sf::Color::White);
                leaderEntry.setStyle(sf::Text::Bold);
                string entry = "";

                for (int i = 0; i < 5; i++) {
                    entry += to_string(i+1) + ".\t";
                    entry += leaderBoardStuff[i].substr(0,5) + "\t";
                    entry += leaderBoardStuff[i].substr(6) + "\n\n";
                }

                leaderEntry.setString(entry);
                sf::FloatRect entryRect = leaderEntry.getLocalBounds();
                leaderEntry.setOrigin(entryRect.width / 2, entryRect.height);
                leaderEntry.setPosition(width/4, height/4 + 120);

                leaderBoardWindow->draw(leaderTitle);
                leaderBoardWindow->draw(leaderEntry);
                leaderBoardWindow->display();
            }
        }
        else {
            while (gameWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    gameWindow.close();
                }
                if (event.type == sf::Event::MouseButtonReleased) {
                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (happyFace.getGlobalBounds().contains(mouseX, mouseY)) {
                            b = board(cols, rows, mines);
                            gameOver = false;
                            tile::debug = false;
                            tile::won = false;
                            tile::revealed = 0;
                            tile::flags = 0;
                            finalTime = 0;
                            startTime = time(nullptr);
                        }
                        if (leaderboard.getGlobalBounds().contains(mouseX, mouseY)) {
                            if (!tile::paused) {
                                tile::paused = true;
                                if (tile::paused) {
                                    startPause = time(nullptr);
                                }
                            }
                            leaderBoardStuff = getLeaderBoard();
                            leaderBoardWindow = new sf::RenderWindow(sf::VideoMode(width / 2, height / 2), "LEADERBOARD", sf::Style::Close);
                        }
                        if (gameOver || tile::won) {
                            continue;
                        }
                        if (pause.getGlobalBounds().contains(mouseX, mouseY)) {
                            tile::paused = !tile::paused;
                            if (tile::paused) {
                                startPause = time(nullptr);
                            }
                            wasPaused = tile::paused;
                        }
                        if (!tile::paused && debug.getGlobalBounds().contains(mouseX, mouseY)) {
                            tile::debug = !tile::debug;
                        }
                    }
                    if (tile::paused || gameOver || tile::won) {
                        continue;
                    }
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (mouseY < rows * 32) {
                            if (!b.getTile(mouseX / 32, mouseY / 32).revealTile(b.boardTiles, cols, rows)) {
                                gameOver = true;
                                b.revealMines();
                            }
                        }
                    }
                    if (event.mouseButton.button == sf::Mouse::Right) {
                        if (mouseY < rows * 32) {
                            b.getTile(mouseX / 32, mouseY / 32).toggleFlag();
                        }
                    }
                }
            }
        }
        gameWindow.clear(sf::Color::White);
        b.drawBoard(gameWindow);
        if (tile::paused) {
            gameWindow.draw(play);
        }
        else {
            gameWindow.draw(pause);
        }
        gameWindow.draw(debug);
        if (tile::won) {
            gameWindow.draw(winFace);
        }
        else if (gameOver) {
            gameWindow.draw(loseFace);
        }
        else {
            gameWindow.draw(happyFace);
        }
        drawNum(mines-tile::flags,33, 32*(rows+0.5f)+16,gameWindow,3);
        int currTime = time(nullptr);
        int pauseTime = 0;
        if (tile::paused) {
            pauseTime = currTime-startPause;
        }
        else if (startPause != 0) {
            startTime += currTime-startPause;
            startPause = 0;
        }
        else if (finalTime == 0 && (gameOver || tile::won)) {
            finalTime = currTime-startTime;
            if (tile::won) {
                clearAsterisk();
                updateLeaderBoard(finalTime, validUsername);
            }
        }
        int elapsed = currTime-startTime-pauseTime;
        if (gameOver || tile::won) {
            elapsed = finalTime;
        }
        drawNum(elapsed/60,(cols*32)-97, 32*(rows+0.5f)+16,gameWindow,2);
        drawNum(elapsed%60,(cols*32)-54, 32*(rows+0.5f)+16,gameWindow,2);
        gameWindow.draw(leaderboard);
        gameWindow.display();
    }

    return 0;
}

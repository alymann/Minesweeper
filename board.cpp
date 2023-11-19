#include "board.h"
#include"tile.h"
#include<iostream>
#include<SFML/Graphics.hpp>
#include<time.h>
#include<random>
using namespace std;

board::board(int width, int height, int mines) {
    this->width = width;
    this->height = height;
    this->mines = mines;
    tile::mines = mines;
    setTextures();

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            tile t(j, i);
            boardTiles.push_back(t);
        }
    }
    randomMines();
}

void board::randomMines() {
    srand(clock());
    vector<int> positions;
    for (int i = 0; i < height*width; i++) {
        positions.push_back(i);
    }

    for (int i = 0; i < mines; i++) {
        int ind = rand()%positions.size();
        int curr = positions[ind];
        positions.erase(positions.begin()+ind);
        tile& t = boardTiles[curr];
        t.plantMine();
        int x = t.x;
        int y = t.y;
        for (int yDif = -1; yDif <= 1; yDif++) {
            if (y + yDif < 0 || y+yDif >= height) {
                continue;
            }
            for (int xDif = -1; xDif <= 1; xDif++) {
                if (x + xDif < 0 || x+xDif >= width) {
                    continue;
                }
                boardTiles[(y+yDif)*width + (x+xDif)].countAdjMines();
            }
        }
    }
}

void board::setTextures() {
    sf::Texture* flag = new sf::Texture();
    if (!flag->loadFromFile("files/images/flag.png")) {
        cout << "File not found!" << endl;
    }
    boardTextures["flag"] = *flag;

    sf::Texture* mine = new sf::Texture();
    if (!mine->loadFromFile("files/images/mine.png")) {
        cout << "File not found!" << endl;
    }
    boardTextures["mine"] = *mine;

    sf::Texture* num1 = new sf::Texture();
    if (!num1->loadFromFile("files/images/number_1.png")) {
        cout << "File not found!" << endl;
    }
    boardTextures["number_1"] = *num1;

    sf::Texture* num2 = new sf::Texture();
    if (!num2->loadFromFile("files/images/number_2.png")) {
        cout << "File not found!" << endl;
    }
    boardTextures["number_2"] = *num2;

    sf::Texture* num3 = new sf::Texture();
    if (!num3->loadFromFile("files/images/number_3.png")) {
        cout << "File not found!" << endl;
    }
    boardTextures["number_3"] = *num3;

    sf::Texture* num4 = new sf::Texture();
    if (!num4->loadFromFile("files/images/number_4.png")) {
        cout << "File not found!" << endl;
    }
    boardTextures["number_4"] = *num4;

    sf::Texture* num5 = new sf::Texture();
    if (!num5->loadFromFile("files/images/number_5.png")) {
        cout << "File not found!" << endl;
    }
    boardTextures["number_5"] = *num5;

    sf::Texture* num6 = new sf::Texture();
    if (!num6->loadFromFile("files/images/number_6.png")) {
        cout << "File not found!" << endl;
    }
    boardTextures["number_6"] = *num6;

    sf::Texture* num7 = new sf::Texture();
    if (!num7->loadFromFile("files/images/number_7.png")) {
        cout << "File not found!" << endl;
    }
    boardTextures["number_7"] = *num7;

    sf::Texture* num8 = new sf::Texture();
    if (!num8->loadFromFile("files/images/number_8.png")) {
        cout << "File not found!" << endl;
    }
    boardTextures["number_8"] = *num8;

    sf::Texture* hidden = new sf::Texture();
    if (!hidden->loadFromFile("files/images/tile_hidden.png")) {
        cout << "File not found!" << endl;
    }
    boardTextures["tile_hidden"] = *hidden;

    sf::Texture* revealed = new sf::Texture();
    if (!revealed->loadFromFile("files/images/tile_revealed.png")) {
        cout << "File not found!" << endl;
    }
    boardTextures["tile_revealed"] = *revealed;
}

tile& board::getTile(int x, int y) {
    return boardTiles[(y)*width + (x)];
}

void board::revealMines() {
    for (int i = 0; i < boardTiles.size(); i++) {
        if (boardTiles[i].mine) {
            boardTiles[i].forceReveal();
        }
    }
}

void board::drawBoard(sf::RenderWindow& window) {
    for (int i = 0; i < boardTiles.size(); i++) {
        boardTiles[i].drawTile(window, boardTextures);
    }
}

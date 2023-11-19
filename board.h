#ifndef BOARD_H
#define BOARD_H

#include<iostream>
#include<SFML/Graphics.hpp>
#include"tile.h"
#include<vector>
#include<map>
using namespace std;

class board {
    int width;
    int height;
    int mines;
    map<string, sf::Texture> boardTextures;
public:
    vector<tile> boardTiles;
    board(int width, int height, int mines);
    void randomMines();
    void setTextures();
    tile& getTile(int x, int y);
    void revealMines();
    void drawBoard(sf::RenderWindow& window);
};


#endif //BOARD_H

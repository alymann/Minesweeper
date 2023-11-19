#ifndef TILE_H
#define TILE_H

#include<iostream>
#include<SFML/Graphics.hpp>
using namespace std;

class tile {
    bool hidden = true;
    bool flag = false;
    int adjMines = 0;
    sf::Sprite tileBase;
    sf::Sprite tileFeature;
public:
    static bool won;
    static int mines;
    static bool debug;
    static bool paused;
    static int revealed;
    static int flags;
    int x = 0;
    int y = 0;
    bool mine = false;
    tile(int x, int y);
    void toggleFlag();
    void plantMine();
    bool revealTile(vector<tile>& boardTiles, int width, int height);
    void forceReveal();
    void countAdjMines();
    void drawTile(sf::RenderWindow& window, map<string, sf::Texture>& boardTextures);
};


#endif //TILE_H

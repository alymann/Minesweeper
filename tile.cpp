#include "tile.h"
#include<iostream>
#include<SFML/Graphics.hpp>
using namespace std;

bool tile::debug = false;
bool tile::paused = false;
int tile::revealed = 0;
int tile::mines = 0;
int tile::flags = 0;
bool tile::won = false;

tile::tile(int x, int y) {
    this->x = x;
    this->y = y;
    tileBase.setPosition(x*32, y*32);
    tileFeature.setPosition(x*32, y*32);
}

void tile::toggleFlag() {
    flag = !flag;
    if (flag) {
        flags++;
    }
    else {
        flags--;
    }
}

void tile::plantMine() {
    mine = true;
}

bool tile::revealTile(vector<tile>& boardTiles, int width, int height) {
    if (!hidden || flag) {
        return true;
    }
    hidden = false;
    if (mine) {
        return false;
    }
    revealed++;
    if ((width*height-mines) == revealed) {
        won = true;
    }
    if (adjMines == 0) {
        for (int yDif = -1; yDif <= 1; yDif++) {
            if (y + yDif < 0 || y+yDif >= height) {
                continue;
            }
            for (int xDif = -1; xDif <= 1; xDif++) {
                if (x + xDif < 0 || x+xDif >= width) {
                    continue;
                }
                boardTiles[(y+yDif)*width + (x+xDif)].revealTile(boardTiles, width, height);
            }
        }
    }
    return true;
}

void tile::forceReveal() {
    hidden = false;
}

void tile::countAdjMines() {
    adjMines++;
}

void tile::drawTile(sf::RenderWindow& window, map<string, sf::Texture>& boardTextures) {
    if (paused) {
        tileBase.setTexture(boardTextures["tile_revealed"]);
        window.draw(tileBase);
        return;
    }
    if (hidden) {
        tileBase.setTexture(boardTextures["tile_hidden"]);
        window.draw(tileBase);
        if (flag) {
            tileFeature.setTexture(boardTextures["flag"]);
            window.draw(tileFeature);
        }
        else if (won) {
            toggleFlag();
            tileFeature.setTexture(boardTextures["flag"]);
            window.draw(tileFeature);
        }
        if (debug && mine) {
            tileFeature.setTexture(boardTextures["mine"]);
            window.draw(tileFeature);
        }
    }
    else {
        tileBase.setTexture(boardTextures["tile_revealed"]);
        window.draw(tileBase);
        if (mine) {
            tileFeature.setTexture(boardTextures["mine"]);
            window.draw(tileFeature);
        }
        else if (adjMines > 0) {
            tileFeature.setTexture(boardTextures["number_"+to_string(adjMines)]);
            window.draw(tileFeature);
        }
    }
}

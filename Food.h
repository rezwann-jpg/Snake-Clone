#ifndef FOOD_H
#define FOOD_H

#pragma once

#include "include/SDL.h"

class Food
{
public:
    Food(int screeenWidth, int screenHeight, int cellSize);
    void respawn(int screeenWidth, int screenHeight, int cellSize);
    int GetX() const;
    int GetY() const;
    void render(SDL_Renderer *renderer);
    ~Food();

private:
    int x, y;
    int cellSize;
};

#endif
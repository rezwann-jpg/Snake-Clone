#ifndef SNAKE_H
#define SNAKE_H

#pragma once

#include "include/SDL.h"
#include <vector>

enum class Direction { UP, DOWN, LEFT, RIGHT };

class Snake {
public:
    Snake(int cellSize);
    void changeDirection(Direction newDirection);
    void move();
    bool checkSelfCollision() const;
    bool checkWallCollision(int screenWidth, int screenHeight) const;
    bool eatFood(const class Food& food);
    void render(SDL_Renderer *renderer);
    ~Snake();

private:
    struct point {
        int x, y;
    };

    std::vector<point> body;
    Direction direction;
    int cellSize; 

};

#endif
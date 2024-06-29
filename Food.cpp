#include "Food.h"
#include <cstdlib>
#include <ctime>

Food::Food(int screeenWidth, int screenHeight, int cellSize) : cellSize(cellSize) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    respawn(screeenWidth, screenHeight, cellSize);
}

Food::~Food() {

}

void Food::respawn(int screeenWidth, int screenHeight, int cellSize) {
    x = std::rand() % (screeenWidth / cellSize);
    y = std::rand() % (screenHeight / cellSize);
}

int Food::GetX() const {
    return x;
}

int Food::GetY() const {
    return y;
}

void Food::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 240, 117, 170, 255);
    SDL_Rect rect = { x * cellSize, y * cellSize, cellSize, cellSize };
    SDL_RenderFillRect(renderer, &rect);
}
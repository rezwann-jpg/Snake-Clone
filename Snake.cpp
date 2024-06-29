#include "Snake.h"
#include "Food.h"

Snake::Snake(int cellSize) :direction(Direction::RIGHT), cellSize(cellSize) {
    body.push_back({ 10, 10 });
}

Snake::~Snake() {

}

void Snake::changeDirection(Direction newDirection) {
    if ((direction == Direction::UP && newDirection != Direction::DOWN) || 
        (direction == Direction::DOWN && newDirection != Direction::UP) ||
        (direction == Direction::LEFT && newDirection != Direction::RIGHT) ||
        (direction == Direction::RIGHT && newDirection != Direction::LEFT)) {
            direction = newDirection;
    }
}

void Snake::move() {
    point newHead = body.front();

    switch (direction) {
        case Direction::UP:         newHead.y -= 1; break;
        case Direction::DOWN:       newHead.y += 1; break;
        case Direction::LEFT:       newHead.x -= 1; break;
        case Direction::RIGHT:      newHead.x += 1; break;
    }

    body.insert(body.begin(), newHead);
    body.pop_back();
}

bool Snake::checkSelfCollision() const {
    if (body.size() <= 4) {
        return false;
    }

    const point& head = body.front();

    for (size_t i = 4; i < body.size(); i++) {
        if (head.x == body[i].x && head.y == body[i].y) {
            return true;
        }
    }

    return false;
}

bool Snake::checkWallCollision(int screenWidth, int screenHeight) const {
    const point& head = body.front();

    return head.x < 0 || head.y < 0 || head.x >= screenWidth / cellSize || head.y >= screenHeight / cellSize;
} 

bool Snake::eatFood(const Food& food) {
    point head = body.front();

    if (head.x == food.GetX() && head.y == food.GetY()) {
        body.push_back({ -1, -1 });
        return true;
    }

    return false;
}

void Snake::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 188, 90, 148, 255);
    for (const point& p : body) {
        SDL_Rect rect = { p.x * cellSize, p.y * cellSize, cellSize, cellSize };
        SDL_RenderFillRect(renderer, &rect);
    }
}
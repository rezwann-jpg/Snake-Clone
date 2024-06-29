#define SDL_MAIN_HANDLED
#include <iostream>
#include <string>
#include "include/SDL.h"
#include "include/SDL_ttf.h"
#include "Snake.h"
#include "Food.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CELL_SIZE 20

enum class GameState {
    PLAYING,
    GAME_OVER
};

bool InitSDL(SDL_Window*& window, SDL_Renderer*& renderer, TTF_Font*& font) {
     if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "SDL Init Failed:" << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Snake Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        std::cerr << "SDL Window Creation Failed:" << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL Renderer Creation Failed:" << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF init error:" << TTF_GetError() << std::endl;
        return false;
    }
    
    font = TTF_OpenFont("Early Gameboy.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font" << TTF_GetError() << std::endl;
        return false;
    }

    return true;
}

void CloseSDL(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font) {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void RenderText(SDL_Renderer *renderer, TTF_Font *font, const std::string& text, int x, int y) {
    SDL_Color color = { 255, 222, 149, 255 };
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstrect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, nullptr, &dstrect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

bool ShowGameOverScreen(SDL_Renderer *renderer, TTF_Font *font, int score, int& highScore) {
    SDL_SetRenderDrawColor(renderer, 173, 216, 153, 255);
    SDL_RenderClear(renderer);

    RenderText(renderer, font, "Game Over!", WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100);
    RenderText(renderer, font, "Score: " + std::to_string(score), WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50);
    
    if (score > highScore) {
        highScore = score;
        RenderText(renderer, font, "New High Score!", WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2);
    }
    else {
        RenderText(renderer, font, "High Score: " + std::to_string(highScore), WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2);
    }

    RenderText(renderer, font, "Press R to Restart", WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 + 50);
    RenderText(renderer, font, "Press Q to Quit", WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 + 100);

    SDL_RenderPresent(renderer);

    SDL_Event e;
    while (true) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_r) {
                    return true;
                }
                else if (e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_ESCAPE) {
                    return false;
                }
            }
            else if (e.type == SDL_QUIT) {
                return false;
            }
        }
    }
}

void ResetGame(Snake& snake, Food& food, int& score) {
    snake = Snake(CELL_SIZE);
    food.respawn(WINDOW_WIDTH, WINDOW_HEIGHT, CELL_SIZE);
    score = 0;
}

int main(int argc, char const *argv[]) {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    TTF_Font *font = nullptr;

    if (!InitSDL(window, renderer, font)) {
        std::cerr << "Failed to initialize SDL" << std::endl;
        return -1;
    }

    Snake snake(CELL_SIZE);
    Food food(WINDOW_WIDTH, WINDOW_HEIGHT, CELL_SIZE);
    GameState gameState = GameState::PLAYING;

    bool running = true;
    SDL_Event event;
    Uint32 lastMoveTime  = 0;
    const Uint32 MOVE_DELAY = 100;
    int score = 0;
    int highScore = 0;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (gameState == GameState::PLAYING) {
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            snake.changeDirection(Direction::UP);
                            break;
                        case SDLK_DOWN:
                            snake.changeDirection(Direction::DOWN);
                            break;
                        case SDLK_LEFT:
                            snake.changeDirection(Direction::LEFT);
                            break;
                        case SDLK_RIGHT:
                            snake.changeDirection(Direction::RIGHT);
                            break;
                    }
                }
                else {
                    running = false;
                }
            }
        }

        if (gameState == GameState::PLAYING) {
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime > lastMoveTime + MOVE_DELAY) {
                snake.move();

                if (snake.checkWallCollision(WINDOW_WIDTH, WINDOW_HEIGHT)) {
                    gameState = GameState::GAME_OVER;
                }

                if (snake.checkSelfCollision()) {
                    gameState = GameState::GAME_OVER;
                }

                if (snake.eatFood(food)) {
                    food.respawn(WINDOW_WIDTH, WINDOW_HEIGHT, CELL_SIZE);
                    score += 10;
                }

                lastMoveTime = currentTime;
            }
        
            SDL_SetRenderDrawColor(renderer, 173, 216, 153, 255);
            SDL_RenderClear(renderer);

            snake.render(renderer);
            food.render(renderer);
            RenderText(renderer, font, "Score: " + std::to_string(score), 10, 10);

            SDL_RenderPresent(renderer);
        }
        else if (gameState == GameState::GAME_OVER) {
            bool restart = ShowGameOverScreen(renderer, font, score, highScore);
            if (restart) {
                ResetGame(snake, food, score);
                gameState = GameState::PLAYING;
            }
            else {
                running = false;
            }
        }
    }

    CloseSDL(window, renderer, font);

    return 0;
}
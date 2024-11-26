#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

typedef struct Player Player;

typedef struct Game
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Rect rect;
}Game;

SDL_Point *GetMouse(Game *game, Player *player);
void BlitImage(Game *game, const char *image_path, int x, int y);
void runGameLoop(Game *game, Player *player);

#endif
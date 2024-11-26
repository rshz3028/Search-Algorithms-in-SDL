#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

typedef struct Game Game;

typedef struct Player
{
    SDL_Rect rect;
    int speed;
    SDL_Texture *texture;
    SDL_Point target;
}Player;

Player *CreatePlayer(Game *Game, int x, int y);
void UpdatePlayer(Player *player, Game *game);
void KillPlayer(Player *player);

#endif
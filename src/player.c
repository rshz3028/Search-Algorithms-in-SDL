#include <stdio.h>
#include <SDL2/SDL.h>

#include <player.h>
#include <game.h>

Player *CreatePlayer(Game *Game, int x, int y)
{
    Player *player = malloc(sizeof(Player));
    if(player==NULL)
    {
        printf("Player null error\n %s",SDL_GetError());
        return NULL;
    }
    player->rect = (SDL_Rect){x,y,32,32};
    player->speed = 50;

    SDL_Surface *surface = SDL_LoadBMP("../assets/player.bmp");
    if(surface==NULL)
    {
        printf("Player surface null\n %s",SDL_GetError());
        free(player);
        return NULL;
    }
    player->texture = SDL_CreateTextureFromSurface(Game->renderer, surface);
    if(player->texture == NULL)
    {
        printf("player texture null\n %s",SDL_GetError());
        SDL_FreeSurface(surface);
        free(player);
        
        return NULL;
    }
    SDL_FreeSurface(surface);

    return player;
}

void UpdatePlayer(Player *player, Game *game)
{
    const Uint8 *KeyState = SDL_GetKeyboardState(NULL);
    if(KeyState[SDL_SCANCODE_UP]) player->rect.y -= player->speed;
    if(KeyState[SDL_SCANCODE_DOWN]) player->rect.y += player->speed;
    if(KeyState[SDL_SCANCODE_RIGHT]) player->rect.x += player->speed;
    if(KeyState[SDL_SCANCODE_LEFT]) player->rect.x -= player->speed;

    if(player->rect.x > 1888)player->rect.x = 1888;
    else if(player->rect.x < 0) player->rect.x = 0;
    if(player->rect.y > 1048) player->rect.y = 1048;
    else if(player->rect.y < 0) player->rect.y = 0;
    //printf("player pos: (%d,%d)\n",player->rect.x,player->rect.y);
}

void KillPlayer(Player *player)
{
    SDL_DestroyTexture(player->texture);
    free(player);
}
#include <SDL2/SDL.h>
#include <stdio.h>

#include <player.h>
#include <ai.h>
#include <game.h>


SDL_Point *GetMouse(Game *game, Player *player)
{
    SDL_Point *p = malloc(sizeof(SDL_Point)); 
    SDL_GetMouseState(&p->x, &p->y);

    player->target.x = p->x;
    player->target.y = p->y;

    return p;
}


void BlitImage(Game *game, const char *image_path, int x, int y) {   
    SDL_Surface *surface = SDL_LoadBMP(image_path);
    if (!surface) {
        printf("Surface Null error\t%s\n", SDL_GetError());
        return;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        printf("Texture Null error\t%s\n", SDL_GetError());
        return;
    }
    int text_width, text_height;
    SDL_QueryTexture(texture, NULL, NULL, &text_width, &text_height);
    SDL_Rect rect;
    rect.x = x - (text_width/2);
    rect.y = y - (text_height/2);
    rect.w = text_width;
    rect.h = text_height;

    SDL_RenderCopy(game->renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}


void runGameLoop(Game *game, Player *player)
{
    int running = 1;
    SDL_Event event;
    SDL_Point left_mouse;

    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) running = 0; 
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(event.button.button == SDL_BUTTON_LEFT)
                    {
                        SDL_Point * p = GetMouse(game, player);
                        left_mouse.x = p->x;
                        left_mouse.y = p->y;
                        free(p);
                        Node *path = BSF(game, player);
                        //PrintPath(path);
                        printf("target aquired: %d,%d\n",path->data.x,path->data.y);
                    }
            }
        }

        SDL_SetRenderDrawColor(game->renderer, 255,255,255,255);
        SDL_RenderClear(game->renderer);

        BlitImage(game,"../assets/click_spot.bmp",left_mouse.x, left_mouse.y);
        SDL_RenderCopy(game->renderer, player->texture, NULL, &player->rect);
        UpdatePlayer(player, game);
        SDL_RenderPresent(game->renderer);
        SDL_Delay(16);
    }
}
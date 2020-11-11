#include "gui_main.h"

#include <stdio.h>
#include "SDL.h"


int gui_main()
{
    SDL_Window* window;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
            "Hello there",
            SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
            800,600,
            SDL_WINDOW_RESIZABLE
            );

    if(window == nullptr)
    {
        printf("Failed to open window. %s\n", SDL_GetError());
        return 0;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);


    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255,0,0,0);
    SDL_RenderDrawPoint(renderer, 1, 1);


    SDL_Event event;
    bool running = true;

    while(running)
    {
        SDL_RenderPresent(renderer);
        SDL_WaitEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT: running = false; break;
        }
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

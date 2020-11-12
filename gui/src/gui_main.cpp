#include "gui_main.h"

#include <stdio.h>
#include "SDL.h"
#include "stb_image.h"
#include "font.h"



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

    if(!init_fonts())
        return 1;

    Font font;
    if(!create_font(&font, renderer, "ArialCEBold.ttf"))
        return 1;

    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0" );
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 40,40,40,0);
    SDL_RenderClear(renderer);

    render_text(renderer, &font, "H", 11, 50);

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
            case SDL_KEYDOWN:
               {
                   switch(event.key.keysym.sym)
                   {
                       case SDLK_ESCAPE: running = false; break;
                   }
               }
        }
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

/*
    int tex_width, tex_height, tex_channels;
    unsigned char* data = stbi_load("piston.png", &tex_width, &tex_height, &tex_channels, STBI_rgb_alpha);
    
    SDL_Texture* texture;
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(data, tex_width, tex_height, 32, 4*tex_width, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect texture_rect;
    texture_rect.x = 0;
    texture_rect.y = 0;
    texture_rect.w = 150;
    texture_rect.h = 50;

    stbi_image_free(data);
*/

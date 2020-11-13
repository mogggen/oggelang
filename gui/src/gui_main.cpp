#include "gui_main.h"

#include <stdio.h>
#include "SDL.h"
#include "stb_image.h"
#include "window.h"
#include "font.h"



int gui_main()
{
    Window window;

    SDL_Init(SDL_INIT_VIDEO);

    window.window = SDL_CreateWindow(
            "Hello there",
            SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
            800,600,
            SDL_WINDOW_RESIZABLE
            );

    if(window.window == nullptr)
    {
        printf("Failed to open window. %s\n", SDL_GetError());
        return 0;
    }

    window.renderer = SDL_CreateRenderer(window.window, -1, 0);

    if(!init_fonts())
        return 1;

    Font font;
    if(!create_font(&font, &window, "ArialCEBold.ttf"))
        return 1;

    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0" );
    SDL_SetRenderDrawBlendMode(window.renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(window.renderer, 40,40,40,0);
    SDL_RenderClear(window.renderer);

    render_text(&window, &font, "Hello there!", 11, 50);

    SDL_SetRenderDrawColor(window.renderer, 255,0,0,0);
    SDL_RenderDrawPoint(window.renderer, 1, 1);


    SDL_Event event;
    bool running = true;

    while(running)
    {
        SDL_RenderPresent(window.renderer);
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
    
    SDL_DestroyRenderer(window.renderer);
    SDL_DestroyWindow(window.window);
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

#include "gui_main.h"

#include <stdio.h>
#include "SDL.h"
#include "stb_image.h"
#include "window.h"
#include "buttons.h"
#include "font.h"


void button_callback(void* data)
{
    printf("button is clicked %d\n", *(int*)data);
}

int gui_main()
{
    Window window;

    SDL_Init(SDL_INIT_VIDEO);

    window.window = SDL_CreateWindow(
            "OggeLang",
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

    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );


    SDL_SetRenderDrawBlendMode(window.renderer, SDL_BLENDMODE_BLEND);

    ButtonGroup buttons = {&font};

    int button_int = 6;
    Button b = {
        2, 24, 2, 100,
        "I'm a button.",
        button_callback,
        &button_int,
    };
    add_button(&buttons, b);
    int button_int2 = 9;
    Button b2 = {
        2, 24, 103, 204,
        "I'm a button too.",
        button_callback,
        &button_int2,
    };
    add_button(&buttons, b2);

    int tex_width, tex_height, tex_channels;
    unsigned char* data = stbi_load("run.png", &tex_width, &tex_height, &tex_channels, STBI_rgb_alpha);
    
    SDL_Texture* texture;
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(data, tex_width, tex_height, 32, 4*tex_width, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

    texture = SDL_CreateTextureFromSurface(window.renderer, surface);
    SDL_Rect texture_rect;
    texture_rect.x = 2;
    texture_rect.y = 2;
    texture_rect.w = tex_width;
    texture_rect.h = tex_height;

    stbi_image_free(data);

    SDL_Event event;
    bool running = true;

    while(running)
    {
        SDL_SetRenderDrawColor(window.renderer, 40,40,40,0);
        SDL_RenderClear(window.renderer);

        draw_buttons(&buttons, &window);
        SDL_RenderCopy(window.renderer, texture, nullptr, &texture_rect);

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
            case SDL_MOUSEBUTTONDOWN:
               {
                   Point p = {event.button.x, event.button.y};
                   switch(event.button.button)
                   {
                       case SDL_BUTTON_LEFT: check_click(&buttons, p); break;
                   }
               }
            case SDL_MOUSEMOTION:
               {
                   Point p = {event.button.x, event.button.y};
                   check_enter(&buttons, p);
               }
        }


    }
    
    SDL_DestroyRenderer(window.renderer);
    SDL_DestroyWindow(window.window);
    SDL_Quit();

    return 0;
}

/*
*/

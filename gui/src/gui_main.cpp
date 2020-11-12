/*#include "gui_main.h"

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

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, nullptr, &texture_rect);
    SDL_SetRenderDrawColor(renderer, 255,0,0,0);
    render_text(renderer);

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
}*/
#include <iostream>
#include <vector>
#include <algorithm>
#include "SDL.h"

void fillTexture(SDL_Renderer *renderer, SDL_Texture *texture, int r, int g, int b, int a)
{
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, NULL);
}

void prepareForRendering(SDL_Renderer *renderer)
{
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
}

void checkSdlError()
{
    const char *sdlError = SDL_GetError();
    if(sdlError && *sdlError)
    {
        ::std::cout << "SDL ERROR: " << sdlError << ::std::endl;
    }
}

int gui_main()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_HAPTIC);

    SDL_Window *window = SDL_CreateWindow("SDL test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        320, 240,
        SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    const int width = 50;
    const int height = 50;

    ::std::vector<SDL_Texture*> textures;

    SDL_Texture *redTexture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    textures.push_back(redTexture);

    SDL_Texture *greenTexture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    textures.push_back(greenTexture);

    SDL_Texture *purpleTexture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    textures.push_back(purpleTexture);

    // Here is setting the blend mode for each and every used texture:
    for(int i = 0; i < textures.size(); ++i)
    {
        SDL_SetTextureBlendMode(textures[i], SDL_BLENDMODE_BLEND);
    }

    int purpleAlpha = 0;

    fillTexture(renderer, redTexture, 255, 0, 0, 255);
    fillTexture(renderer, greenTexture, 0, 255, 0, 128);
    fillTexture(renderer, purpleTexture, 255, 0, 255, purpleAlpha);

    prepareForRendering(renderer);

    bool running = true;
    while(running)
    {
        SDL_Rect rect;
        rect.w = width;
        rect.h = height;

        SDL_RenderClear(renderer);

        rect.x = 50;
        rect.y = 50;
        SDL_RenderCopy(renderer, redTexture, NULL, &rect);

        rect.x = 75;
        rect.y = 70;
        SDL_RenderCopy(renderer, greenTexture, NULL, &rect);

        rect.x = 75;
        rect.y = 30;
        SDL_RenderCopy(renderer, purpleTexture, NULL, &rect);

        SDL_RenderPresent(renderer);

        // Process events
        {
            SDL_Event event;
            while(SDL_PollEvent(&event) == 1)
            {
                if(event.type == SDL_QUIT)
                {
                    running = false;
                }
                else if(event.type == SDL_KEYDOWN)
                {
                    switch(event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_a:
                        purpleAlpha = ::std::max(purpleAlpha - 32, 0);
                        fillTexture(renderer, purpleTexture, 255, 0, 255, purpleAlpha);
                        prepareForRendering(renderer);
                        ::std::cout << "Alpha: " << purpleAlpha << ::std::endl;
                        break;
                    case SDLK_s:
                        purpleAlpha = ::std::min(purpleAlpha + 32, 255);
                        fillTexture(renderer, purpleTexture, 255, 0, 255, purpleAlpha);
                        prepareForRendering(renderer);
                        ::std::cout << "Alpha: " << purpleAlpha << ::std::endl;
                        break;
                    }
                }
            }

            checkSdlError();
        }
    }

    for(int i = 0; i < textures.size(); ++i)
    {
        SDL_DestroyTexture(textures[i]);
    }
    textures.clear();

    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();

    checkSdlError();

    return 0;
}

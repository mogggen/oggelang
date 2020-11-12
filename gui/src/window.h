#ifndef WINDOW_H
#define WINDOW_H
#include "SDL.h"

struct Window
{
    SDL_Window* window;
    SDL_Renderer* renderer;
};

bool create_window(Window* window, const char* title);

#endif

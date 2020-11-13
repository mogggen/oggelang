#ifndef WINDOW_H
#define WINDOW_H

#include "SDL.h"
#include "frame.h"

struct Color
{
    union {
        struct {
            char r;
            char g;
            char b;
            char a;
        };
        int color;
    };
};

struct Point
{
    int x,y;
};

struct Window
{
    SDL_Window* window;
    SDL_Renderer* renderer;
};

bool create_window(Window* window, const char* title);

#endif

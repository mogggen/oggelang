#ifndef WINDOW_H
#define WINDOW_H

#include "SDL.h"
#include "frame.h"

struct Color
{
    union {
        unsigned int color;
        struct {
            char a;
            char b;
            char g;
            char r;
        };
    };
};

const Color COLOR_WHITE = Color{0xffffffff};
const Color COLOR_DARK  = Color{0x282828ff};
const Color COLOR_GRAY  = Color{0x32302fff};
const Color COLOR_LIGHT = Color{0xfbf1c7ff};

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
void draw_rect(Window* window, Color color, Point max, Point min);
void draw_rect_fill(Window* window, Color color, Point max, Point min);

#endif

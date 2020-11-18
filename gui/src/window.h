#ifndef WINDOW_H
#define WINDOW_H

#include "SDL.h"

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
const Color COLOR_DARK2 = Color{0x504945ff};
const Color COLOR_GRAY  = Color{0x928374ff};
const Color COLOR_LIGHT = Color{0xfbf1c7ff};

const Color COLOR_BRIGHT_RED     = Color{0xfb4934ff};
const Color COLOR_BRIGHT_GREEN   = Color{0xb8bb26ff};
const Color COLOR_BRIGHT_YELLOW  = Color{0xfabd2fff};
const Color COLOR_BRIGHT_BLUE    = Color{0x83a598ff};
const Color COLOR_BRIGHT_PURPLE  = Color{0xd3869bff};
const Color COLOR_BRIGHT_AQUA    = Color{0x8ec07cff};
const Color COLOR_BRIGHT_ORANGE  = Color{0xfe8019ff};

const Color COLOR_NEUTRAL_RED    = Color{0xcc241dff};
const Color COLOR_NEUTRAL_GREEN  = Color{0x98971aff};
const Color COLOR_NEUTRAL_YELLOW = Color{0xd79921ff};
const Color COLOR_NEUTRAL_BLUE   = Color{0x458588ff};
const Color COLOR_NEUTRAL_PURPLE = Color{0xb16286ff};
const Color COLOR_NEUTRAL_AQUA   = Color{0x689d6aff};
const Color COLOR_NEUTRAL_ORANGE = Color{0xd65d0eff};

const Color COLOR_FADED_RED      = Color{0x9d0006ff};
const Color COLOR_FADED_GREEN    = Color{0x79740eff};
const Color COLOR_FADED_YELLOW   = Color{0xb57614ff};
const Color COLOR_FADED_BLUE     = Color{0x076678ff};
const Color COLOR_FADED_PURPLE   = Color{0x8f3f71ff};
const Color COLOR_FADED_AQUA     = Color{0x427b58ff};
const Color COLOR_FADED_ORANGE   = Color{0xaf3a03ff};


struct Point
{
    int x,y;
};

struct Area
{
    int x, y, width, height;
};

inline bool in_area(Area* area, Point pos)
{
    return area->x <= pos.x && pos.x <= area->x + area->width &&
            area->y <= pos.y && pos.y <= area->y + area->height;
}

struct Image
{
    SDL_Texture* texture;
    int width, height;
};

struct Window
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width, height;
};

bool create_window(Window* window, const char* title);
void draw_line(Window* window, Color color, Point a, Point b);
void draw_rect(Window* window, Color color, Point max, Point min);
void draw_rect_fill(Window* window, Color color, Point max, Point min);

#endif

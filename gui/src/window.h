#ifndef WINDOW_H
#define WINDOW_H

#include "SDL.h"
#include "types.h"


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
void draw_rect(Window* window, Color color, Area a);
void draw_rect_fill(Window* window, Color color, Area a);

#endif

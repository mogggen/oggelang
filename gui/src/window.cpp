#include "window.h"

bool create_window(Window* window, const char* title)
{
    return false;
}

void draw_line(Window* window, Color color, Point a, Point b)
{
    SDL_SetRenderDrawColor(window->renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(window->renderer, a.x, a.y, b.x, b.y);
}

void draw_rect(Window* window, Color color, Point max, Point min)
{
    SDL_SetRenderDrawColor(window->renderer, color.r, color.g, color.b, color.a);

    SDL_Rect rect;
    rect.x = min.x;
    rect.y = min.y;
    rect.w = max.x - min.x;
    rect.h = max.y - min.y;
    
    SDL_RenderDrawRect(window->renderer, &rect);
}

void draw_rect_fill(Window* window, Color color, Point max, Point min)
{
    SDL_SetRenderDrawColor(window->renderer, color.r, color.g, color.b, color.a);

    SDL_Rect rect;
    rect.x = min.x;
    rect.y = min.y;
    rect.w = max.x - min.x;
    rect.h = max.y - min.y;
    
    SDL_RenderFillRect(window->renderer, &rect);
}

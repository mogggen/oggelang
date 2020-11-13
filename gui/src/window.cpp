#include "window.h"

bool create_window(Window* window, const char* title);

void draw_rect(Window* window, Color color, Point max, Point min)
{
    SDL_SetRenderDrawColor(window->renderer, color.r, color.g, color.b, color.a);
    
}

#ifndef FONT_H
#define FONT_H

#include <unordered_map>
#include "SDL.h"
#include "window.h"

struct Character
{
    bool is_valid = false;
    SDL_Texture* texture = nullptr;
    int width,height;
    int top, left;
    int advance;
};

struct Font
{
    const char* filename;
    Character characters[256];
};

bool init_fonts();
bool create_font(Font* font, Window* window, const char* filename);
void render_text(Window* window, Font* font, const char* text, int x, int y);

#endif

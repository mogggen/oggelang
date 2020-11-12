#ifndef FONT_H
#define FONT_H

#include <unordered_map>
#include "SDL.h"

struct Character
{
    bool is_valid = false;
    SDL_Texture* texture = nullptr;
    int width, height;
    int advance;
};

struct Font
{
    const char* name;
    Character characters[256];
};

bool init_fonts();
bool create_font(Font* font, SDL_Renderer* renderer, const char* filename);
void render_text(SDL_Renderer* renderer, Font* font, const char* text);

#endif

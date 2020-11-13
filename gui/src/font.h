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
    int size;
    Character characters[256];
};

bool init_fonts();
bool create_font(Font* font, Window* window, const char* filename);
void draw_text(Window* window, Font* font, const char* text, Point pos);
void get_text_size(Font* font, const char* text, int* out_width, int* out_height);

#endif

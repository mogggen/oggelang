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

bool init_fonts(Window* window);
void draw_text(Window* window, const Font* font, const char* text, Point pos);
void draw_text(Window* window, const Font* font, const char* text, Point pos, char end_char, Color color);
void get_text_size(const Font* font, const char* text, int* out_width, int* out_height);

const Font* get_regular_font();
const Font* get_monospace_font();

#endif

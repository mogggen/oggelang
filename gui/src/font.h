#ifndef FONT_H
#define FONT_H

#include <unordered_map>
#include "SDL.h"
#include "types.h"
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

struct DrawTextCursor
{
    const Font* font;
    int start_xpos;
    int line_spacing;
    Point pos;
};

bool init_fonts(Window* window);
int draw_text(Window* window, const Font* font, const char* text, Point pos);
int draw_text(Window* window, const Font* font, const char* text, Point pos, char end_char, Color color);
void draw_char(Window* window, DrawTextCursor* cursor, char c, Color color);

void get_text_size(const Font* font, const char* text, int* out_width, int* out_height);
DrawTextCursor create_cursor(const Font* font, Point pos);
void next_line(DrawTextCursor* cursor);

const Font* get_regular_font();
const Font* get_monospace_font();

#endif

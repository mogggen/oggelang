#include "font.h"

#include "ft2build.h"
#include FT_FREETYPE_H

FT_Library library;
SDL_Color colors_white[256];

bool init_fonts()
{
    if(FT_Init_FreeType(&library))
    {
        printf("Failed to init freetype.\n");
        return false;
    }
    
    for(int i=0; i<256; i++)
    {
        colors_white[i].r = i;
        colors_white[i].g = i;
        colors_white[i].b = i;
        colors_white[i].a = i;
    }

    return true;
}

bool create_font(Font* font, SDL_Renderer* renderer, const char* filename)
{

    FT_Face face;

    int error;

    error = FT_New_Face( library, "CallingCode-Regular.ttf", 0, &face );

    if(error)
    {
        printf("Failed to load font.\n");
        return false;
    }

    error = FT_Set_Char_Size( face, 0, 16*64, 300, 300 );
    error = FT_Set_Pixel_Sizes( face, 0, 32 );

    int loaded_chars = 0;

    for(int i=0; i <= 255; i++)
    {
        char c = (char)i;
        Character* character = font->characters+i;
    
        int char_index = FT_Get_Char_Index(face, c);
        if(char_index == 0)
            continue;

        error = FT_Load_Glyph( face, char_index, 0 );
        if(error != 0)
            continue;

        error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
        if(error != 0)
            continue;

        int width = face->glyph->bitmap.width;
        int rows = face->glyph->bitmap.rows;

        if(width != 0 && rows != 0)
        {
            SDL_Texture* texture;
            SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(face->glyph->bitmap.buffer,
                    width, rows,
                    8,
                    face->glyph->bitmap.pitch,
                    0x00000000, 0x00000000, 0x00000000, 0x000000ff);
            //format);



            SDL_SetPaletteColors(surface->format->palette, colors_white, 0, 256);

            if(surface == nullptr)
            {
                printf("Failed create surface. %s\n", SDL_GetError());
                continue;
            }

            texture = SDL_CreateTextureFromSurface(renderer, surface);
            if(texture == nullptr)
            {
                printf("Failed create texture. %s\n", SDL_GetError());
                continue;
            }

            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
            character->texture = texture;
        }

        character->is_valid = true;
        character->width = width;
        character->height = rows;
        character->advance = face->glyph->advance.x/64;

        loaded_chars++;
    }

    printf("Loaded %d chars\n", loaded_chars);
    return true;
}

void render_text(SDL_Renderer* renderer, Font* font, const char* text)
{
    int x_offset = 0;
    while(*text != '\0')
    {
        Character* character = font->characters+*text;
        if(!character->is_valid)
            continue;

        if(character->texture != nullptr)
        {
            SDL_Rect texture_rect;
            texture_rect.x = x_offset;
            texture_rect.y = 0;
            texture_rect.w = character->width;
            texture_rect.h = character->height;

            SDL_RenderCopy(renderer, character->texture, nullptr, &texture_rect);
        }

        x_offset += character->advance;
        text++;
    }
}

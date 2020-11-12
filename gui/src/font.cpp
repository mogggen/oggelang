#include "font.h"

#include "ft2build.h"
#include FT_FREETYPE_H

FT_Library library;

void render_text(SDL_Renderer* renderer)
{

    if(FT_Init_FreeType(&library))
    {
        printf("Failed to init freetype.\n");
        return;
    }

    FT_Face face;

    int error;

    error = FT_New_Face( library, "CallingCode-Regular.ttf", 0, &face );

    if(error)
    {
        printf("Failed to load font.\n");
        return;
    }

    error = FT_Set_Char_Size( face, 0, 16*64, 300, 300 );
    error = FT_Set_Pixel_Sizes( face, 0, 16 );

    int A_index = FT_Get_Char_Index(face, 'A');

    error = FT_Load_Glyph( face, A_index, 0 );

    error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );

    int width = face->glyph->bitmap.width;
    int rows = face->glyph->bitmap.rows;

    printf("wr: %d %d\n", width, rows);

    SDL_Texture* texture;
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(face->glyph->bitmap.buffer,
                        width, rows,
                        8,
                        width,
                        0, 0, 0, 0x000000ff);
                        //SDL_PIXELFORMAT_RGB332);

    if(surface == nullptr)
    {
        printf("Failed create surface. %s\n", SDL_GetError());
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_Rect texture_rect;
    texture_rect.x = 0;
    texture_rect.y = 0;
    texture_rect.w = 400;
    texture_rect.h = 400;
    
    SDL_RenderCopy(renderer, texture, nullptr, &texture_rect);
}

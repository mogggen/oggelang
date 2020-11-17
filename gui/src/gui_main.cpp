#include "gui_main.h"

#include <stdio.h>
#include "SDL.h"
#include "stb_image.h"
#include "window.h"
#include "buttons.h"
#include "font.h"
#include "view.h"
#include "control_bar.h"
#include "buffer_view.h"
#include "buffer.h"

int gui_main()
{
    Window window;
    ControlBar control_bar;
    BufferView buffer_view;
    BufferView buffer_view2;
    
    Font font;
    Font code_font;

    Buffer buffer1;

    VSplit _split2 = VSplit((View*)&buffer_view, (View*)&buffer_view2);
    HSplit _split1 = HSplit((View*)&control_bar, (View*)&_split2, CONTROL_BAR_HEIGHT);
    View* views = &_split1;

    window.width  = 800;
    window.height = 600;

    SDL_Init(SDL_INIT_VIDEO);

    window.window = SDL_CreateWindow(
            "OggeLang",
            SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
            window.width, window.height,
            SDL_WINDOW_RESIZABLE
            );

    if(window.window == nullptr)
    {
        printf("Failed to open window. %s\n", SDL_GetError());
        return 0;
    }

    window.renderer = SDL_CreateRenderer(window.window, -1, 0);
    
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
    SDL_SetRenderDrawBlendMode(window.renderer, SDL_BLENDMODE_BLEND);

    if(!init_fonts())
        return 1;

    if(!create_font(&font, &window, "ArialCEBold.ttf", 12))
        return 1;
    if(!create_font(&code_font, &window, "UbuntuMono-Regular.ttf", 16))
        return 1;

    create_control_bar(&window, &control_bar, &font);
    buffer_from_source_file("test_programs/long_test.ogge", &buffer1);
    create_buffer_view(&buffer_view, &code_font);
    create_buffer_view(&buffer_view2, &code_font);
    set_buffer(&buffer_view, &buffer1);
    set_buffer(&buffer_view2, &buffer1);


    //int tex_width, tex_height, tex_channels;
    //unsigned char* data = stbi_load("run.png", &tex_width, &tex_height, &tex_channels, STBI_rgb_alpha);
    //
    //SDL_Texture* texture;
    //SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(data, tex_width, tex_height, 32, 4*tex_width, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

    //texture = SDL_CreateTextureFromSurface(window.renderer, surface);
    //SDL_Rect texture_rect;
    //texture_rect.x = 2;
    //texture_rect.y = 2;
    //texture_rect.w = tex_width;
    //texture_rect.h = tex_height;

    //stbi_image_free(data);

    SDL_Event event;
    bool running = true;

    Point mouse_pos = {0,0};

    while(running)
    {
        SDL_SetRenderDrawColor(window.renderer, 40,40,40,0);
        SDL_RenderClear(window.renderer);

        //draw_buffer_view(&window, &buffer_view, &area2);
        Area window_area = Area{0,0,window.width, window.height};
        views->draw(&window, &window_area);
        //SDL_RenderCopy(window.renderer, texture, nullptr, &texture_rect);

        SDL_RenderPresent(window.renderer);
        SDL_WaitEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT: running = false; break;
            case SDL_KEYDOWN:
               {
                   switch(event.key.keysym.sym)
                   {
                       case SDLK_ESCAPE: running = false; break;
                   }
               } break;
            case SDL_MOUSEBUTTONDOWN:
               {
                   switch(event.button.button)
                   {
                       case SDL_BUTTON_LEFT:  views->mouse_left_click(mouse_pos); break;
                       case SDL_BUTTON_RIGHT: views->mouse_right_click(mouse_pos); break;
                   }
               } break;
            case SDL_MOUSEWHEEL:
               {
                   views->mouse_scroll_update(event.wheel.y, mouse_pos);
               } break;
            case SDL_MOUSEMOTION:
               {
                   mouse_pos = {event.button.x, event.button.y};
                   views->mouse_enter(mouse_pos);
               } break;
            case SDL_WINDOWEVENT:
               {
                   switch(event.window.event)
                   {
                       case SDL_WINDOWEVENT_RESIZED: 
                           {
                               window.width  = event.window.data1;
                               window.height = event.window.data2;
                           } break;
                   }
               } break;
        }


    }
    
    close_buffer(&buffer1);
    SDL_DestroyRenderer(window.renderer);
    SDL_DestroyWindow(window.window);
    SDL_Quit();

    return 0;
}

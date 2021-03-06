#include "gui_main.h"

#include <stdio.h>
#include <vector>
#include "SDL.h"
#include "stb_image.h"
#include "block_alloc.h"
#include "window.h"
#include "buttons.h"
#include "font.h"
#include "view.h"
#include "control_bar.h"
#include "buffer_view.h"
#include "buffer.h"
#include "float_menu.h"
#include "file_util.h"
#include "util.h"

#define allocate_new(dest, alloc, type, construct) {\
    dest = (type *)allocate(alloc, sizeof( type ));\
    type _t = construct;\
    memcpy(dest, &_t, sizeof( type ));\
}

struct Gui
{
    BlockAlloc alloc;
    Window window;
    Font regular_font;
    Font monospace_font;

    ControlBar control_bar;
    BufferView buffer_views[2];
    View* views;

    bool float_menu_is_open = false;
    FloatMenu float_menu;

    int main_buffer = -1;

    std::vector<Buffer> buffers;
};

Gui gui;

int init_gui(Gui* gui)
{
    gui->alloc = create_block_alloc(1024);

    // init window
    gui->window.width  = 800;
    gui->window.height = 600;

    SDL_Init(SDL_INIT_VIDEO);

    gui->window.window = SDL_CreateWindow(
            "OggeLang",
            SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
            gui->window.width, gui->window.height,
            SDL_WINDOW_RESIZABLE
            );

    if(gui->window.window == nullptr)
    {
        printf("Failed to open window. %s\n", SDL_GetError());
        return 0;
    }

    gui->window.renderer = SDL_CreateRenderer(gui->window.window, -1, 0);
    
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
    SDL_SetRenderDrawBlendMode(gui->window.renderer, SDL_BLENDMODE_BLEND);

    // init fonts
    if(!init_fonts())
        return 0;

    if(!create_font(&gui->regular_font, &gui->window, "ArialCEBold.ttf", 12))
        return 0;
    if(!create_font(&gui->monospace_font, &gui->window, "UbuntuMono-Regular.ttf", 17))
        return 0;

    // init views
    
    create_control_bar(&gui->window, &gui->control_bar, &gui->regular_font);
    for(int i=0; i<2; i++)
        create_buffer_view(&gui->buffer_views[i], &gui->monospace_font);

    ViewSelect* select1;
    ViewSelect* select2;
    VSplit* split2;
    HSplit* split1;

    allocate_new(select1, gui->alloc, ViewSelect, ViewSelect((View*)&gui->buffer_views[0]))
    allocate_new(select2, gui->alloc, ViewSelect, ViewSelect((View*)&gui->buffer_views[1]))
    
    allocate_new(split2, gui->alloc, VSplit, VSplit((View*)select1, (View*)select2, 0.5f))
    allocate_new(split1, gui->alloc, HSplit, HSplit((View*)&gui->control_bar, (View*)split2, CONTROL_BAR_HEIGHT))

    gui->views = (View*)split1;

    return 1;
}

int gui_main()
{
    if(!init_gui(&gui))
        return 0;
        
    Buffer buffer1;
    //buffer_from_source_file("test_programs/long_test.ogge", &buffer1);
    //set_buffer(&gui.buffer_views[0], &buffer1);
    //set_buffer(&gui.buffer_views[1], &buffer1);

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
        SDL_SetRenderDrawColor(gui.window.renderer, 40,40,40,0);
        SDL_RenderClear(gui.window.renderer);

        //draw_buffer_view(&window, &buffer_view, &area2);
        Area window_area = Area{0,0,gui.window.width, gui.window.height};
        gui.views->draw(&gui.window, &window_area);
        if(gui.float_menu_is_open)
            draw_float_menu(&gui.window, &gui.float_menu, &gui.regular_font);
        //SDL_RenderCopy(window.renderer, texture, nullptr, &texture_rect);

        SDL_RenderPresent(gui.window.renderer);
        SDL_WaitEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT: running = false; break;
            case SDL_KEYDOWN:
               {
                   switch(event.key.keysym.sym)
                   {
                       case SDLK_ESCAPE: running = false; break;
                       case SDLK_o: 
                         {
                            if(event.key.keysym.mod & KMOD_CTRL)
                                open_file();
                         } break;
                   }
               } break;
            case SDL_MOUSEBUTTONDOWN:
               {
                   if(gui.float_menu_is_open)
                   {
                       mouse_click(&gui.float_menu, mouse_pos);
                       gui.float_menu_is_open = false;
                   }
                   else switch(event.button.button)
                   {
                       case SDL_BUTTON_LEFT:  gui.views->mouse_left_click(mouse_pos); break;
                       case SDL_BUTTON_RIGHT: gui.views->mouse_right_click(mouse_pos); break;
                   }
               } break;
            case SDL_MOUSEWHEEL:
               {
                   gui.views->mouse_scroll_update(event.wheel.y, mouse_pos);
               } break;
            case SDL_MOUSEMOTION:
               {
                   mouse_pos = {event.button.x, event.button.y};
                   if(gui.float_menu_is_open)
                       mouse_enter(&gui.float_menu, mouse_pos);
                   else
                       gui.views->mouse_enter(mouse_pos);
               } break;
            case SDL_WINDOWEVENT:
               {
                   switch(event.window.event)
                   {
                       case SDL_WINDOWEVENT_RESIZED: 
                           {
                               gui.window.width  = event.window.data1;
                               gui.window.height = event.window.data2;
                           } break;
                   }
               } break;
        }


    }
    
    for(auto b: gui.buffers)
        close_buffer(&b);
    SDL_DestroyRenderer(gui.window.renderer);
    SDL_DestroyWindow(gui.window.window);
    SDL_Quit();


    dealloc(gui.alloc);
    return 0;
}



void open_float_menu(int xpos, int ypos, const char** options, int n_options, void(*callback)(int))
{
    if(gui.float_menu_is_open)
        return;

    init_float_menu(&gui.float_menu, Point{xpos,ypos}, options, n_options, &gui.regular_font, gui.window.width, gui.window.height, callback);
    gui.float_menu_is_open = true;
}

Buffer& get_buffer(int buffer_idx)
{
    return gui.buffers[buffer_idx];
}
std::vector<Buffer>& get_buffers()
{
    return gui.buffers;
}

void reload_buffers()
{
    
}

void open_file()
{
    char filepath[1024];
    if(!get_open_file_path(&gui.window, filepath, 1024))
        return;

    // check if file is already in a buffer
    unsigned long filepath_hash = hash_djb2(filepath);
    int i = 0;
    for(Buffer& b : gui.buffers)
    {
        if(b.filepath_hash == filepath_hash)
        {
            set_buffer(&gui.buffer_views[0], i);
            return;
        }
        i++;
    }

    int filepath_len = strlen(filepath)+1;

    printf("opend new file\n");

    char* filen = (char*)allocate(gui.alloc, filepath_len);
    memcpy(filen, filepath, filepath_len);

    Buffer new_buffer;
    buffer_from_source_file(&new_buffer, filen);
    
    gui.buffers.push_back(new_buffer);
    int new_buffer_idx = gui.buffers.size()-1;

    if( gui.main_buffer < 0 )
        gui.main_buffer = new_buffer_idx;

    set_buffer(&gui.buffer_views[0], new_buffer_idx);
}

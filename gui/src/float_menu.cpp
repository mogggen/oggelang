#include "float_menu.h"

void button_callback(void* owner, void* data)
{
    FloatMenu* menu = (FloatMenu*)owner;
    printf("Picked: %s\n", menu->options[(int)data]);
    menu->callback((int)data);
}

void init_float_menu(FloatMenu* menu, Point pos, const char** options, int n_options, int win_width, int win_height, void(*callback)(int))
{
    menu->options = options;
    menu->n_options = n_options;
    menu->callback = callback;

    menu->buttons.font = get_regular_font();
    menu->buttons.callback_owner = menu;
    clear_buttons(&menu->buttons);

    menu->area.width = 0;
    menu->area.height = 0;
    menu->option_height = 0;

    int y_offset = 0;
    for(int i=0; i<n_options; i++)
    {
        Button* btn = new_button(&menu->buttons, options[i], Point{0, y_offset}, 0,0, false, button_callback, (void*)i);
        int w = btn->right - btn->left;
        int h = btn->bottom - btn->top+1;

        if(w > menu->area.width)
            menu->area.width = w;
        menu->area.height += h;
        
        if(h > menu->option_height)
            menu->option_height = h;

        y_offset += h;
    }

    for(Button& b : menu->buttons.buttons)
        b.right = menu->area.width;

    if(pos.x + menu->area.width > win_width)
        menu->area.x = win_width - menu->area.width;
    else
        menu->area.x = pos.x;
    if(pos.y + menu->area.height > win_height)
        menu->area.y = win_height - menu->area.height;
    else
        menu->area.y = pos.y;
}

void draw_float_menu(Window* window, FloatMenu* menu)
{
    draw_rect_fill(window, COLOR_DARK, Point{menu->area.x+menu->area.width, menu->area.y+menu->area.height}, Point{menu->area.x, menu->area.y});

    draw_buttons(&menu->buttons, window, Point{menu->area.x, menu->area.y});
    
    draw_rect(window, COLOR_LIGHT, Point{menu->area.x+menu->area.width, menu->area.y+menu->area.height}, Point{menu->area.x, menu->area.y});
}

void mouse_enter(FloatMenu* menu, Point mouse_pos)
{
    if(in_area(&menu->area, mouse_pos))
    {
        check_enter(&menu->buttons, mouse_pos);
    }
}
void mouse_click(FloatMenu* menu, Point mouse_pos)
{
    if(in_area(&menu->area, mouse_pos))
    {
        check_click(&menu->buttons, mouse_pos);
    }
}

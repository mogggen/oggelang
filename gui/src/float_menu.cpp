#include "float_menu.h"

const int FLOAT_MENU_PADDING = 3;

struct FloatMenu
{
    const char** options;
    int n_options;
    Area area; 
    int option_height;

    ButtonGroup buttons;

    void (*callback)(int);
};

static FloatMenu float_menu; // can only be one float menu open at a time
static bool float_menu_open = false;

void button_callback(void* owner, void* data)
{
    FloatMenu* menu = (FloatMenu*)owner;
    printf("Picked: %s\n", menu->options[(int)data]);
    float_menu_open = false;
    menu->callback((int)data);
}

void draw_float_menu(Window* window)
{
    if(float_menu.area.x + float_menu.area.width > window->width)
        float_menu.area.x = window->width - float_menu.area.width;
    else
        float_menu.area.x = float_menu.area.x;

    if(float_menu.area.y + float_menu.area.height > window->height)
        float_menu.area.y = window->height - float_menu.area.height;
    else
        float_menu.area.y = float_menu.area.y;

    draw_rect_fill(window, COLOR_DARK, Point{float_menu.area.x+float_menu.area.width, float_menu.area.y+float_menu.area.height}, Point{float_menu.area.x, float_menu.area.y});

    draw_buttons(&float_menu.buttons, window, Point{float_menu.area.x, float_menu.area.y});
    
    draw_rect(window, COLOR_LIGHT, Point{float_menu.area.x+float_menu.area.width, float_menu.area.y+float_menu.area.height}, Point{float_menu.area.x, float_menu.area.y});
}

void float_menu_mouse_enter(Point mouse_pos)
{
    if(in_area(&float_menu.area, mouse_pos))
    {
        check_enter(&float_menu.buttons, mouse_pos);
    }
}
void float_menu_mouse_click(Point mouse_pos)
{
    if(in_area(&float_menu.area, mouse_pos))
    {
        check_click(&float_menu.buttons, mouse_pos);
    }
    else 
    {
        // close float menu
        float_menu_open = false;
    }
}

void open_float_menu(int xpos, int ypos, const char** options, int n_options, void(*callback)(int))
{
    if(float_menu_open)
        return;


    float_menu_open = true;
    
    float_menu.options = options;
    float_menu.n_options = n_options;
    float_menu.callback = callback;

    float_menu.buttons.font = get_regular_font();
    float_menu.buttons.callback_owner = &float_menu;
    clear_buttons(&float_menu.buttons);

    float_menu.area.x = xpos;
    float_menu.area.y = ypos;
    float_menu.area.width = 0;
    float_menu.area.height = 0;
    float_menu.option_height = 0;

    int y_offset = 0;
    for(int i=0; i<n_options; i++)
    {
        Button* btn = new_button(&float_menu.buttons, options[i], Point{0, y_offset}, 0,0, false, button_callback, (void*)i);
        int w = btn->right - btn->left;
        int h = btn->bottom - btn->top+1;

        if(w > float_menu.area.width)
            float_menu.area.width = w;
        float_menu.area.height += h;
        
        if(h > float_menu.option_height)
            float_menu.option_height = h;

        y_offset += h;
    }

    for(Button& b : float_menu.buttons.buttons)
        b.right = float_menu.area.width;

}

bool is_float_menu_open()
{
    return float_menu_open;
}



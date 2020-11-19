#include "control_bar.h"

#include "gui_main.h"
    
void ControlBar::mouse_left_click(Point mouse_pos)
{
    check_click(&this->buttons, mouse_pos);
}
void ControlBar::mouse_enter(Point mouse_pos)
{
    check_enter(&this->buttons, mouse_pos);
}
void ControlBar::draw(Window* window, Area* area)
{
    draw_buttons(&this->buttons, window, Point{area->x, area->y});

    //draw_line(window, COLOR_GRAY, Point{0,CONTROL_BAR_HEIGHT}, Point{window->width,CONTROL_BAR_HEIGHT});
}

void open_file_button_callback(void* owner, void* data)
{
    Window* window = (Window*)data;
    printf("Open file button\n");
    open_file();
}

void run_button_callback(void* owner, void*)
{
    printf("Run button\n");
}

void stop_button_callback(void* owner, void*)
{
    printf("Stop button\n");
}

void create_control_bar(Window* window, ControlBar* bar, Font* font)
{
    bar->buttons.font = font;
    bar->buttons.callback_owner = bar;

    int x_pos = 2;
    int y_pos = 2;
     
    {
        Button* b = new_button(&bar->buttons,
                "Open",
                Point{x_pos,y_pos},
                0,20, true,
                open_file_button_callback,
                window);
        x_pos += b->right - b->left + 4;
    }
    {
        Button* b = new_button(&bar->buttons,
                "Run",
                Point{x_pos,y_pos},
                0,20, true,
                run_button_callback,
                nullptr);
        x_pos += b->right - b->left + 4;
    }
    {
        Button* b = new_button(&bar->buttons,
                "Stop",
                Point{x_pos,y_pos},
                0,20, true,
                stop_button_callback,
                nullptr);
        x_pos += b->right - b->left + 4;
    }
}

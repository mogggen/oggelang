#include "control_bar.h"

void open_file_button_callback(void*)
{
    printf("Open file button\n");
}

void run_button_callback(void*)
{
    printf("Run button\n");
}

void stop_button_callback(void*)
{
    printf("Stop button\n");
}

void create_control_bar(Window* window, ControlBar* bar, Font* font)
{
    bar->buttons.font = font;

    int x_pos = 2;
    int y_pos = 2;
     
    {
        Button* b = new_button(&bar->buttons,
                "Open",
                Point{x_pos,y_pos},
                0,20,
                open_file_button_callback,
                nullptr);
        x_pos += b->right - b->left + 4;
    }
    {
        Button* b = new_button(&bar->buttons,
                "Run",
                Point{x_pos,y_pos},
                0,20,
                run_button_callback,
                nullptr);
        x_pos += b->right - b->left + 4;
    }
    {
        Button* b = new_button(&bar->buttons,
                "Stop",
                Point{x_pos,y_pos},
                0,20,
                stop_button_callback,
                nullptr);
        x_pos += b->right - b->left + 4;
    }
}

void draw_control_bar(Window* window, ControlBar* bar)
{
    draw_buttons(&bar->buttons, window);

    draw_line(window, COLOR_GRAY, Point{0,CONTROL_BAR_HEIGHT}, Point{window->width,CONTROL_BAR_HEIGHT});
}

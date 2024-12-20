#include "control_bar.h"

#include <stdio.h>
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
    const int MAIN_FILE_TEST_X_POS = 215;
    
    draw_buttons(&this->buttons, window, Point{area->x, area->y});

    draw_text(window, buttons.font, "Main file:", Point{MAIN_FILE_TEST_X_POS-55, 5});


    if(get_main_buffer() != nullptr)
    {
        draw_text(window, buttons.font, get_main_buffer()->filename, Point{MAIN_FILE_TEST_X_POS, 5});
    }
    else
    {
        draw_text(window, buttons.font, "No file selected", Point{MAIN_FILE_TEST_X_POS, 5});
    }
}

void open_file_button_callback(void* owner, void* data)
{
    printf("Open file button\n");
    open_file();
}

void run_button_callback(void* owner, void*)
{
    printf("Run button\n");
    run_program();
}

void stop_button_callback(void* owner, void*)
{
    printf("Stop button\n");
}

void step_button_callback(void* owner, void* data)
{
    step_program();
    printf("Step button\n");
}

void create_control_bar(Window* window, ControlBar* bar)
{
    bar->buttons.font = get_regular_font();
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
    {
        Button* b = new_button(&bar->buttons,
                "Step",
                Point{x_pos,y_pos},
                0,20, true,
                step_button_callback,
                nullptr);
        x_pos += b->right - b->left + 4;
    }
}

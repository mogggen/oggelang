#include "view_select.h"
#include <stdio.h>
#include "gui_main.h"
#include "float_menu.h"

void create_view_select(ViewSelect* view)
{
    create_buffer_view(&view->buffer_view);
    view->inner = &view->buffer_view;
}
void create_view_select(ViewSelect* view, View* inner)
{
    create_view_select(view);
    view->inner = inner;
}

void ViewSelect::mouse_scroll_update(int scroll, Point mouse_pos)
{
    inner->mouse_scroll_update(scroll, mouse_pos);
}

const int VSPLIT_INDEX = 0;
const int HSPLIT_INDEX = 1;
const int CODE_BUFER_INDEX = 2;
const int N_OWN_VIEW_OPTIONS = 3;
const int N_VIEWS_OPTIONS = N_OWN_VIEW_OPTIONS + N_SELECTABLE_VIEWS;

const char* views_option[N_VIEWS_OPTIONS] = 
{
    "Vertical split",
    "Horizontal split",
    "Code buffer",
};

ViewSelect* current_view_select;

void view_select_callback(int index)
{
    if(index >= N_OWN_VIEW_OPTIONS)
        current_view_select->inner = get_selectable_views()[index-N_OWN_VIEW_OPTIONS];
    else if(index == VSPLIT_INDEX)
    {
        printf("vsplit\n");
    }
    else if(index == HSPLIT_INDEX)
    {
        printf("hsplit\n");
    }
    else if(index == CODE_BUFER_INDEX)
    {
        current_view_select->inner = &current_view_select->buffer_view;
    }
}

void ViewSelect::mouse_left_click(Point mouse_pos)
{
    if(in_area(&button_area, mouse_pos))
    {
        View** selectable_views = get_selectable_views();

        for(int i=0; i < N_SELECTABLE_VIEWS; i++)
            views_option[i+N_OWN_VIEW_OPTIONS] = selectable_views[i]->name();

        current_view_select = this;
        open_float_menu(mouse_pos.x, mouse_pos.y, views_option, N_VIEWS_OPTIONS, view_select_callback);
    }
    else
        inner->mouse_left_click(mouse_pos);
}
void ViewSelect::mouse_right_click(Point mouse_pos)
{
    inner->mouse_right_click(mouse_pos);
}
void ViewSelect::mouse_enter(Point mouse_pos)
{
    inner->mouse_enter(mouse_pos);
}

void ViewSelect::draw(Window* window, Area* area)
{
    inner->draw(window, area);
    button_area = Area{
        area->width-15,
        area->y+5,
        10,
        10
    };
    draw_rect_fill(window, COLOR_BRIGHT_PURPLE, Point{button_area.x + button_area.width, button_area.y + button_area.height}, Point{button_area.x, button_area.y});
}

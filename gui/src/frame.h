#ifndef FRAME_H
#define FRAME_H

struct Frame
{
    bool is_visible;
    void (*clicked_callback());
};

#endif

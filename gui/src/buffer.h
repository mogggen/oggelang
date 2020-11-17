#ifndef BUFFER_H
#define BUFFER_H 

#include <vector>

struct Buffer
{
    const char* filename;
    char* buffer;
    long buffer_size;
    std::vector<char*> lines;
    int selected_line;
};

void buffer_from_source_file(const char* filename, Buffer* buffer);
void close_buffer(Buffer* buffer);

#endif /* BUFFER_H */

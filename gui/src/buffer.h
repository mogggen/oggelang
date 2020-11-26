#ifndef BUFFER_H
#define BUFFER_H 

#include <vector>

struct Buffer
{
    const char* filepath;
    const char* filename;
    unsigned long filepath_hash;
    long long last_modified_time;
    char* buffer;
    long buffer_size;
    std::vector<char*> lines;
    int selected_line;
};

void buffer_from_source_file(Buffer* buffer, const char* filename);
void close_buffer(Buffer* buffer);
bool reload_buffer(Buffer* buffer);

#endif /* BUFFER_H */

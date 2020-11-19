#include "buffer.h"

#include "util.h"
#include "file_util.h"

void buffer_from_source_file(Buffer* buffer, const char* filepath)
{
    buffer->filepath = filepath;
    buffer->filepath_hash = hash_djb2(filepath);
    buffer->filename = get_filename_form_path(filepath);

    FILE* file;
    auto error_no = fopen_s(&file, filepath, "r");
    if(error_no != 0)
    {
        printf("Failed to open file %s\n", filepath);
        char error_str[] = "Failed to open file";
        auto len = strlen(error_str);
        buffer->buffer = (char*)malloc(len+1);
        memcpy(buffer->buffer, error_str, len+1);
        buffer->buffer[len] = '\0';
        buffer->buffer_size = (long)len;
        return;
    }

    fseek(file, 0L, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    buffer->buffer = (char*)malloc(file_size+1);
    buffer->buffer[file_size] = '\0';
    buffer->buffer_size = file_size;

    fread(buffer->buffer, 1, file_size, file);

    fclose(file);

    buffer->lines.clear();
    buffer->lines.push_back(buffer->buffer);
    for(int i=0; i<buffer->buffer_size; i++)
    {
        if( buffer->buffer[i] == '\n' && i+1 < buffer->buffer_size)
            buffer->lines.push_back(buffer->buffer+i+1);
    }

    buffer->selected_line = 0;
}

void close_buffer(Buffer* buffer)
{
    free(buffer->buffer);
    buffer->lines.clear();
}

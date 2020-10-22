#include "file_reader.h"

#include <stdlib.h>

bool create_file_reader(FileReader* fr, const char* filename)
{
    fr->file = fopen(filename, "r");
    if(fr->file == nullptr)
        return false;


    fr->buffer = (char*)malloc(BUFFER_SIZE*2);
    fr->other_buffer = fr->buffer+BUFFER_SIZE;
    fr->other_end = fr->other_buffer;
    fr->current = fr->buffer;
    fr->loc.line = 1;
    fr->loc.column = 1;
    fr->is_finished = false;
    fr->has_read_to_other_buffer = false;
    
    fr->end = fr->buffer + fread(fr->buffer, sizeof(char), BUFFER_SIZE, fr->file);

    return true;
}

void close(FileReader& fr)
{
    fclose(fr.file);
    if(fr.buffer < fr.other_buffer)
        free(fr.buffer);
    else
        free(fr.other_buffer);
}

char read_char(FileReader& fr)
{
    char ret = *(fr.current);

    if(ret == '\n')
    {
        fr.loc.line++;
        fr.loc.column = 1;
    }
    else
    {
        fr.loc.column++;
    }

    fr.current++;
    if(fr.current >= fr.end)
    {
        if(fr.other_buffer == fr.other_end) // other buffer hasn't been filled yet
        {
            int read_bytes = fread(fr.other_buffer, sizeof(char), BUFFER_SIZE, fr.file);
            if(read_bytes == 0)
            {
                fr.buffer[0] = 0;
                fr.is_finished = true;
            }
            fr.other_end = fr.other_buffer + read_bytes;
        }

        // swap buffers
        char* t = fr.buffer;
        fr.buffer = fr.other_buffer;
        fr.other_buffer = t;
        fr.end = fr.other_end;
        fr.other_end = fr.other_buffer; // empty other buffer

        fr.current = fr.buffer;
    }

    return ret;
}

inline bool is_white_space(char c)
{
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

inline bool is_number(char c)
{
    return '0' <= c && c <= '9';
}

void read_to_next_word(FileReader& fr)
{
    while(!is_white_space(read_char(fr)));
    while(is_white_space(*fr.current)) {read_char(fr);}
}

void read_word(FileReader& fr, char* str, int size)
{
    for(int i=0; i<size-1; i++)
    {
        if(is_white_space(*fr.current))
        {
            str[i] = '\0';
            break;
        }
        else
        {
            str[i] = read_char(fr);
            if (str[i] == 0)
                break;
        }
    }
}

void peek_word(FileReader& fr, char* str, int size)
{
    char* curr_end = fr.end;
    char* curr = fr.current;

    for (int i=0; i<size; i++)
    {
        if(curr >= curr_end)
        {
            int read_bytes = fread(fr.other_buffer, sizeof(char), BUFFER_SIZE, fr.file);
            if(read_bytes == 0) 
                return; // there isn't more to read.
            fr.other_end = fr.other_buffer+read_bytes; 
            curr_end = fr.other_end;
            curr = fr.other_buffer;
        }

        if(is_white_space(*curr))
        {
            str[i] = '\0';
            break;
        }
        else
        {
            str[i] = *(curr++);

            if (str[i] == 0)
                break;
        }
    }
}

bool read_value(FileReader& fr, int* out)
{
    char c = *fr.current;
    *out = 0;
    int sign = 1;

    while(is_white_space((c = *fr.current))) {read_char(fr);}

    if (c == '-')
    {
        sign = -1;
        c = read_char(fr);
    }
    else if(!is_number(c))
    {
        read_char(fr);
        return false;
    }

    while(is_number(*fr.current))
    {
        c = read_char(fr);
        *out *= 10;
        *out += (int)c-'0';
    }

    *out *= sign;

    return true;
}

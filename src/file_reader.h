#ifndef FILE_READER_H
#define FILE_READER_H

#include <stdio.h>

#define BUFFER_SIZE 10

struct FileLocation
{
    int line;
    int column;
};

struct FileReader
{
    FILE* file;
    char* buffer;
    char* other_buffer;
    char* current;
    char* end;
    char* other_end;
    FileLocation loc;
    bool is_finished;
    bool has_read_to_other_buffer;
};

bool create_file_reader(FileReader* fr, const char* filename);
void close(FileReader& fr);
char read_char(FileReader& fr);
void read_to_next_word(FileReader& fr);
void read_word(FileReader& fr, char* str, int size);
void peek_word(FileReader& fr, char* str, int size);
bool read_value(FileReader& fr, int* out);

#endif

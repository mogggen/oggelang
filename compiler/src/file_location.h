#ifndef FILE_LOCATION_H
#define FILE_LOCATION_H

struct FileLocation
{
    const char* filename;
    int line;
    int column;
};

#endif

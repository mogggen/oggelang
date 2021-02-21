#include "file_util.h"

void convert_to_unix_file_path(char* path)
{
    while(*path != '\0')
    {
        if(*path == '\\')
            *path = '/';
        path++;
    }
}

#ifdef WIN32

#include <windows.h>
#include <commdlg.h>
#include <stdio.h>
#include "SDL.h"
#include "SDL_syswm.h"
#include <sys/types.h>
#include <sys/stat.h>

const char* get_filename_form_path(const char* file_path)
{
    const char* last_slash = file_path;

    while(*file_path != '\0')
    {
        if(*file_path == '/')
            last_slash = file_path;
        file_path++;
    }
    return last_slash+1;
}

#define stat _stat

long long get_last_modified_time(const char* filename)
{
    struct stat result;
    if(stat(filename, &result) == 0)
        return result.st_mtime;
    else
        return 0;
}

//bool get_open_file_path(Window* window, char* file_path_buf, int buf_size)
//{
//    file_path_buf[0] = '\0';
//
//    // get HWND
//    SDL_SysWMinfo sdlinfo; 
//    SDL_version sdlver;
//    SDL_VERSION(&sdlver);
//    sdlinfo.version = sdlver;
//    SDL_GetWindowWMInfo(window->window, &sdlinfo);
//    HWND hwnd = sdlinfo.info.win.window;
//
//
//    OPENFILENAMEA ofn;
//    ZeroMemory(&ofn, sizeof(ofn));
//
//    ofn.lStructSize = sizeof(ofn);
//    ofn.hwndOwner = hwnd;
//    ofn.lpstrFile = file_path_buf;
//    ofn.nMaxFile = buf_size;
//    ofn.lpstrFilter = "Ogge Files (*.ogge)\0*.ogge\0All Files (*.*)\0*.*\0";
//    ofn.nFilterIndex = 1;
//    ofn.lpstrFileTitle = nullptr;
//    ofn.nMaxFileTitle = 0;
//    ofn.lpstrInitialDir = nullptr;
//    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
//
//    if(!GetOpenFileNameA(&ofn))
//    {
//        printf("%lu\n", CommDlgExtendedError());
//        return false;
//    }
//
//    printf("Selected: %s\n", file_path_buf);
//
//    return true;
//}

#elif __unix__

#include <stdio.h>
#include <sys/stat.h>

const char* get_filename_form_path(const char* file_path)
{
    const char* last_slash = file_path;

    while(*file_path != '\0')
    {
        if(*file_path == '/')
            last_slash = file_path;
        file_path++;
    }
    return last_slash+1;
}

long long get_last_modified_time(const char* filename)
{
    struct stat result;
    if(stat(filename, &result) == 0)
        return result.st_mtime;
    else
        return 0;
}

//bool get_open_file_path(Window* window, char* file_path_buf, int buf_size)
//{
//    FILE* f = popen("zenity --file-selection", "r");
//    fgets(file_path_buf, buf_size, f);
//    fclose(f);
//
//    // remove '\n' at end of filepath
//    char* c = file_path_buf;
//    while(*c != '\0' && *c != '\n') c++;
//    *c = '\0';
//
//    return true;
//}

#endif

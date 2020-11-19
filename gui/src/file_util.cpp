#include "file_util.h"

#ifdef WIN32
#include <windows.h>
#include <commdlg.h>
#include <stdio.h>
#include "SDL.h"
#include "SDL_syswm.h"

const char* get_filename_form_path(const char* file_path)
{
    const char* last_slash = file_path;

    while(*file_path != '\0')
    {
        if(*file_path == '\\')
            last_slash = file_path;
        file_path++;
    }
    return last_slash+1;
}

bool get_open_file_path(Window* window, char* file_path_buf, int buf_size)
{
    file_path_buf[0] = '\0';

    // get HWND
    SDL_SysWMinfo sdlinfo; 
    SDL_version sdlver;
    SDL_VERSION(&sdlver);
    sdlinfo.version = sdlver;
    SDL_GetWindowWMInfo(window->window, &sdlinfo);
    HWND hwnd = sdlinfo.info.win.window;


    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = file_path_buf;
    ofn.nMaxFile = buf_size;
    ofn.lpstrFilter = "Ogge Files (*.ogge)\0*.ogge\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if(!GetOpenFileNameA(&ofn))
    {
        printf("%lu\n", CommDlgExtendedError());
        return false;
    }

    printf("Selected: %s\n", file_path_buf);

    return true;
}

#endif

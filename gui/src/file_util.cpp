#include "file_util.h"

#ifdef WIN32
#include <windows.h>
#include <commdlg.h>
#include <stdio.h>
#include "SDL.h"
#include "SDL_syswm.h"

void get_open_file_name(Window* window)
{
    // get HWND
    SDL_SysWMinfo sdlinfo; 
    SDL_version sdlver;
    SDL_VERSION(&sdlver);
    sdlinfo.version = sdlver;
    SDL_GetWindowWMInfo(window->window, &sdlinfo);
    HWND hwnd = sdlinfo.info.win.window;


    char buf[MAX_PATH];
    buf[0] = '\0';

    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = buf;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "Ogge Files (*.ogge)\0*.ogge\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if(GetOpenFileNameA(&ofn))
    {
        printf("Selected: %s\n", buf);
    }
    else
    {
        printf("%lu\n", CommDlgExtendedError());
    }
}

#endif

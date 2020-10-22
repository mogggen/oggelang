" compile and run commands
set makeprg=ninja
nnoremap <f3> :wa<cr>:silent make! -C build\<cr>:cw<cr>
nnoremap <S-f3> :wa<cr>:!ninja -C build\ -t clean<cr>:silent make! -C build\<cr>:cw<cr>
nnoremap <f4> :10sp<cr>:terminal bin\ogge.exe<cr>
nnoremap <f2> :wa<cr>:!cmake -S . -B build\ -G "Ninja" -DCMAKE_BUILD_TYPE=Debug<cr>

nnoremap <f8> :!explorer .<cr><cr>

nnoremap <leader>g :call ToggleHeader()<cr>

:GuiFont Consolas:h10

command -nargs=1 SetExecutable call SetExe(<f-args>)

function SetExe(file)
    let command = 'nnoremap <f4> :10sp <cr>:terminal bin\\' . a:file . '.exe<cr>'
    execute command
endfunction

function ToggleHeader()
    let type = expand("%:e")
    if type=="h"
        if filereadable(expand("%:r") . ".cc")
            :e %:r.cc
        elseif filereadable(expand("%:r") . ".c")
            :e %:r.c
        else
            :e %:r.cpp
        endif
    elseif type=="cc"
        :e %:r.h
    elseif type=="cpp"
        :e %:r.h
    elseif type=="c"
        :e %:r.h
    endif
endfunction

let g:ctrlp_custom_ignore = {
  \ 'dir':  '\v[\/]\.(git|hg|svn)|target|build|Build|bin|docs$',
  \ 'file': '\v\.(exe|so|dll)$',
  \ 'link': 'some_bad_symbolic_links',
  \ }

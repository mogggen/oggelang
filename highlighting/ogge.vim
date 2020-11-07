" Vim syntax file
" Language: oggelang
" Maintainer: Magnus Lind
" Latest Revision: 2 November 2020

if exists("b:current_syntax")
    finish
endif

syn keyword oggeKeyword var if print printc
syn keyword oggeKeyword goto nextgroup=oggeLine skipwhite
syn match oggeLine '[a-z,A-Z,å,ä,ö,Å,Ä,Ö,_,\,/,0-9]*:[0-9]\+'
syn match oggeVar '[a-z,A-Z,å,ä,ö,Å,Ä,Ö,_][a-z,A-Z,å,ä,ö,Å,Ä,Ö,_,0-9]*'
syn match oggeNumber '\d\+'
syn keyword oggeNumber alloc

let b:current_syntax = "ogge"
hi def link oggeKeyword Statement
hi def link oggeVar Type
hi def link oggeNumber Constant
hi def link oggeLine PreProc


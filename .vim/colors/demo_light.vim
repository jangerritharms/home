set background=dark
highlight clear
if exists("syntax on")
    syntax reset
endif
let g:colors_name = "Demo light"

" Interface
highlight Visual ctermfg=0 ctermbg=4 cterm=NONE
highlight CursorLine ctermfg=NONE ctermbg=1 cterm=NONE

" Text
highlight Character ctermfg=6 ctermbg=NONE cterm=NONE
highlight StorageClass ctermfg=4 ctermbg=NONE cterm=NONE
highlight Keyword ctermfg=9 ctermbg=NONE cterm=NONE
highlight Function ctermfg=9 ctermbg=NONE cterm=NONE
highlight link cType cStorageClass
highlight Comment ctermfg=3 ctermbg=NONE cterm=NONE
highlight Constant ctermfg=10 ctermbg=NONE cterm=NONE
highlight Statement ctermfg=7

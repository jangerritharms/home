set background=dark
highlight clear
if exists("syntax on")
    syntax reset
endif
let g:colors_name = "Demo light"

" Interface
highlight Visual ctermfg=0 ctermbg=3 cterm=NONE
highlight CursorLine ctermfg=NONE ctermbg=8 cterm=NONE
highlight StatusLine ctermfg=0 ctermbg=15 cterm=NONE
highlight ModeMsg ctermfg=2 ctermbg=NONE cterm=NONE
highlight LineNr ctermfg=2 ctermbg=8 cterm=NONE

" Text
highlight Normal ctermfg=2 ctermbg=NONE cterm=NONE
highlight String ctermfg=5 ctermbg=NONE cterm=NONE
highlight Comment ctermfg=4 ctermbg=NONE cterm=NONE
highlight Statement ctermfg=1 ctermbg=NONE cterm=NONE
highlight Constant ctermfg=15 ctermbg=NONE cterm=NONE
highlight Type ctermfg=3 ctermbg=NONE cterm=NONE

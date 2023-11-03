section .init
global _init:function
_init:
        push ebp
        mov ebp, esp
        ; gcc will put the contents of crtbegin.o's .init section here

section .fini
global _fini:function
_fini:
        push ebp
        mov ebp, esp
        ; gcc will put the contents of crtbegin.o's .fini section hre

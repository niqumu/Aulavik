section .init
        ; gcc will put the contents of crtend.o's .init section here
        pop ebp
        ret

section .fini
        ; gcc will put the contents of crtend.o's .fini section here
        pop ebp
        ret

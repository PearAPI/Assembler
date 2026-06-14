main:
        mov ax, bx              ; 2 byte
        mov ax, 5               ; 6 byte
loop:
        add bx, 100h            ; 6 byte
        mov ax, bh              ; 2 byte
        cmp ax, 800h            ; 6 byte
        je done                 ; 6 byte
        jmp loop                ; 6 byte
done:
        jmp main                ; 6 byte

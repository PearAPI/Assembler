init_memory:
    mov bx, 1000h
    mov ax, 0
    
clear_loop:
    mov [bx], ax
    add bx, 2
    cmp bx, 2000h
    jne clear_loop
    
    hlt
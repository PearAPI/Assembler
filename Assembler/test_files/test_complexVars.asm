copy_routine:
    mov si, source_buffer
    mov di, dest_buffer
    mov cx, 50
    
copy_step:
    mov ax, [si]
    mov [di], ax
    add si, 2
    add di, 2
    loop copy_step      ; "loop" ist Instruction, "copy_step" ist Identifier

source_buffer:
dest_buffer:
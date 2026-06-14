xor ax, ax

loop:
    add ax, 1

    cmp ax, 20h
    jne loop

    hlt
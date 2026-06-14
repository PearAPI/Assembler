; Dieser Kommentar sollte vom Lexer ignoriert werden
setup_data:
mov     ax,10       ; Mehrere Tabs und kein Leerzeichen nach dem Komma
add [200h] , ax     ; Leerzeichen VOR dem Komma
mov cx, 0xFF        ; Alternative Hex-Schreibweise (falls du 0x unterstützt)

hlt                 ; Einzelner Befehl weit eingerückt
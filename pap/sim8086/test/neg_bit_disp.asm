
; https://github.com/cmuratori/computer_enhance/blob/eaded081fd8d492fa588d345e2a47a80d3ff1d48/perfaware/part1/listing_0040_challenge_movs.asm
bits 16

; Signed displacements
mov ax, [bx + di - 37]
mov [si - 300], cx
mov dx, [bx - 32]


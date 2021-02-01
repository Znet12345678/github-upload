[bits 16]
global _start
global _puts
extern _cmain
_start:
push bp
mov bp,sp
call _cmain
pop bp
ret
_puts:
push bp
mov bp,sp
mov si,[bp+4]
.l:
mov al,[si]
cmp al,0
jz .e
mov ah,0x0e
int 0x10
inc si
jmp .l
.e:
pop bp
ret

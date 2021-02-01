[bits 16]
[org 0x7c00]
section .text
_start:
xor ax,ax
xor bx,bx
xor cx,cx
xor dh,dh
mov si,ax
mov di,ax
mov sp,0xffff
push bp
mov bp,sp
mov ah,0x42
mov byte [0x2000],dl
mov word [DAP.sectors],1
mov word [DAP.lba],1
mov word [DAP.offset],0x7e00
mov bx,DAP
mov si,bx
int 0x13
jc err
jmp 0:0x7e00
pop bp
hng:jmp hng
err:
push bp
mov bp,sp
push errmsg
call puts
add sp,2
jmp hng
puts:push bp
mov bp,sp
mov bx,[bp+4]
mov ah,0x0e
mov al,[bx]
.l:
int 0x10
inc bx
mov al,[bx]
cmp al,0
jz .e
jmp .l
.e:
pop bp
ret
data:
DAP:
.size:db 0x10
.unused:db 0
.sectors:dw 0
.offset:dw 0
.segment:dw 0
.lba:dq 0
DAP_END:
errmsg:db 'Failed to load',0
times 510 - ($ - $$) db 0
dw 0xaa55


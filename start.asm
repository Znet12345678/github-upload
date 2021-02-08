[bits 16]
global _start
global _puts
extern _pent
extern _pent_end
extern _cmain
;entry point
_start:
push bp
mov bp,sp
call _cmain
pop bp
ret
;put string
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
;setup data segment to heap segment
global _dsSetup
_dsSetup:
push bp
mov bp,sp
mov ax,ds
mov es,ax
mov ax,[_pent+2]
mov ds,ax
pop bp
ret
;restore ds from es
global _dsRetire
_dsRetire:
push bp
mov bp,sp
mov ax,es
mov ds,ax
pop bp
ret
;increment ds
global _dsInc
_dsInc:
push bp
mov bp,sp
mov ax,ds
inc ax
mov ds,ax
pop bp
ret
;put character
global _putc
_putc:
push bp
mov bp,sp
mov al,[bp+4]
mov ah,0x0e
int 0x10
pop bp
ret

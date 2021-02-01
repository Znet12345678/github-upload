all: bs.bin Makefile floppy.img krnl16.com
start.o: start.asm Makefile
	nasm -f as86 start.asm -o start.o
cmain.o: cmain.c Makefile
	bcc -I -Iinclude -c cmain.c -o cmain.o -ansi
bs.bin: bs.asm Makefile
	nasm -f bin bs.asm -o bs.bin
string.o: string.c Makefile
	bcc -I -Iinclude -c string.c -o string.o -ansi
krnl16.com:start.o cmain.o string.o Makefile
	ld86 -T0x7e00  -L -d start.o cmain.o string.o -o krnl16.com
floppy.img: Makefile bs.bin krnl16.com
	cat bs.bin krnl16.com > krnl16.bin
	qemu-img create floppy.img 1.44M
	dd if=krnl16.bin of=floppy.img conv=notrunc
clean:
	rm -f *.o *.bin *.img *.com

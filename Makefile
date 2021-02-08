all: bs.bin Makefile floppy.img krnl16.com
start.o: start.asm Makefile
	nasm -f as86 start.asm -o start.o
cmain.o: cmain.c include/sys/types.h include/string.h include/stdio.h include/stdlib.h Makefile
	bcc -0 -I -Iinclude -c cmain.c -o cmain.o -ansi
bs.bin: bs.asm Makefile
	nasm -f bin bs.asm -o bs.bin
libmem.o: libmem.c include/sys/types.h include/stdio.h include/stdlib.h include/string.h Makefile
	bcc -0 -I -Iinclude -c libmem.c -o libmem.o -ansi
string.o: string.c include/string.h Makefile
	bcc -0 -I -Iinclude -c string.c -o string.o -ansi
processimage.o: processimage.asm Makefile
	nasm -f as86 processimage.asm -o processimage.o
krnl16.com:start.o cmain.o string.o libmem.o processimage.o Makefile
	ld86 -T0x7e00  -L -d start.o cmain.o string.o libmem.o processimage.o -o krnl16.com
floppy.img: Makefile bs.bin krnl16.com
	cat bs.bin krnl16.com > krnl16.bin
	qemu-img create floppy.img 1.44M
	dd if=krnl16.bin of=floppy.img conv=notrunc
clean:
	rm -f *.o *.bin *.img *.com

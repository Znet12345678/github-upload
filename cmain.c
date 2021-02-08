#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//cmain
int cmain(){
	char *test = smalloc(*((unsigned int *)pent + 1),1024);
	puts("zDOS v0-ALPHA bootstrap complete\r\n");
	while(1);
}
/*
 * put hex
 */
int putx(unsigned int n){
	int i = (sizeof(n))*8-4;
	while(i >= 0){
		unsigned char val = (n >> i) & 0xf;
		unsigned char rep = val < 0xa ? val + '0' : val + 'A' - 0xa;
		putc(rep);
		i-=4;
	}
}
/*
 * panic call
 */
void panic(msg){
	puts(msg);
	puts("\r\npanic(...)\r\n");
	while(1);
}

#ifndef __STDLIB_H
#define __STDLIB_H
#include <sys/types.h>
void *smalloc(unsigned int segment,size_t n);
void sfree(unsigned int segment,void *pntr);
void *realloc(void *pntr,size_t n);
struct freemkr{
	void *nxtfree;
	void *prevfree;
	size_t size;
};

struct ProcessImage{
	void *offset;
	void *seg;
};
extern struct ProcessImage *pent;
#endif

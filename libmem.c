#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
struct  freemkr*firstfree = 0;
unsigned char outOfMem = 0;
/*
 * Byte Zero- zeros n bytes of pntr
 */
void bzero(void *pntr,size_t n){
	size_t i = 0;
	while(i < n){
		*((char*)pntr + i) = 0;
		i++;
	}
}
/*
 * gets current data segment
 */
unsigned int dsGet(){
#asm
	mov ax,ds
#endasm
}
/*
 * sets ds to *segment* and backs up the current ds to es
 */
void dsSetup2(unsigned int segment){
#asm
	mov ax,ds
	mov es,ax
	mov ax,[bp+4]
	mov ds,ax
#endasm
}
/*
 * Segment Malloc
 * allocates memory in the heap specified by segment
 */
void *smalloc(unsigned int segment,size_t n){
	struct freemkr *freepntr;
	dsSetup2(segment);
	/*
	 * Checks to see if firstfree is set if not
	 * sets it to the first heap value of the process image
	 * initializes nxtfree and prevfree to zero
	 * sets the size equal to 0xffff-the base of the heap
	 */
	if(!firstfree){
		firstfree = (struct freemkr*)*(unsigned int *)pent;
		firstfree->nxtfree = 0;
		firstfree->prevfree = 0;
		firstfree->size = 0xffff-*(unsigned int *)pent;
	}
	freepntr = firstfree;
	if(outOfMem)
		panic("Out of memory in segment");
	while(1){
		if(freepntr->size >= n){//We have enough memory in this free zone to return this address
			void *ret = freepntr+sizeof(unsigned int);
			size_t size = freepntr->size-n;//size of left over memory after we allocate
			struct freemkr *nxtfree = freepntr->nxtfree;
			struct freemkr *prevfree = freepntr->prevfree;
			*(unsigned int *)freepntr = n;
			bzero(ret,n);//initialize the memory to zero bytes
			if(size > sizeof(*freepntr)+sizeof(unsigned int)){
				/*
				 * We have enough left over memory to start a new free area
				 */
				struct freemkr *newfree = (char*)ret + sizeof(unsigned int) + n; //Puts the new free marker after the allocated chunk and the size word
				newfree->prevfree = prevfree;
				newfree->nxtfree = nxtfree;
				newfree->size = size;
				if(prevfree)
					prevfree->nxtfree = newfree;
				if(nxtfree)
					nxtfree->prevfree = newfree;
				if(ret == firstfree)
					firstfree = newfree;
			}else{
				/*
				 * We don't have enough memory left in this free area
				 */
				if(prevfree)
					prevfree->nxtfree = nxtfree;
				if(nxtfree){
					nxtfree->prevfree = prevfree;
					if(firstfree == ret)
						firstfree = nxtfree;
				}else if(firstfree == ret){
					/*
					 * We don't have enough free memory in the segment for another
					 * smalloc call
					 */
					outOfMem = 1;
				}
			}
			dsRetire();//reset ds
			return ret;
		}else{/*This free area is too small*/
			struct freepntr *orig = freepntr;
			freepntr=freepntr->nxtfree;
			if(freepntr < orig){
				//Overflow check
				dsRetire();
				puts("Segment #");
				putx(segment);
				puts(":");
				panic("Out of memory in segment");
			}
		} 
	}
	dsRetire();//resets ds
}
/*
 * Frees pntr in segment
 */
void sfree(unsigned int segment,void *pntr){
	struct freemkr *freepntr;
	struct freemkr *low,*high;
	struct freemkr *newfree;
	dsSetup2(segment);//Sets up ds
	if(!firstfree){//We tried to free before malloc was able to setup firstfree
		dsRetire();
		panic("free before malloc");
	}
	freepntr = firstfree;
	if(pntr < freepntr){//Our pointer is before the first freepntr
		freepntr->prevfree = pntr;
		newfree = pntr;
		newfree->size = *(unsigned int *)((char *)pntr - sizeof(unsigned int));
		newfree->prevfree = 0;
		newfree->nxtfree = freepntr;
		firstfree = newfree;
		dsRetire();
		return;
	}
	//Our pointer is after the first freepntr
	high = firstfree->nxtfree;
	while(high < freepntr){//Where is the next highest freepntr
		if(high->nxtfree == 0){
			/*
			 * Our pointer is the highest free area
			 */
			high->nxtfree = pntr;
			newfree = pntr;
			newfree->prevfree = high;
			newfree->nxtfree = 0;
			newfree->size = *(unsigned int *)((char *)pntr - sizeof(unsigned int));
			dsRetire();
			return;
		}
		high = high->nxtfree;

	}
	/*
	 * our pointer is somewhere between firstfree and the highest freepntr
	 */
	low = high->prevfree;
	newfree = pntr;
	newfree->prevfree = low;
	newfree->nxtfree = high;
	newfree->size = *(unsigned int *)((char *)pntr - sizeof(unsigned int));
	dsRetire();
}

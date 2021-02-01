#ifndef __STRING_H
#define __STRING_H
#include <sys/types.h>
size_t strlen(const char *str);
int strcmp(const char *str1,const char *str2);
int strncmp(const char *str1,const char *str2,size_t n);
int memcmp(const void *pntr1,const void *pntr2,size_t n);
void *memcpy(void *dest,const void *src,size_t n);
char *strcpy(char *dest,const char *src);
char *strcat(char *dest,const char *src);
char *strncpy(char *dest,const char *src,size_t n);
#endif

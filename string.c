#include <string.h>
size_t strlen(str){
	size_t i = 0;
	while(*(char*)(str+i) != 0)
		i++;
	return i;
}
int strcmp(str1,str2){
	size_t i = 0;
	if(strlen(str1) != strlen(str2)){
		return strlen(str1)-strlen(str2);
	}
	while(i < strlen(str1)){
		if(((char*)str1)[i] != ((char*)str2)[i])
			return ((char*)str1)[i]-((char*)str2)[i];
		i++;
	}
	return 0;
}
unsigned long min(a,b){
	return a < b ? a : b;
}
int strncmp(str1,str2,n){
	size_t i = 0;
	while(i < n){
		if(((char*)str1)[i] != ((char*)str2)[i]){
			return (((char*)str1)[i]-((char*)str2)[i]);
		}
		if(((char*)str1)[i] == 0)
			break;
		i++;
	}
	return 0;
}
int memcmp(pntr1,pntr2,n){
	size_t i = 0;
	while(i < n){
		if(((char*)pntr1)[i] != ((char*)pntr2)[i])
			return ((char*)pntr1)[i]-((char*)pntr2)[i];
		i++;
	}
	return i;
}
void *memcpy(dest,src,n){
	size_t i = 0;
	while(i < n){
		*((char*)dest + i) = *((char*)src + i);
		i++;
	}
	return dest;
}
char *strcpy(dest,src){
	return memcpy(dest,src,strlen(src));
}
char *strcat(dest,src){
	return memcpy(dest + strlen(dest),src,strlen(src));
}
char *strncpy(dest,src,n){
	memcpy(dest,src,min(n,strlen(src)));
}

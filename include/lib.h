#ifndef LIB_H
#define LIB_H

#define rUARSTAT0 (*(volatile unsigned long *)0x50000010)
#define rUTXH0 (*(volatile unsigned long *)0x50000020)
void putc(char c);
int do_bootm_linux(void);
unsigned int strlen(const char * s);
char * strcpy(char * dest,const char *src);

#endif

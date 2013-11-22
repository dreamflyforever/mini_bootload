#include"lib.h"

unsigned int strlen(const char * s)
{
    const char *sc;

    for (sc = s; *sc != '\0'; ++sc)
	/* nothing */;
    return sc - s;
}

char * strcpy(char * dest,const char *src)
{
    char *tmp = dest;

    while ((*dest++ = *src++) != '\0')
	/* nothing */;
    return tmp;
}
void putc(char c)
{
  while (! ((rUARSTAT0)&(1 << 2)) ) ;
  rUTXH0 = c; 
}

#include"lib.h"
#include"nand.h"

//from 0x00200000 to 0x30008000, size = 0x00300000

void _main(void)
{
	unsigned char * p=(unsigned char *)0x30008000;
	ReadImage(p,0x00200000,0x00300000);
	do_bootm_linux();
}

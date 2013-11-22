#include"s3c2440.h"
#include"lib.h"
#include"nand.h"

/* 初始化NAND Flash */
void s3c2440_nand_init(void)
{
#define TACLS   1
#define TWRPH0  4
#define TWRPH1  0
	rNFCONF= (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
	/* 使能NAND Flash控制器, 初始化ECC, 禁止片选 */
	rNFCONT= (1<<4)|(1<<1)|(1<<0);
	//put('A');
	/* 复位NAND Flash */
	s3c2440_nand_reset();
	//put('A');
}

/* 读函数
 * Large Page
 */
void nand_read_ll(unsigned char *buf, unsigned long start_addr, int size)
{
	int i, j;
	/* 选中芯片 */
	s3c2440_nand_select_chip();
	for(i=start_addr; i < (start_addr + size);) {
		s3c2440_nand_clean_RB();
		/* 发出READ0命令 */
		s3c2440_write_cmd(0);
		/* Write Address */
		s3c2440_write_addr(i);
		s3c2440_write_cmd(0x30);
		s3c2440_wait_idle();
		for(j=0; j < NAND_SECTOR_SIZE_LP; j++ , i++) {
			*buf = s3c2440_read_data();
			buf++;
			//putc(s3c2440_read_data());
		}
	}
	/* 取消片选信号 */
	s3c2440_nand_deselect_chip();
	return ;
}

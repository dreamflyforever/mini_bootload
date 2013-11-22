#include"s3c2440.h"
#include"lib.h"
#include"nand.h"

/***********************************************
  检测坏块函数
 ***********************************************/
int s3c2440_NandIsBadBlock(unsigned long addr)
{
	unsigned long sector = addr >> 11;
	unsigned char bad_value;
	volatile unsigned char *p = (volatile unsigned char *)&rNFADDR;

	s3c2440_nand_reset();
	s3c2440_nand_select_chip();
	s3c2440_nand_clean_RB();
	s3c2440_write_cmd(0);
	*p=(2048 & 0xFF);
	*p=((2048 >>8) & 0xff);
	*p=(sector & 0xff);
	*p=((sector >> 8) & 0xff);
	*p=((sector >> 16) & 0xff);
	s3c2440_write_cmd(0x30);

	s3c2440_wait_idle();

	bad_value =s3c2440_read_data();


	if (bad_value == 0xFF) {
		return 0; 							//好的块
	}

	return 1; 							//坏的块
}

/********************************************************
  第二阶段拷贝
 ********************************************************/
void ReadImage(unsigned char *buf, unsigned long start_addr, int size)
{
	int i, j;
	/* 选中芯片 */
	s3c2440_nand_select_chip();
	for(i=start_addr; i < (start_addr + size);) {

		while(s3c2440_NandIsBadBlock(i))
		{ 											//坏的块
			size+=2048;
			i+=2048;
		}
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







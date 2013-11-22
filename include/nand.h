#ifndef NAND_D
#define NAND_D
//=================
// Nand Flash
//=================
#define	NFCONF			0x4E000000	//NAND Flash configuration
#define	NFCONT			0x4E000004      //NAND Flash control
#define	NFCMD			0x4E000008      //NAND Flash command
#define	NFADDR			0x4E00000C      //NAND Flash address
#define	NFDATA			0x4E000010      //NAND Flash data
#define	NFDATA8			0x4E000010      //NAND Flash data
#define	NFMECCD0		0x4E000014      //NAND Flash ECC for Main Area
#define	NFMECCD1		0x4E000018
#define	NFSECCD			0x4E00001C	//NAND Flash ECC for Spare Area
#define	NFSTAT			0x4E000020	//NAND Flash operation status
#define	NFESTAT0		0x4E000024
#define	NFESTAT1		0x4E000028
#define	NFMECC0			0x4E00002C
#define	NFMECC1			0x4E000030
#define	NFSECC			0x4E000034
#define	NFSBLK			0x4E000038	//NAND Flash Start block address
#define	NFEBLK			0x4E00003C	//NAND Flash End block address


#define rNFCONF		(*(volatile unsigned int *)NFCONF)
#define rNFCONT		(*(volatile unsigned int *)NFCONT)
#define rNFCMD		(*(volatile unsigned int *)NFCMD)
#define rNFADDR		(*(volatile unsigned int *)NFADDR)
#define rNFDATA		(*(volatile unsigned int *)NFDATA)
#define rNFSTAT		(*(volatile unsigned int *)NFSTAT)
#define NAND_SECTOR_SIZE_LP 2048
static inline void s3c2440_nand_reset(void);
static inline unsigned char s3c2440_read_data(void);
static inline void s3c2440_nand_clean_RB(void);
static inline void s3c2440_wait_idle(void);
static inline unsigned char s3c2440_read_data(void);
static inline void s3c2440_nand_select_chip(void);
static inline void s3c2440_nand_deselect_chip(void);
static inline void s3c2440_write_cmd(int cmd);
static inline void s3c2440_write_addr(unsigned int addr);

void s3c2440_nand_init(void);
void nand_read_ll(unsigned char *buf, unsigned long start_addr, int size);
void ReadImage(unsigned char *buf, unsigned long start_addr, int size);
int s3c2440_NandIsBadBlock(unsigned long addr);

/* S3C2440的NAND Flash操作函数 */
/* 复位 */
static inline void s3c2440_nand_reset(void)
{
	s3c2440_nand_select_chip();
	s3c2440_nand_clean_RB();
	s3c2440_write_cmd(0xff);  // 复位命令
	//put('A');
	s3c2440_wait_idle();
	//put('A');
	s3c2440_nand_deselect_chip();
}
/* 发出片选信号 */
static inline void s3c2440_nand_select_chip(void)
{
	int i;
	rNFCONT &= (~(1<<1));
	for(i=0; i<10; i++);   
}
/* 取消片选信号 */
static inline void s3c2440_nand_deselect_chip(void)
{
	rNFCONT |= (1<<1);
}
/* 发出命令 */
static inline void s3c2440_write_cmd(int cmd)
{
	volatile unsigned char *p = (volatile unsigned char *)&rNFCMD;
	//命令寄存器只能写低八位数据，其他位保留
	*p = cmd;
}
/* 发出地址 */
static inline void s3c2440_write_addr(unsigned int addr)
{
	int i;
	volatile unsigned char *p = (volatile unsigned char *)&rNFADDR;
	//写地址只能写低八位，其他位保留
	*p = addr & 0xff;
	for(i=0; i<10; i++);
	*p = (addr >> 8) & 0x07;
	for(i=0; i<10; i++);
	*p = (addr >> 11) & 0xff;
	for(i=0; i<10; i++);
	*p = (addr >> 19) & 0xff;
	for(i=0; i<10; i++);
	*p = (addr >> 27) & 0x07;
	for(i=0; i<10; i++);
}
/* 读取数据 */
static inline unsigned char s3c2440_read_data(void)
{
	volatile unsigned char *p = (volatile unsigned char *)&rNFDATA;
	//该数据寄存器是一个FIFO来的，能读八位，十六位，和三十二位
	return *p & (0xFF);
}

/*
 *RnB清零
 */
static inline void s3c2440_nand_clean_RB(void)
{
	rNFSTAT|=(1<<2);
}


/*
 *等待NAND Flash就绪 
 */
static inline void s3c2440_wait_idle(void)
{
	int i;
	while(!(rNFSTAT & (1<<2)))
		for(i=0; i<10; i++);
}


#endif

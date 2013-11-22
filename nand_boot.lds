/*
*/
SECTIONS {
	. = 0x33F80000;
	Image_RO_Base = . ;
  	.text : { 	
			Main/start.o(.text)
			lib/Nand/nand_boot.o(.text)
			*.o(.text)
		}
	.rodata ALIGN(4) : {*(.rodata)} 
	Image_RO_Limit = . ;
	Image_RW_Base = . ;
  	.data ALIGN(4) : { *(.data) }
	Image_RW_Limit = . ;
	Image_ZI_Base = . ;
  	.bss ALIGN(4)  : { *(.bss)  *(COMMON) }
	Image_ZI_Limit = . ;
}

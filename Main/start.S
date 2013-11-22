#include"s3c2440.h"
// Start of executable code

.globl _start
_start:

//
// Exception vector table (physical address = 0x00000000)
//

// 0x00: Reset
b Reset
// 0x04: Undefined instruction exception
UndefEntryPoint:
b HandleUndef
// 0x08: Software interrupt exception
SWIEntryPoint:
b HandleSWI
// 0x0c: Prefetch Abort (Instruction Fetch Memory Abort)
PrefetchAbortEnteryPoint:
b HandlePrefetchAbort
// 0x10: Data Access Memory Abort
DataAbortEntryPoint:
b HandleDataAbort
// 0x14: Not used
NotUsedEntryPoint:
b HandleNotUsed
// 0x18: IRQ(Interrupt Request) exception
IRQEntryPoint:
b HandleIRQ
// 0x1c: FIQ(Fast Interrupt Request) exception
FIQEntryPoint:
b HandleFIQ
//设置cpu进入SVC模式
Reset:
mrs	r0, cpsr      
bic	r0, r0, #0x1f
orr	r0, r0, #0xd3
msr	cpsr, r0
// disable watch dog timer
mov r1, #WTCON
mov r2, #0x0
str r2, [r1]
// disable all interrupts
ldr r1, =INTMSK
mov r2, #0xffffffff
str r2, [r1]
ldr r1, =INTSUBMSK
ldr r2, =0x7ff
str r2, [r1]
// initialise system clocks
mov r1, #LOCKTIME
mvn r2, #0xff000000
str r2, [r1]

ldr r1, =CLKDIVN
mov r2, #5
str r2, [r1]
//设置异步总线模式,Asynchronous模式
mrc p15, 0, r1, c1, c0, 0	// read ctrl register
orr r1, r1, #0xc0000000		// Asynchronous
mcr p15, 0, r1, c1, c0, 0	// write ctrl register
//Configure UPLL
	ldr	r0,=UPLLCON
ldr	r1,=((56<<12)+(2<<4)+2)
	str	r1,[r0]
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	//Configure MPLL
	ldr	r0,=MPLLCON
	ldr	r1,=0x7f021	//=((92<<12)+(1<<4)+1)	
	str	r1,[r0]


	/*
	 * flush v4 I/D caches
	 */
	mov	r0, #0
	mcr	p15, 0, r0, c7, c7, 0	/* flush v3/v4 cache Ë¢ÐÂcache*/
	mcr	p15, 0, r0, c8, c7, 0	/* flush v4 TLB Ë¢ÐÂTLB*/

	/*
	 * disable MMU stuff and cache
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002300	//@ clear bits 13, 9:8 (--V- --RS)
	bic	r0, r0, #0x00000087	//@ clear bits 7, 2:0 (B--- -CAM)
	orr	r0, r0, #0x00000002	//@ set bit 2 (A) Align
	orr	r0, r0, #0x00001000	//@ set bit 12 (I) I-Cache
	mcr	p15, 0, r0, c1, c0, 0

	bl memsetup
	bl InitUART
	bl copy_myself

	// jump to ram
	ldr pc, =on_the_ram
	nop
	nop
	1: b 1b // infinite loop

	on_the_ram:

	// get read to call C functions
	ldr sp, DW_STACK_START // setup stack pointer
	mov fp, #0 // no previous frame, so fp=0
	mov a2, #0 // set argv to NULL

	bl _main // call main

	mov pc, #0x00000000 // otherwise, reboot


	// initialise the static memory
	memsetup:

	// set memory control registers
	mov r1, #BWSCON
	adrl r2, mem_cfg_val
	add r3, r1, #52

	1: ldr r4, [r2], #4
	str r4, [r1], #4
	cmp r1, r3
	bne 1b
	mov pc, lr

	//
	// copy_myself: copy vivi to ram
	//
	copy_myself:
	mov r10, lr

	// get read to call C functions (for nand_read())
	ldr sp, DW_STACK_START // setup stack pointer
	mov fp, #0 // no previous frame, so fp=0

	bl s3c2440_nand_init

	// copy vivi to RAM
	ldr r0,BootLoaderAddr
	mov r1, #0x0
	mov r2, #0x800
	bl nand_read_ll

	ok_nand_read:

	mov pc, r10

	//只有这里没有改成宏定义的寄存器名
	// Initialize UART
	//
	// r0 = number of UART port
	InitUART:

	// set GPIO for UART
	mov r1, #0x56000000
	add r1, r1, #0x70
	ldr r2, gpio_con_uart
	str r2, [r1, #0x0]
	ldr r2, gpio_up_uart
	str r2, [r1, #0x8]

	ldr r1, SerBase
	mov r2, #0x0
	str r2, [r1, #0x08]
	str r2, [r1, #0x0C]
	mov r2, #0x3
	str r2, [r1, #0x00]
	ldr r2, =0x245
	str r2, [r1, #0x04]

	mov r2, #((50625000 / (115200 * 16)) - 1)
	str r2, [r1, #0x28]

	mov r3, #100
	mov r2, #0x0
	1: sub r3, r3, #0x1
	tst r2, r3
	bne 1b

	mov pc, lr


	//
	// Exception handling functions
	//
	HandleUndef:
1: b 1b // infinite loop
HandleSWI:
1: b 1b // infinite loop
HandlePrefetchAbort:
1: b 1b // infinite loop
HandleDataAbort:
1: b 1b // infinite loop
HandleIRQ:
1: b 1b // infinite loop
HandleFIQ:
1: b 1b // infinite loop
HandleNotUsed:
1: b 1b // infinite loop


//
// Data Area
//
// Memory configuration values
.align 4
mem_cfg_val:
.long 0x22111110
.long 0x00000700
.long 0x00000700
.long 0x00000700
.long 0x00000700
.long 0x00000700
.long 0x00000700
.long 0x00018009
.long 0x00018009
.long 0x00aC03F4
.long 0xB2
.long 0x30
.long 0x30


// Processor clock values
.align 4

// inital values for GPIO
gpio_con_uart:
.long 0x0016faaa
gpio_up_uart:
.long 0x000007ff

.align 2
DW_STACK_START:
.word 0x33ff0000

.align 4
SerBase:
.long 0x50000000

BootLoaderAddr:
.long Image_RO_Base 

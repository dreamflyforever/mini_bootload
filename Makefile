#到每个模块去收集这四个变量的信息
#初始设定这四个变量为简单变量
programs 	:= 
sources 	:= 
libraries 	:= 
extra_claen 	:= 

objects 	= $(subst .S,.o,$(subst .c,.o,$(sources)))

include_dir := include
CFLAGS		:= -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer -ffreestanding
CFLAGS 		+= $(addprefix -I,$(include_dir))
vpath %.h 	$(include_dir)

RM 		:= rm -f
CC 		:= arm-linux-gcc
LD		:= arm-linux-ld
OBJCOPY		:= arm-linux-objcopy

all:

modules		:= $(subst /module.mk,,$(shell find . -name module.mk))
include $(addsuffix /module.mk,$(modules))
include config.mk

.PHONY: all
all 	: $(programs)


.PHONY: clean
clean 	:
	$(RM) $(objects) $(programs) $(libraries) $(extra_claen)



%.o: %.S
	$(CC) -c $(CFLAGS) -o $@ $<
%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

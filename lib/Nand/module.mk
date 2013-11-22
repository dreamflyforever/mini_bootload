#四个变量类似于局部变量
local_dir 	:= lib/Nand
NAND_BOOT	:= $(local_dir)/nand_boot.o
local_lib 	:= $(local_dir)/Nand.o
local_src 	:= $(wildcard $(local_dir)/*.c) $(wildcard $(local_dir)/*.S)
local_objs 	:= $(subst $(NAND_BOOT),,$(subst .S,.o,$(subst .c,.o,$(local_src))))

libraries 	+= $(local_lib) $(NAND_BOOT)
sources 	+= $(local_src)
extra_claen	+= $(local_dir)/*~

$(local_lib):  $(local_objs)
	$(LD) 	-r -o $@ $^

TGT 		:= nand_boot_elf
TAT		:= nand_boot.bin
extra_claen	+= *~
programs 	+= $(TAT) $(TGT)
$(TAT):		$(TGT)
	$(OBJCOPY)  -O binary -S $< $@
$(TGT):		$(libraries)
	$(LD) -Tnand_boot.lds -o $@ $^

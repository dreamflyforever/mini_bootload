#四个变量类似于局部变量
local_dir 	:= lib/String
local_lib 	:= $(local_dir)/String.o
local_src 	:= $(wildcard $(local_dir)/*.c) $(wildcard $(local_dir)/*.S)
local_objs 	:= $(subst .S,.o,$(subst .c,.o,$(local_src)))

libraries 	+= $(local_lib)
sources 	+= $(local_src)
extra_claen	+= $(local_dir)/*~

$(local_lib):  $(local_objs)
	$(LD) 	-r -o $@ $^

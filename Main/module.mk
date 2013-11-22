#四个变量类似于局部变量
local_dir 	:= Main
START		:= $(local_dir)/start.o
local_lib 	:= $(local_dir)/Main.o
local_src 	:= $(wildcard $(local_dir)/*.c) $(wildcard $(local_dir)/*.S)
local_objs 	:= $(subst $(START),,$(subst .S,.o,$(subst .c,.o,$(local_src))))

libraries 	+= $(local_lib) $(START)
sources 	+= $(local_src)
extra_claen	+= $(local_dir)/*~

$(local_lib):  $(local_objs)
	$(LD) 	-r -o $@ $^

WARNINGS := -Wno-long-long -Wall -Wswitch-enum -pedantic -Werror
CXXFLAGS := -pthread -std=gnu++98 $(WARNINGS)
VERBOSE := -v # add into CXXFLAGS
NACL_SDK_ROOT=/home/yjy/project/nacl_root/native_client
NACL_TC_PATH :=${NACL_SDK_ROOT}/toolchain/linux_x86/nacl_x86_newlib
NACL_CXX := $(NACL_TC_PATH)/bin/x86_64-nacl-g++
NACL_CC := $(NACL_TC_PATH)/bin/x86_64-nacl-gcc
NACL_GDB := $(NACL_TC_PATH)/bin/x86_64-nacl-gdb
NACL_OBJDUMP:=$(NACL_TC_PATH)/bin/x86_64-nacl-objdump
CFLAGS := -std=c99 -nostdlib -I$(NACL_SDK_ROOT)/scons-out/nacl-x86-64/include -I$(NACL_SDK_ROOT)/../ 
CXXFLAGS :=$(CXXFLAGS) $(CFLAGS)
LDFLAGS :=  -L$(NACL_SDK_ROOT)/scons-out/nacl-x86-64/lib/ 
# -Bdynamic -fPIC for dynamic load 
RUN :=${NACL_SDK_ROOT}/run.py
NAME :=hello
PLATFORM :=x86-64
all: $(NAME).nexe
clean:
	rm -f *.pexe *.nexe *.bc *.S *.o
$(NAME).nexe:
	$(NACL_CC) -static  -o $@ -g -O0 $(CFLAGS) $(LDFLAGS) $(NAME).c
	cp $@ ../../host/
$(NAME).$(PLATFORM).nexe: $(NAME).nexe
	cp $< $(NAME).$(PLATFORM).nexe 
analysis: $(NAME).$(PLATFORM).nexe
	file $<
	readelf -l $<
	$(NACL_OBJDUMP) -d $< > $(NAME).S

run: $(NAME).nexe
	python $(RUN) $< 
gdb: $(NAME).nexe
	python $(RUN) --debug $< 	

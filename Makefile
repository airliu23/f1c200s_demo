#
# SPDX-License-Identifier:	GPL-2.0+
#

ARCH ?= arm
CROSS_COMPILE ?= ./tools/toolchain/bin/arm-none-eabi-

CC 		:= $(CROSS_COMPILE)gcc
AS 		:= $(CC) -x assembler-with-cpp
LD 		:= $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump

#############################################################
# -Wall   : 生成所有的警告信息
# -static : 禁止使用共享链接
# -march  : 针对某架构优化
# -mthumb : 支持thumb指令  ---> 加上该指令软件运行失败，不确定原因
# -c      : 只编译，不链接
# -g      : 生成调试信息，gdb可用
# -O	  : gcc优化等级
# -I      : 索引的头文件地址
# -l	  : 链接时搜索指定的库文件
# -L	  : 链接时搜索制定的文件夹
#############################################################

INC_FLAGS := -Iinclude -Isrc/driver/
CCFLAGS  := -march=armv5te -c -O0 -g0 -Wall $(INC_FLAGS)
ASMFLAGS := -c -O0 -g0 -march=armv5te -Wall

LDFLAGS :=


# All source file
SRC_PATH := src src/driver lib
C_SRC := $(foreach dir,$(SRC_PATH),$(wildcard $(dir)/*.c))
ASM_SRC := $(foreach dir,$(SRC_PATH),$(wildcard $(dir)/*.S))



# create object
OBJ := $(foreach f,$(ASM_SRC),$(patsubst %S,%o,$(f))) $(foreach f,$(C_SRC),$(patsubst %c,%o,$(f)))
OBJS := $(sort $(OBJ))

# output
OUTPUT := f1c200s


HOSTCC := gcc

MKBOOT_SRC := ./tools/mksunxiboot.c
MKBOOT 	   := ./tools/mksunxiboot

all:$(OUTPUT)_SD.bin FORCE

# f1c200s_SD.bin
$(OUTPUT)_SD.bin:$(OUTPUT).bin tools
	$(MKBOOT) $(OUTPUT).bin $@

# f1c200s.bin
$(OUTPUT).bin:$(OUTPUT).elf
	$(OBJCOPY) -O binary $^ $@

# f1c200s.elf
$(OUTPUT).elf:$(OBJS)
	$(LD) $(LDFLAGS) -T link.ld $(OBJS) -Map $(OUTPUT).map -o $@
	rm -rf $^

%.o:%.c
	$(CC) $(CCFLAGS) $^ -o $@

%.o:%.S
	$(AS) $(ASMFLAGS) $^ -o $@


$(MKBOOT):$(MKBOOT_SRC)
	$(HOSTCC) $^ -o $@

objdump:$(OUTPUT).elf
	@$(OBJDUMP) -S $^

download:
	@sudo dd if=$(OUTPUT)_SD.bin of=/dev/sda bs=1024 seek=8 conv=fdatasync 

tools:$(MKBOOT)
	@echo $(C_SRC)
	@echo $(ASM_SRC)
	@echo $(OBJS)

clean:FORCE
	rm $(OBJS) $(OUTPUT)*

PHONY += FORCE
FORCE:





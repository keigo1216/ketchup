# Clangのパス
CC := $(or $(CLANG_PATH), /usr/bin/clang)
OBJDUMP := $(or $(OBJDUMP_PATH), /usr/bin/llvm-objdump)
OBJCOPY := $(or $(OBJCOPY_PATH), /usr/bin/llvm-objcopy)
NM := $(or $(NM_PATH),/usr/bin/llvm-nm)

# クロスコンパイルのターゲットアーキテクチャ（OSなし）
TARGET_ARCHITECTUR := aarch64-none-elf
TARGET_MATCHNE := armv8-a

# ソースファイル
KERNEL_SRC_DIR := kernel
COMMON_SRC_DIR := common
USER_SRC_DIR := user
KERNEL_SRCS := $(wildcard $(addsuffix /*.c, $(KERNEL_SRC_DIR) $(COMMON_SRC_DIR)))
KERNEL_ASM := $(wildcard $(addsuffix /*.S, $(KERNEL_SRC_DIR) $(COMMON_SRC_DIR)))
USER_SRCS := $(wildcard $(addsuffix /*.c, $(USER_SRC_DIR) $(COMMON_SRC_DIR)))
KERNEL_SRC := kernel/kernel.c
KERNEL_OBJS := $(KERNEL_SRCS:.c=.o)
USER_OBJS := $(USER_SRCS:.c=.o)
INCLUDES := $(foreach d, $(KERNEL_SRC_DIR) $(COMMON_SRC_DIR) $(USER_SRC_DIR), -I$d)

# コンパイルオプション
CFLAGS := -std=c11 -Wall -O0 -g3 --target=$(TARGET_ARCHITECTUR) -march=$(TARGET_MATCHNE) -ffreestanding -nostdlib $(INCLUDES)

# 出力ファイル（実行可能バイナリ）
KERNEL := kernel.elf
USER := shell

# デバイスツリーブロブのパス
FW_DIR := ./bcm2710-rpi-3-b-plus.dtb

# QEMUの実行
QEMU := qemu-system-aarch64
MEMORY := 512
MATHINE := raspi3ap

CPU := cortex-a53

# QEMUの実行オプション
QEMU_OPTS := -M $(MATHINE) -cpu $(CPU) -m $(MEMORY) -nographic -kernel $(KERNEL) -dtb $(FW_DIR) -D qemu.log -serial mon:stdio --no-reboot -smp 4 -d in_asm

# デフォルトターゲット
all: clean $(KERNEL) dump nm

%o: %c
	$(CC) $(CFLAGS) -c $< -o $@

# カーネルのビルド
$(KERNEL): $(KERNEL_OBJS) $(KERNEL_ASM) $(USER).bin.o
	$(CC) $(CFLAGS) -Wl,-Tkernel/kernel.ld -Wl,-Map=kernel.map -o $@ $^

# ユーザーアプリケーションのビルド
$(USER).elf: $(USER_OBJS)
	$(CC) $(CFLAGS) -Wl,-Tuser/user.ld -Wl,-Map=user.map -o $@ $^

# shell.elf => shell.binへの変換 (生バイナリ)
$(USER).bin: $(USER).elf
	$(OBJCOPY) --set-section-flags .bss=alloc,contents -O binary $< $@

# shell.bin => shell.bin.oへの変換 (ELF)
$(USER).bin.o: $(USER).bin
	$(OBJCOPY) -Ibinary -Oelf64-littleaarch64 $< $@

# dump
dump: $(KERNEL)
	$(OBJDUMP) -d kernel.elf >> kernel.dump
	$(OBJDUMP) -d $(USER).elf >> $(USER).dump

nm: $(KERNEL)
	$(NM) kernel.elf >> kernel.nm
	$(NM) $(USER).bin.o >> $(USER).nm

# github actionsでのテスト実行
test: $(KERNEL_OBJS) $(KERNEL_ASM) $(USER).bin.o
	$(CC) $(CFLAGS) -Wl,-Tkernel/kernel.ld -Wl,-Map=kernel.map -o $@ $^

# QEMUでのテスト実行
run: $(KERNEL)
	$(QEMU) $(QEMU_OPTS)

# クリーンアップ
clean:
	rm -f $(KERNEL_OBJS) $(USER_OBJS) $(KERNEL) $(USER).elf $(USER).bin $(USER).bin.o kernel.dump *.map *.nm *.dump

.PHONY: all run clean
# Clangのパス
CLANG := /usr/local/opt/llvm/bin/clang
OBJDUMP=/usr/local/opt/llvm/bin/llvm-objdump

# クロスコンパイルのターゲットアーキテクチャ（OSなし）
TARGET_ARCH := aarch64-none-elf
TARGET_MATCHNE := armv8-a

# ソースファイル
SRC_DIR := kernel common
SRCS := $(wildcard $(addsuffix /*.c, $(SRC_DIR)))
ASM := $(wildcard $(addsuffix /*.S, $(SRC_DIR)))
SRC := kernel/kernel.c
OBJS := $(SRCS:.c=.o)
INCLUDES := $(foreach d, $(SRC_DIR), -I$d)

# コンパイルオプション
CFLAGS := -std=c11 -Wall -O2 -g3 --target=$(TARGET_ARCH) -march=$(TARGET_MATCHNE) -ffreestanding -nostdlib $(INCLUDES)

# 出力ファイル（実行可能バイナリ）
KERNEL := kernel.elf

# デバイスツリーブロブのパス
FW_DIR := ./bcm2710-rpi-3-b-plus.dtb

# QEMUの実行
QEMU := qemu-system-aarch64
MEMORY := 512
MATHINE := raspi3ap

CPU := cortex-a53

# QEMUの実行オプション
QEMU_OPTS := -M $(MATHINE) -cpu $(CPU) -m $(MEMORY) -nographic -kernel $(KERNEL) -dtb $(FW_DIR) -D qemu.log -serial mon:stdio --no-reboot -smp 4

# デフォルトターゲット
all: clean $(KERNEL) dump

%o: %c
	$(CLANG) $(CFLAGS) -c $< -o $@

# ターゲットのビルド
$(KERNEL): $(OBJS) $(ASM)
	$(CLANG) $(CFLAGS) -Wl,-Tkernel/kernel.ld -Wl,-Map=kernel.map -o $@ $^
# dump
dump: $(KERNEL)
	$(OBJDUMP) -d kernel.elf >> kernel.dump

# github actionsでのテスト実行
test: $(SRCS) $(ASM)
	clang $(CFLAGS) -Wl,-Tkernel/kernel.ld -Wl,-Map=kernel.map -o kernel.elf $^

# QEMUでのテスト実行
run: $(KERNEL)
	$(QEMU) $(QEMU_OPTS)

# クリーンアップ
clean:
	rm -f $(OBJS) $(KERNEL) kernel.dump

.PHONY: all run clean

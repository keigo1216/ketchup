# Clangのパス
CLANG := /usr/local/opt/llvm/bin/clang

# クロスコンパイルのターゲットアーキテクチャ（OSなし）
TARGET_ARCH := aarch64-none-elf

# コンパイルオプション
CFLAGS := -Wall -O2 --target=$(TARGET_ARCH) -ffreestanding -nostdinc -nostdlib

# ソースファイル
SRC := kernel.c

OBJ := $(SRC:.c=.o)

# 出力ファイル（実行可能バイナリ）
KERNEL := kernel.elf

# デバイスツリーブロブのパス
FW_DIR := ./bcm2710-rpi-3-b-plus.dtb

# QEMUの実行
QEMU := qemu-system-aarch64
MEMORY := 1024

CPU := cortex-a53

# QEMUの実行オプション
QEMU_OPTS := -M virt -cpu $(CPU) -m $(MEMORY) -nographic -kernel $(KERNEL) -dtb $(FW_DIR)

# デフォルトターゲット
all: $(KERNEL)

# ターゲットのビルド
$(KERNEL): $(SRC)
	$(CLANG) $(CFLAGS) -Wl,-Tkernel.ld -o kernel.elf kernel.c

# QEMUでのテスト実行
run: $(KERNEL)
	$(QEMU) $(QEMU_OPTS)

# クリーンアップ
clean:
	rm -f $(OBJ) $(KERNEL)

.PHONY: all run clean

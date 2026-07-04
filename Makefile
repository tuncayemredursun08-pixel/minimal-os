.PHONY: all clean run iso

CC = gcc
LD = ld
NASM = nasm
QEMU = qemu-system-x86_64
GRUB_MKRESCUE = grub-mkrescue

CFLAGS = -m64 -march=x86-64 -ffreestanding -fno-stack-protector -fno-builtin -Wall -Wextra -O2
LDFLAGS = -m elf_x86_64 -T linker.ld
NASMFLAGS = -f elf64

KERNEL_OBJ = kernel/boot.o kernel/kernel.o kernel/vga.o kernel/interrupts.o kernel/gdt.o
WM_OBJ = wm/wm.o wm/window.o wm/input.o wm/framebuffer.o
LIBC_OBJ = libc/string.o libc/memory.o
ALL_OBJ = $(KERNEL_OBJ) $(WM_OBJ) $(LIBC_OBJ)

KERNEL_BIN = kernel.bin
ISO_NAME = minimal-os.iso
BOOT_DIR = boot

all: $(KERNEL_BIN) iso

$(KERNEL_BIN): $(ALL_OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

kernel/boot.o: kernel/boot.asm
	$(NASM) $(NASMFLAGS) -o $@ $<

kernel/kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c -o $@ $<

kernel/vga.o: kernel/vga.c
	$(CC) $(CFLAGS) -c -o $@ $<

kernel/interrupts.o: kernel/interrupts.c
	$(CC) $(CFLAGS) -c -o $@ $<

kernel/gdt.o: kernel/gdt.asm
	$(NASM) $(NASMFFLAGS) -o $@ $<

wm/wm.o: wm/wm.c
	$(CC) $(CFLAGS) -c -o $@ $<

wm/window.o: wm/window.c
	$(CC) $(CFLAGS) -c -o $@ $<

wm/input.o: wm/input.c
	$(CC) $(CFLAGS) -c -o $@ $<

wm/framebuffer.o: wm/framebuffer.c
	$(CC) $(CFLAGS) -c -o $@ $<

libc/string.o: libc/string.c
	$(CC) $(CFLAGS) -c -o $@ $<

libc/memory.o: libc/memory.c
	$(CC) $(CFLAGS) -c -o $@ $<

iso: $(KERNEL_BIN)
	mkdir -p $(BOOT_DIR)/grub
	cp $(KERNEL_BIN) $(BOOT_DIR)/
	cp boot/grub.cfg $(BOOT_DIR)/grub/
	$(GRUB_MKRESCUE) -o $(ISO_NAME) $(BOOT_DIR)

run: iso
	$(QEMU) -cdrom $(ISO_NAME) -m 512M -serial stdio

run-debug: iso
	$(QEMU) -cdrom $(ISO_NAME) -m 512M -serial stdio -S -gdb tcp::1234

clean:
	rm -f $(ALL_OBJ) $(KERNEL_BIN) $(ISO_NAME)
	rm -rf $(BOOT_DIR)/grub $(BOOT_DIR)/kernel.bin

help:
	@echo "Available targets:"
	@echo "  make all        - Build kernel and ISO"
	@echo "  make iso        - Create bootable ISO"
	@echo "  make run        - Run in QEMU"
	@echo "  make run-debug  - Run in QEMU with GDB support"
	@echo "  make clean      - Clean build artifacts"

.PHONY: all clean run iso limine limine-build

CC := gcc
LD := ld
NASM := nasm
QEMU := qemu-system-x86_64
XORRISO := xorriso
LIMINE_DIR ?= limine

CFLAGS := -m64 -march=x86-64 -ffreestanding -fno-stack-protector -fno-builtin -Wall -Wextra -O2
LDFLAGS := -m elf_x86_64 -T linker.ld
NASMFLAGS := -f elf64

KERNEL_OBJ := kernel/boot.o kernel/kernel.o kernel/vga.o kernel/interrupts.o kernel/gdt.o
WM_OBJ := wm/wm.o wm/window.o wm/input.o wm/framebuffer.o
LIBC_OBJ := libc/string.o libc/memory.o
ALL_OBJ := $(KERNEL_OBJ) $(WM_OBJ) $(LIBC_OBJ)

KERNEL_BIN := kernel.bin
ISO_NAME := minimal-os.iso
ISO_ROOT := iso_root

all: iso

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
	$(NASM) $(NASMFLAGS) -o $@ $<

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

limine:
	@if [ ! -x "$(LIMINE_DIR)/limine" ]; then \
		echo "Limine tools not found in $(LIMINE_DIR). Clone/build Limine first:"; \
		echo "  git clone --branch v12.x https://github.com/Limine-Bootloader/Limine.git $(LIMINE_DIR)"; \
		echo "  cd $(LIMINE_DIR) && ./bootstrap && ./configure && make"; \
		exit 1; \
	fi

iso: $(KERNEL_BIN) limine
	rm -rf $(ISO_ROOT)
	mkdir -p $(ISO_ROOT)/boot
	cp $(KERNEL_BIN) $(ISO_ROOT)/boot/kernel.bin
	cp boot/limine.conf $(ISO_ROOT)/boot/limine.conf
	cp $(LIMINE_DIR)/limine-bios.sys $(ISO_ROOT)/boot/
	cp $(LIMINE_DIR)/limine-bios-cd.bin $(ISO_ROOT)/boot/
	cp $(LIMINE_DIR)/limine-uefi-cd.bin $(ISO_ROOT)/boot/
	mkdir -p $(ISO_ROOT)/EFI/BOOT
	cp $(LIMINE_DIR)/BOOTX64.EFI $(ISO_ROOT)/EFI/BOOT/
	$(XORRISO) -as mkisofs -R -r -J \
		-b boot/limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table \
		-hfsplus -apm-block-size 2048 --efi-boot boot/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(ISO_ROOT) -o $(ISO_NAME)
	$(LIMINE_DIR)/limine bios-install $(ISO_NAME)

run: iso
	$(QEMU) -cdrom $(ISO_NAME) -m 512M -serial stdio

run-debug: iso
	$(QEMU) -cdrom $(ISO_NAME) -m 512M -serial stdio -S -gdb tcp::1234

clean:
	rm -f $(ALL_OBJ) $(KERNEL_BIN) $(ISO_NAME)
	rm -rf $(ISO_ROOT)

help:
	@echo "Minimal OS build targets:"
	@echo "  make          Build kernel and Limine BIOS/UEFI ISO"
	@echo "  make run      Build and boot in QEMU"
	@echo "  make clean    Remove build artifacts"
	@echo "  make help     Show this help"

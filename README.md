# Minimal Unix-based OS with Custom Window Manager

A minimal operating system built from scratch with:
- Custom x86-64 bootloader (GRUB-based)
- Minimal kernel with interrupt handling
- Custom overlapping window manager
- Basic graphics support (VGA text mode + framebuffer)

## Architecture

- **Bootloader**: GRUB 2 (legacy multiboot)
- **Kernel**: Custom minimal kernel in C and Assembly
- **Display**: VGA text mode initially, framebuffer graphics for window manager
- **Window Manager**: Custom overlapping window manager with mouse/keyboard input

## Requirements

- `gcc` (cross-compiler for x86-64 preferred)
- `nasm` (Netwide Assembler)
- `ld` (GNU Linker)
- `qemu-system-x86_64` (for testing)
- `grub-mkrescue` (for creating bootable ISO)
- `xorriso` (ISO creation tool)

## Building

```bash
make clean
make
```

## Running

```bash
make run  # Boots the OS in QEMU
```

## Project Structure

- `boot/` — Bootloader and GRUB configuration
- `kernel/` — Core kernel code (entry point, interrupts, VGA driver)
- `wm/` — Window manager implementation
- `libc/` — Minimal C library functions
- `linker.ld` — Linker script for kernel layout

## Features

- [x] GRUB bootloader
- [x] Kernel entry point
- [x] Interrupt handling (IDT)
- [x] VGA text mode driver
- [x] Basic graphics framebuffer
- [x] Window manager foundation
- [x] Overlapping windows with depth
- [x] Mouse and keyboard input handling
- [ ] Filesystem support
- [ ] Process management
- [ ] Memory management (paging)

## Quick Start

```bash
qemu-system-x86_64 -cdrom minimal-os.iso -m 512M -serial stdio
```

## References

- https://wiki.osdev.org/
- https://www.gnu.org/software/grub/manual/
- Bare Metal x86-64 development

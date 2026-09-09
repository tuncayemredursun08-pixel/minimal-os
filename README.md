# Minimal OS

A from-scratch x86-64 hobby operating system written in C and Assembly.

## Boot stack

- **Bootloader:** Limine v12.x
- **Kernel:** custom x86-64 kernel
- **Display:** VGA text mode first, with framebuffer support in the existing window-manager code
- **Architecture:** x86-64
- **Testing:** QEMU

The project previously used GRUB. It now boots through Limine's Multiboot1 protocol. Limine's documentation supports `multiboot1` entries with a kernel path and documents BIOS/UEFI hybrid ISO creation. fileciteturn20file1 fileciteturn17file0

## Repository layout

```text
boot/
  limine.conf       Limine boot configuration
kernel/
  boot.asm          x86 bootstrap + long-mode setup
  kernel.c          kernel entry point
  vga.*             VGA text driver
  interrupts.*      interrupt handling
  gdt.asm           GDT setup
wm/                 experimental window manager
libc/               tiny freestanding C helpers
linker.ld           kernel linker script
Makefile            kernel + Limine ISO build
.github/workflows/  reproducible CI build
```

## Build on Linux

Install `gcc`, `binutils`, `nasm`, `xorriso`, and QEMU, then build Limine:

```bash
git clone --depth 1 --branch v12.x https://github.com/Limine-Bootloader/Limine.git limine
cd limine
./bootstrap
./configure
make -j2
cd ..
make LIMINE_DIR=limine
```

Run it:

```bash
make run LIMINE_DIR=limine
```

The resulting `minimal-os.iso` is a Limine BIOS/UEFI hybrid image. fileciteturn17file0

## GitHub Actions

Every push and pull request builds Limine, compiles the kernel, creates the ISO, performs a short QEMU smoke test, and uploads `minimal-os.iso` as the `minimal-os-iso` artifact.

## Current features

- [x] Limine boot configuration
- [x] x86-64 kernel entry
- [x] VGA text output
- [x] Interrupt/GDT foundations
- [x] Existing window-manager foundation
- [ ] Physical memory manager
- [ ] Paging/virtual memory manager
- [ ] Keyboard driver
- [ ] Filesystem
- [ ] Process/thread scheduler
- [ ] User mode
- [ ] Shell

## Next milestone

The next goal is a **real text-only VGA desktop/shell**: keyboard input, command parsing, scrolling, basic filesystem commands, and a small kernel allocator. Graphics can be added later without replacing the text console.

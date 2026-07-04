; Multiboot header and boot entry point

bits 32

; Multiboot header (must be 32-bit)
MULTIBOOT_MAGIC equ 0x1BADB002
MULTIBOOT_FLAGS equ 0x00000003  ; PAGE_ALIGN | MEMORY_INFO
MULTIBOOT_CHECKSUM equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

align 4

section .multiboot
    dd MULTIBOOT_MAGIC
    dd MULTIBOOT_FLAGS
    dd MULTIBOOT_CHECKSUM

section .text

; Entry point for the kernel
global start
extern kernel_main
extern gdt_load
extern idt_load

start:
    ; Save multiboot info
    mov edi, ebx        ; EBX contains multiboot info pointer

    ; Setup stack
    mov esp, stack_top
    mov ebp, esp

    ; Load 64-bit GDT
    call gdt_load

    ; Switch to 64-bit mode
    call setup_long_mode

    ; Jump to 64-bit kernel
    jmp 0x08:start64

bits 64

start64:
    ; Clear registers
    xor rax, rax
    xor rbx, rbx
    xor rcx, rcx
    xor rdx, rdx
    xor rsi, rsi
    xor rdi, rdi

    ; Load IDT
    call idt_load

    ; Call kernel main
    call kernel_main

    ; Halt
    cli
    hlt
    jmp $

bits 32

setup_long_mode:
    ; Enable PAE
    mov eax, cr4
    or eax, 0x20
    mov cr4, eax

    ; Load page table pointer (identity mapping)
    mov eax, pml4
    mov cr3, eax

    ; Enable long mode
    mov ecx, 0xC0000080  ; EFER MSR
    rdmsr
    or eax, 0x100       ; LM bit
    wrmsr

    ; Enable paging
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ret

; Minimal paging structures (identity map first 2MB)
align 4096
pml4:
    dq pdpt + 0x3       ; Present, Read/Write
    times 511 dq 0

align 4096
pdpt:
    dq pdt + 0x3        ; Present, Read/Write
    times 511 dq 0

align 4096
pdt:
    dq pt + 0x3         ; Present, Read/Write
    times 511 dq 0

align 4096
pt:
    %assign i 0
    %rep 512
        dq (i << 12) | 0x3  ; 2MB identity mapping
        %assign i i+1
    %endrep

; Stack
align 16
section .bss
    alignb 16
    resb 16384          ; 16KB stack
stack_top:

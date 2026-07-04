; Global Descriptor Table (GDT) setup for 64-bit mode

bits 32

global gdt_load

gdt_start:
    dq 0x0000000000000000    ; Null descriptor
    
    ; Code descriptor (64-bit)
    dq 0x00209a0000000000    ; Long mode code
    
    ; Data descriptor (64-bit)
    dq 0x0000920000000000    ; Data

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dq gdt_start

gdt_load:
    lgdt [gdt_descriptor]
    ret

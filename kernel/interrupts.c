#include "kernel.h"
#include <stdint.h>

#define IDT_SIZE 256

typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  attributes;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed)) IDT_Entry;

typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) IDT_Descriptor;

static IDT_Entry idt[IDT_SIZE];
static IDT_Descriptor idt_desc;

// Interrupt service routine declarations
extern void isr_0(void);
extern void isr_1(void);
extern void isr_32(void);  // Timer
extern void isr_33(void);  // Keyboard
extern void isr_44(void);  // Mouse

void set_idt_entry(int index, uint64_t handler) {
    idt[index].offset_low = handler & 0xFFFF;
    idt[index].offset_mid = (handler >> 16) & 0xFFFF;
    idt[index].offset_high = (handler >> 32) & 0xFFFFFFFF;
    idt[index].selector = 0x08;  // Kernel code segment
    idt[index].ist = 0;
    idt[index].attributes = 0x8E;  // Present, ring 0, interrupt gate
}

void interrupts_init(void) {
    // Initialize IDT
    idt_desc.limit = sizeof(idt) - 1;
    idt_desc.base = (uint64_t)&idt;
    
    // Set up interrupt handlers
    set_idt_entry(0, (uint64_t)isr_0);   // Division by zero
    set_idt_entry(1, (uint64_t)isr_1);   // Debug
    set_idt_entry(32, (uint64_t)isr_32); // Timer
    set_idt_entry(33, (uint64_t)isr_33); // Keyboard
    set_idt_entry(44, (uint64_t)isr_44); // Mouse
    
    // Load IDT
    asm volatile("lidt %0" : : "m"(idt_desc));
    
    // Enable interrupts
    asm volatile("sti");
}

void keyboard_handler(void) {
    // Read scan code from port 0x60
    uint8_t scan_code = *(volatile uint8_t *)0x60;
    
    // Send acknowledgement to PIC
    asm volatile("outb %0, %1" : : "a"((uint8_t)0x20), "d"((uint16_t)0x20));
}

void mouse_handler(void) {
    // Read mouse data from port 0x60
    uint8_t mouse_data = *(volatile uint8_t *)0x60;
    
    // Send acknowledgement to PIC
    asm volatile("outb %0, %1" : : "a"((uint8_t)0x20), "d"((uint16_t)0xA0));
}

void idt_load(void) {
    asm volatile("lidt %0" : : "m"(idt_desc));
}

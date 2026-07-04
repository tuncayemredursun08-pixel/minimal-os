#include "kernel.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

static uint16_t *vga_buffer = (uint16_t *)VGA_MEMORY;
static int vga_row = 0;
static int vga_col = 0;
static uint8_t current_color = (VGA_COLOR_WHITE << 4) | VGA_COLOR_BLACK;

void vga_init(void) {
    vga_buffer = (uint16_t *)VGA_MEMORY;
    vga_row = 0;
    vga_col = 0;
    vga_clear();
}

void vga_clear(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = (current_color << 8) | ' ';
    }
    vga_row = 0;
    vga_col = 0;
}

void vga_set_color(uint8_t fg, uint8_t bg) {
    current_color = (bg << 4) | fg;
}

void vga_putchar(char c) {
    if (c == '\n') {
        vga_col = 0;
        vga_row++;
    } else if (c == '\r') {
        vga_col = 0;
    } else if (c == '\t') {
        vga_col += 4;
    } else {
        vga_buffer[vga_row * VGA_WIDTH + vga_col] = (current_color << 8) | (unsigned char)c;
        vga_col++;
    }
    
    // Handle line wrapping
    if (vga_col >= VGA_WIDTH) {
        vga_col = 0;
        vga_row++;
    }
    
    // Handle scrolling
    if (vga_row >= VGA_HEIGHT) {
        // Scroll up
        for (int i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++) {
            vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
        }
        // Clear last line
        for (int i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_WIDTH * VGA_HEIGHT; i++) {
            vga_buffer[i] = (current_color << 8) | ' ';
        }
        vga_row = VGA_HEIGHT - 1;
    }
}

void vga_print(const char *str) {
    for (int i = 0; str[i]; i++) {
        vga_putchar(str[i]);
    }
}

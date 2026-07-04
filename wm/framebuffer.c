#include "wm.h"

// Framebuffer resolution and memory
#define FB_WIDTH 1024
#define FB_HEIGHT 768
#define FB_ADDRESS 0xFD000000  // Typical framebuffer address

static uint32_t *framebuffer = (uint32_t *)FB_ADDRESS;
static int fb_width = FB_WIDTH;
static int fb_height = FB_HEIGHT;

void framebuffer_init(void) {
    // Initialize framebuffer
    // In real implementation, we'd set up VBE or use bootloader info
    framebuffer = (uint32_t *)FB_ADDRESS;
    fb_width = FB_WIDTH;
    fb_height = FB_HEIGHT;
}

void framebuffer_clear(uint32_t color) {
    for (int i = 0; i < fb_width * fb_height; i++) {
        framebuffer[i] = color;
    }
}

void framebuffer_draw_pixel(int x, int y, uint32_t color) {
    if (x < 0 || x >= fb_width || y < 0 || y >= fb_height) return;
    framebuffer[y * fb_width + x] = color;
}

void framebuffer_draw_rect(int x, int y, int w, int h, uint32_t color) {
    for (int py = y; py < y + h && py < fb_height; py++) {
        for (int px = x; px < x + w && px < fb_width; px++) {
            if (px >= 0 && py >= 0) {
                framebuffer[py * fb_width + px] = color;
            }
        }
    }
}

void framebuffer_draw_rect_outline(int x, int y, int w, int h, uint32_t color) {
    // Top and bottom lines
    for (int px = x; px < x + w && px < fb_width; px++) {
        if (y >= 0) framebuffer[y * fb_width + px] = color;
        if (y + h - 1 >= 0 && y + h - 1 < fb_height) {
            framebuffer[(y + h - 1) * fb_width + px] = color;
        }
    }
    
    // Left and right lines
    for (int py = y; py < y + h && py < fb_height; py++) {
        if (x >= 0) framebuffer[py * fb_width + x] = color;
        if (x + w - 1 >= 0 && x + w - 1 < fb_width) {
            framebuffer[py * fb_width + x + w - 1] = color;
        }
    }
}

void framebuffer_draw_line(int x1, int y1, int x2, int y2, uint32_t color) {
    // Bresenham's line algorithm (simplified)
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = (dx > dy ? dx : dy);
    
    if (steps == 0) {
        framebuffer_draw_pixel(x1, y1, color);
        return;
    }
    
    for (int i = 0; i <= steps; i++) {
        int x = x1 + (dx * i) / steps;
        int y = y1 + (dy * i) / steps;
        framebuffer_draw_pixel(x, y, color);
    }
}

// Simple 5x7 bitmap font
static const uint8_t font5x7[] = {
    0x00, 0x00, 0x00, 0x00, 0x00,  // Space
    0x20, 0x20, 0x20, 0x00, 0x20,  // !
    0x50, 0x50, 0x00, 0x00, 0x00,  // "
};

void framebuffer_draw_string(int x, int y, const char *str, uint32_t color) {
    // Simple text rendering - just draw rectangles for now
    // In a real implementation, use bitmap fonts
    (void)str;  // Suppress warning
    (void)color;
    (void)x;
    (void)y;
    
    // TODO: Implement proper font rendering
}

#include "wm.h"
#include "../kernel/kernel.h"

// Input handling for keyboard and mouse

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define MOUSE_DATA_PORT 0x60

static int mouse_x = 512;
static int mouse_y = 384;
static int mouse_buttons = 0;

void input_process(void) {
    // Poll keyboard
    uint8_t status = *(volatile uint8_t *)KEYBOARD_STATUS_PORT;
    
    if (status & 0x01) {
        uint8_t scan_code = *(volatile uint8_t *)KEYBOARD_DATA_PORT;
        
        // Handle special keys
        if (scan_code == 0x01) {  // ESC
            // TODO: Handle escape key
        }
        // Handle other scan codes...
    }
    
    // Poll mouse (simplified)
    // TODO: Implement proper PS/2 mouse protocol
}

void input_set_mouse_position(int x, int y) {
    mouse_x = x;
    mouse_y = y;
}

void input_get_mouse_position(int *x, int *y) {
    *x = mouse_x;
    *y = mouse_y;
}

void input_set_mouse_button(int button, int pressed) {
    if (pressed) {
        mouse_buttons |= (1 << button);
    } else {
        mouse_buttons &= ~(1 << button);
    }
}

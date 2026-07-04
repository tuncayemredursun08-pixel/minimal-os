#include "kernel.h"
#include "../wm/wm.h"

// Kernel entry point
void kernel_main(void) {
    // Initialize VGA
    vga_init();
    vga_clear();
    
    // Print banner
    vga_print("\n");
    vga_print("================================\n");
    vga_print("  Minimal OS - Booting...\n");
    vga_print("================================\n\n");
    
    vga_print("[*] Initializing interrupts...\n");
    interrupts_init();
    vga_print("[+] Interrupts initialized\n\n");
    
    vga_print("[*] Initializing window manager...\n");
    wm_init();
    vga_print("[+] Window manager initialized\n\n");
    
    vga_print("[*] Creating test windows...\n");
    wm_create_test_windows();
    vga_print("[+] Test windows created\n\n");
    
    vga_print("================================\n");
    vga_print("  System Ready\n");
    vga_print("================================\n\n");
    vga_print("Waiting for input...\n");
    
    // Main loop
    while (1) {
        // Handle input events
        input_process();
        
        // Render window manager
        wm_render();
        
        // Process window events
        wm_update();
    }
}

// Kernel panic handler
void kernel_panic(const char *message) {
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_RED);
    vga_print("\n\n=== KERNEL PANIC ===");
    vga_print(message);
    vga_print("\nSystem halted.\n");
    
    // Halt
    asm volatile("cli; hlt");
    
    // Infinite loop (should never reach)
    while (1);
}

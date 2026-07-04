#include "wm.h"
#include "../kernel/kernel.h"

static WindowManager wm;

void wm_init(void) {
    wm.screen_width = 1024;
    wm.screen_height = 768;
    wm.bg_color = COLOR_BLUE;
    wm.window_list = NULL;
    wm.focused_window = NULL;
    wm.window_count = 0;
    
    framebuffer_init();
    framebuffer_clear(wm.bg_color);
}

void wm_create_test_windows(void) {
    // Create test windows
    Window *w1 = wm_create_window(100, 100, 300, 200, "Window 1");
    Window *w2 = wm_create_window(250, 150, 300, 200, "Window 2");
    Window *w3 = wm_create_window(400, 200, 300, 200, "Window 3");
    
    if (w1) w1->color = COLOR_RED;
    if (w2) w2->color = COLOR_GREEN;
    if (w3) w3->color = COLOR_CYAN;
}

Window *wm_create_window(int x, int y, int width, int height, const char *title) {
    // Allocate window
    static Window windows[32];  // Static array for memory
    static int window_idx = 0;
    
    if (window_idx >= 32) {
        vga_print("[!] Max windows reached\n");
        return NULL;
    }
    
    Window *w = &windows[window_idx++];
    w->x = x;
    w->y = y;
    w->width = width;
    w->height = height;
    w->color = COLOR_WHITE;
    w->title = title;
    w->z_index = wm.window_count;
    w->visible = 1;
    w->next = NULL;
    w->prev = NULL;
    
    // Add to window list (sorted by z-index)
    if (wm.window_list == NULL) {
        wm.window_list = w;
    } else {
        Window *curr = wm.window_list;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = w;
        w->prev = curr;
    }
    
    wm.window_count++;
    wm_focus_window(w);
    
    return w;
}

void wm_destroy_window(Window *w) {
    if (w == NULL) return;
    
    // Remove from list
    if (w->prev) w->prev->next = w->next;
    if (w->next) w->next->prev = w->prev;
    if (wm.window_list == w) wm.window_list = w->next;
    
    if (wm.focused_window == w) {
        wm.focused_window = w->prev ? w->prev : w->next;
    }
    
    wm.window_count--;
}

void wm_focus_window(Window *w) {
    if (w == NULL) return;
    
    // Move to top (highest z-index)
    if (wm.focused_window) {
        wm.focused_window->z_index--;
    }
    
    // Remove from list
    if (w->prev) w->prev->next = w->next;
    if (w->next) w->next->prev = w->prev;
    if (wm.window_list == w) wm.window_list = w->next;
    
    // Add to front
    w->next = wm.window_list;
    if (wm.window_list) wm.window_list->prev = w;
    w->prev = NULL;
    wm.window_list = w;
    w->z_index = wm.window_count - 1;
    wm.focused_window = w;
}

void wm_move_window(Window *w, int x, int y) {
    if (w == NULL) return;
    w->x = x;
    w->y = y;
}

void wm_resize_window(Window *w, int width, int height) {
    if (w == NULL) return;
    w->width = width;
    w->height = height;
}

void wm_update(void) {
    // Update window states
    // TODO: Handle animations, transitions, etc.
}

void wm_render(void) {
    // Clear screen
    framebuffer_clear(wm.bg_color);
    
    // Draw windows in z-order (back to front)
    Window *curr = wm.window_list;
    while (curr) {
        if (curr->visible) {
            // Draw window background
            framebuffer_draw_rect(curr->x, curr->y, curr->width, curr->height, curr->color);
            
            // Draw window border
            uint32_t border_color = (curr == wm.focused_window) ? COLOR_WHITE : COLOR_GRAY;
            framebuffer_draw_rect_outline(curr->x, curr->y, curr->width, curr->height, border_color);
            
            // Draw title bar
            framebuffer_draw_rect(curr->x, curr->y, curr->width, 24, border_color);
            framebuffer_draw_string(curr->x + 4, curr->y + 4, curr->title, COLOR_BLACK);
        }
        curr = curr->next;
    }
}

void wm_handle_mouse_click(int x, int y) {
    // Find window at (x, y) - search from front to back
    Window *curr = wm.window_list;
    while (curr) {
        if (curr->visible &&
            x >= curr->x && x < curr->x + curr->width &&
            y >= curr->y && y < curr->y + curr->height) {
            wm_focus_window(curr);
            return;
        }
        curr = curr->next;
    }
}

void wm_handle_mouse_move(int x, int y) {
    // TODO: Handle mouse move events (highlighting, dragging, etc.)
}

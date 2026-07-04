#ifndef WM_H
#define WM_H

#include <stdint.h>
#include <stddef.h>

// Window structure
typedef struct Window {
    int x, y;
    int width, height;
    uint32_t color;
    const char *title;
    int z_index;
    int visible;
    struct Window *next;
    struct Window *prev;
} Window;

// Window Manager
typedef struct {
    int screen_width;
    int screen_height;
    uint32_t bg_color;
    Window *window_list;
    Window *focused_window;
    int window_count;
} WindowManager;

// Window Manager functions
void wm_init(void);
void wm_create_test_windows(void);
Window *wm_create_window(int x, int y, int width, int height, const char *title);
void wm_destroy_window(Window *w);
void wm_focus_window(Window *w);
void wm_move_window(Window *w, int x, int y);
void wm_resize_window(Window *w, int width, int height);
void wm_update(void);
void wm_render(void);
void wm_handle_mouse_click(int x, int y);
void wm_handle_mouse_move(int x, int y);

// Framebuffer functions
void framebuffer_init(void);
void framebuffer_clear(uint32_t color);
void framebuffer_draw_pixel(int x, int y, uint32_t color);
void framebuffer_draw_rect(int x, int y, int w, int h, uint32_t color);
void framebuffer_draw_rect_outline(int x, int y, int w, int h, uint32_t color);
void framebuffer_draw_line(int x1, int y1, int x2, int y2, uint32_t color);
void framebuffer_draw_string(int x, int y, const char *str, uint32_t color);

// Input functions
void input_process(void);

// Colors
#define COLOR_BLACK   0x000000
#define COLOR_RED     0xFF0000
#define COLOR_GREEN   0x00FF00
#define COLOR_BLUE    0x0000FF
#define COLOR_WHITE   0xFFFFFF
#define COLOR_GRAY    0x808080
#define COLOR_CYAN    0x00FFFF
#define COLOR_MAGENTA 0xFF00FF
#define COLOR_YELLOW  0xFFFF00

#endif

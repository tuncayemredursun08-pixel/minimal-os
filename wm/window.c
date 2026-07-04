#include "wm.h"

// Window-specific functionality
// This module handles individual window behavior, resizing, moving, etc.

int window_contains_point(Window *w, int x, int y) {
    if (!w) return 0;
    return (x >= w->x && x < w->x + w->width &&
            y >= w->y && y < w->y + w->height);
}

int window_get_titlebar_rect(Window *w, int *x, int *y, int *w_out, int *h_out) {
    if (!w) return 0;
    *x = w->x;
    *y = w->y;
    *w_out = w->width;
    *h_out = 24;  // Title bar height
    return 1;
}

int window_get_client_rect(Window *w, int *x, int *y, int *w_out, int *h_out) {
    if (!w) return 0;
    *x = w->x;
    *y = w->y + 24;  // Below title bar
    *w_out = w->width;
    *h_out = w->height - 24;
    return 1;
}

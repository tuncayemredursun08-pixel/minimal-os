#include <stddef.h>
#include <stdint.h>

// Simple memory allocator

#define HEAP_SIZE 1048576  // 1MB heap
#define HEAP_START 0x10000000

static uint8_t heap[HEAP_SIZE];
static size_t heap_ptr = 0;

void *malloc(size_t size) {
    if (heap_ptr + size > HEAP_SIZE) {
        return NULL;  // Heap exhausted
    }
    
    void *ptr = &heap[heap_ptr];
    heap_ptr += size;
    
    // Align to 8 bytes
    heap_ptr = (heap_ptr + 7) & ~7;
    
    return ptr;
}

void free(void *ptr) {
    (void)ptr;  // Simple allocator - no real free
}

void *calloc(size_t count, size_t size) {
    size_t total = count * size;
    void *ptr = malloc(total);
    
    if (ptr) {
        uint8_t *p = (uint8_t *)ptr;
        for (size_t i = 0; i < total; i++) {
            p[i] = 0;
        }
    }
    
    return ptr;
}

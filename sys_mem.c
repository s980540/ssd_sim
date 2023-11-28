#include "sys_mem.h"

void *aligned_alloc(size_t size, u32 align)
{
    size_t addr;
    void *alloc_ptr, *aligned_ptr;

    alloc_ptr = malloc(size + align + sizeof(size_t));
    if (alloc_ptr != NULL) {
        addr        = (size_t)alloc_ptr + align + sizeof(size_t);
        aligned_ptr = (void *)(addr - (addr % align));
        *((size_t *)aligned_ptr - 1) = (size_t)alloc_ptr;
        // // debug
        // printf("%4d %8x %8x %8x %8x\n",
        //     (size >= KB ? size / KB : size), align, alloc_ptr, aligned_ptr, *((size_t *)aligned_ptr - 1));
    } else
        return NULL;

    return aligned_ptr;
}
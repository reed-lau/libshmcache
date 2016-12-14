//shm_striping_allocator.c

#include <errno.h>
#include "shm_striping_allocator.h"

void shm_striping_allocator_init(struct shm_striping_allocator *allocator,
		const int64_t base_offset, const int total_size)
{
    allocator->offset.base = base_offset;
    allocator->size.total = total_size;
    allocator->offset.end = base_offset + total_size;

    shm_striping_allocator_reset(allocator);
}

void shm_striping_allocator_reset(struct shm_striping_allocator *allocator)
{
    allocator->size.used = 0;
    allocator->offset.free = allocator->offset.base;
}

int64_t shm_striping_allocator_alloc(struct shm_striping_allocator *allocator,
        const int size)
{
    int64_t ptr_offset;
    if (allocator->offset.end - allocator->offset.free < size) {
        return -1;
    }

    ptr_offset = allocator->offset.free;
    allocator->offset.free += size;
    allocator->size.used += size;
    return ptr_offset;
}

void shm_striping_allocator_free(struct shm_striping_allocator *allocator,
        const int size)
{
    allocator->size.used -= size;
    if (allocator->size.used <= 0) {
        shm_striping_allocator_reset(allocator);
    }
}


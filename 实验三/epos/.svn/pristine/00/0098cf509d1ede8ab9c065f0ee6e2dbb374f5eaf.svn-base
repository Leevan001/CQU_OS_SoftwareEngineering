/**
 * vim: filetype=c:fenc=utf-8:ts=4:et:sw=4:sts=4
 */
#include <sys/types.h>
#include <string.h>
#include <stdint.h>

struct chunk {
    char signature[4];  /* "OSEX" */
    struct chunk *next; /* ptr. to next chunk */
    int state;          /* 0 - free, 1 - used */
#define FREE   0
#define USED   1

    int size;           /* size of this chunk */
};

static struct chunk *chunk_head;

void init_memory_pool(size_t heap_size, uint8_t *heap_base)
{
    chunk_head = (struct chunk *)heap_base;
    strncpy(chunk_head->signature, "OSEX", 4);
    chunk_head->next = NULL;
    chunk_head->state = FREE;
    chunk_head->size  = heap_size;
}

void *malloc(size_t size)
{
    return NULL;
}

void free(void *ptr)
{
}

void *calloc(size_t num, size_t size)
{
    return NULL;
}

void *realloc(void *oldptr, size_t size)
{
    return NULL;
}


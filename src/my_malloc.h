#ifndef CHUNK_META_H
#define CHUNK_META_H

// for testing
typedef struct ChunkMeta {
    struct ChunkMeta *prev;
    struct ChunkMeta *next;
    unsigned int size;
    unsigned int magic_num;
} ChunkMeta;

void *my_malloc(unsigned int size);

void my_free(void *ptr);
#endif
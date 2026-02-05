// # Materials https://danluu.com/malloc-tutorial/
#include "my_malloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAGIC_USED 0x61a8d2e9
#define MAGIC_FREE 0x73a9e991

ChunkMeta *HEAD = NULL;

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

// TODO: Consider adding two chunks merging
ChunkMeta *find_free_chunk(unsigned int size, ChunkMeta **last_chunk) {
    ChunkMeta *curr = HEAD;
    printf("HEAD %p\n", HEAD);

    while (curr && !(curr->magic_num == MAGIC_FREE && curr->size >= size)) {
        *last_chunk = curr;
        curr = curr->next;
        printf("free chunk iteration: last %p  \t cur%p \n", *last_chunk, curr);
    }

    return curr;
}

// TODO: consider inlining of this func
void *request_memory(unsigned int size) { return sbrk((sizeof(ChunkMeta) + size)); }

// TODO: consider inlining of this func
ChunkMeta *get_chunk_meta(void *ptr) { return (ChunkMeta *)(ptr - sizeof(ChunkMeta)); }

// TODO: Consider adding memory alignment
void *my_malloc(unsigned int size) {
    if (!HEAD) {
        ChunkMeta new_chunk = {NULL, NULL, size, MAGIC_USED};
        void *allocated_ptr = request_memory(size);
        *(ChunkMeta *)allocated_ptr = new_chunk;
        HEAD = allocated_ptr;
        printf("00 meta addr %p\n", allocated_ptr);
        printf("00 head ptr %p\n", HEAD);
        return (allocated_ptr + sizeof(ChunkMeta));
    } else {
        ChunkMeta *last_chunk = HEAD;
        ChunkMeta *chunk_ptr = find_free_chunk(size, &last_chunk);

        printf("FREE CHUNK %p\t LAST: %p\n", chunk_ptr, last_chunk);
        if (!chunk_ptr) {
            void *allocated_ptr = request_memory(size);
            printf("01 new allocated memory pointer %p\n", allocated_ptr);
            printf("01 pointer to the last chank = %p\n", last_chunk);
            ChunkMeta new_chunk = {last_chunk, NULL, size, MAGIC_USED};
            *(ChunkMeta *)allocated_ptr = new_chunk;
            last_chunk->next = allocated_ptr;

            return (allocated_ptr + sizeof(ChunkMeta));
        } else {
            printf("02 %p \n", chunk_ptr);
            // TODO: Consider adding chunk resize logic
            chunk_ptr->magic_num = MAGIC_USED;
            return ((void *)chunk_ptr + sizeof(ChunkMeta));
        }
    }
}

void my_free(void *ptr) {
    ChunkMeta *chunk_meta = get_chunk_meta(ptr);

    // TODO: Consider throwing a NULL or status code.
    if (!chunk_meta || chunk_meta->magic_num != MAGIC_USED) {
        printf("ERROR WHILE SETTING MAGIC TO FREE");
        return;
    }

    chunk_meta->magic_num = MAGIC_FREE;
}
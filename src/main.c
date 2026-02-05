#include "my_malloc.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

ChunkMeta *_get_chunk_meta(void *ptr) { return (ptr - sizeof(ChunkMeta)); }

int main() {
    printf("%lu\n", sizeof(ChunkMeta));

    printf("\n===========FITST============\n\n");
    void *ptr1 = my_malloc(4);

    printf("from main %p\n", ptr1);

    ChunkMeta *metadata_ptr1 = _get_chunk_meta(ptr1);

    // check if the metadata has correct position
    assert(metadata_ptr1->magic_num == 0x61a8d2e9); // magic used

    printf("\n===========SECOND============\n\n");

    void *ptr2 = my_malloc(4);
    printf("second chunk addr %p\n", ptr2);
    ChunkMeta *metadata_ptr2 = _get_chunk_meta(ptr2);

    // check if the metadata has correct position in second allocation
    printf("metadata magic num %x \n", metadata_ptr2->magic_num);
    assert(metadata_ptr2->magic_num == 0x61a8d2e9);

    // Check that after creating new chunk in
    // the cain previous one will set -> next to new chunk
    printf("metadata of next %p\n", metadata_ptr1->next);
    assert(metadata_ptr1->next == metadata_ptr2);

    // Check if the next Chunk in the chain has pointer
    // to previous one in -> prev
    assert(metadata_ptr2->prev == metadata_ptr1);

    printf("\n===========THIRD============\n");

    void *ptr3 = my_malloc(4);
    ChunkMeta *metadata_ptr3 = _get_chunk_meta(ptr3);

    printf("Address of second chunk metadata %p\n", metadata_ptr3);

    // Check that after creating new chunk in
    // the cain previous one will set -> next to new chunk
    printf("PTR NEXT %p\n", metadata_ptr2->next);
    printf("PTR CURR %p\n", metadata_ptr3);

    assert(metadata_ptr2->next == metadata_ptr3);

    assert(metadata_ptr3->prev == metadata_ptr2);

    printf("\n===========FREE() TESTING============\n");
    my_free(ptr2);

    assert(metadata_ptr2->magic_num == 0x73a9e991); // MAGIC FREE

    void *ptr_afrer_free = my_malloc(3);
    printf("NEW PTR AFTER FREE %p\n", ptr_afrer_free);
    printf("OLD 2 PTR %p \n", ptr2);

    assert(ptr_afrer_free == ptr2);
}

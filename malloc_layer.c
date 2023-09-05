#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#define TEST_ALLOC_BYTES      50
#define TEST_ALLOC_ITERATIONS 20
#define TEST_ALLOC_DEBUG

#undef TEST_ALLOC_DEBUG // comment it to debug code!

typedef uint8_t myMallocOffset_t; // 8-bits is enough to hold 128 bytes boundaries

#define myMallocAligment (128)
#define myMallocAlign(p) (((p)+(myMallocAligment-1)) & ~(myMallocAligment-1))
#define myMallocOffsetSize (sizeof(myMallocOffset_t)) 
#define myMallocHeaderSize (myMallocOffsetSize+(myMallocAligment-1)) // Header size is for the worst case (malloc address aligned)

void* my_malloc(uint32_t size_bytes){
    void* ptr=NULL;

    if(size_bytes){
        // Allocates header size + requested bytes
        void * p = malloc(myMallocHeaderSize+size_bytes);
        #ifdef TEST_ALLOC_DEBUG
            printf("- malloc [non aligned address=%p] : [requested bytes=%d].\n",(uintptr_t)p, (int32_t)(myMallocHeaderSize+size_bytes));
        #endif
        if(p){
            /* If malloc address is not aligned the user pointer will be aligned to next boundary
   If malloc address is aligned (there is no room space to store offset behind it) the user pointer will   skip current boundary to the next one (worst case). */
            ptr = (void *) myMallocAlign(((uintptr_t)p + myMallocOffsetSize));
            // Calculates offset to the aligned (up) boundary
            ptrdiff_t offset = (uintptr_t)ptr - (uintptr_t)p;
            #ifdef TEST_ALLOC_DEBUG
                printf("aligned address after offset [%p].\n",(uintptr_t)ptr);            
                printf("offset=%d, address to store offset=%p.\n",offset,(uintptr_t)((myMallocOffset_t*)ptr - myMallocOffsetSize));
            #endif
            // Stores it always before the aligned pointer
            *((myMallocOffset_t*)ptr - myMallocOffsetSize) = offset;

        }
    }

    return ptr;
}

void my_free(void* ptr){
    if(ptr==NULL) return;
    // Recovers the stored offset
    myMallocOffset_t offset = *((myMallocOffset_t*)ptr-1);
    // Restores the malloc´s address pointer
    void* p=((myMallocOffset_t*)ptr) - offset;
    #ifdef TEST_ALLOC_DEBUG
        printf("free [offset=%d] : [address to free=%p].\n",offset,(uintptr_t)p);
    #endif
    // Frees allocated memory with a valid pointer 
    free(p);
}

int main() {
    int8_t* p[TEST_ALLOC_ITERATIONS];
    printf("TESTING DYNAMIC ALLOCATION LAYER [ITERATIONS=%d]\n",TEST_ALLOC_ITERATIONS);
    for(int i=0;i<TEST_ALLOC_ITERATIONS;i++){
        p[i]=my_malloc(TEST_ALLOC_BYTES);
        printf("Testing aligment : address[%d]=%p ==> %s.\n",i,(uintptr_t)p[i],((uintptr_t)p[i]%myMallocAligment)?"NOT PASSED":"PASSED");
    }
    for(int i=0;i<TEST_ALLOC_ITERATIONS;i++){
        my_free((void*)p[i]);
    }
    return 0;
}

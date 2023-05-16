// Name: Charles Tran

#include "pool_alloc.c"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


// TEST 1: This function is to test if pool_init successfully or not.
void function_test123(size_t *block_sizes, size_t block_count_size) {
    bool initPool = pool_init(block_sizes, block_count_size);
    if (!initPool) {
        printf("TEST: FAILED\n");
    }
    else
        printf("TEST: PASSED\n");
}


void function_test4(size_t *block_sizes, size_t block_count_size) {
    bool initPool = pool_init(block_sizes, block_count_size);
    void *request = pool_malloc(32);
    if (g_pool_heap == request)
        printf("TEST: PASSED\n");
    else
        printf("TEST: FAILED\n");
}

void function_test5(size_t *block_sizes, size_t block_count_size) {
    bool initPool = pool_init(block_sizes, block_count_size);
    void *request = pool_malloc(1024);
    if (&g_pool_heap[32768] == request)
        printf("TEST: PASSED\n");
    else {
        printf("TEST: FAILED\n");
    }
}

void function_test6(size_t *block_sizes, size_t block_count_size) {
    bool initPool = pool_init(block_sizes, block_count_size);
    if (initPool) {
        void *request32 = pool_malloc(32);
        pool_free(request32);
    }
    if (used32[0] == false)
        printf("TEST: PASSED\n");
    else
        printf("TEST: FAILED\n");
}

void function_test7(size_t *block_sizes, size_t block_count_size) {
    bool initPool = pool_init(block_sizes, block_count_size);
    if (initPool) {
        void *request1024 = pool_malloc(1024);
        pool_free(request1024);
    }
    if (used1024[0] == false)
        printf("TEST: PASSED\n");
    else
        printf("TEST: FAILED\n");
}

int main() {
    // TEST 1: Initialize an array with appropriate size. This test wants to check if pool_init() works correctly 
    // Expected result: PASSED
    size_t block_sizes_1[] = {1024,32, 1024, 32, 1024, 32};
    function_test123(block_sizes_1, sizeof(block_sizes_1)/sizeof(block_sizes_1[0]));
    
    // TEST 2: Initialize an array with size 1024 and the array contains all block 32-byte object
    // Expected result: FAILED because the limit for 32-byte object is 1024 blocks
    size_t block_sizes_2[1025] = { [0 ... 1024] = 32 };
    function_test123(block_sizes_2, sizeof(block_sizes_2)/sizeof(block_sizes_2[0]));
    //TEST 3: Initialize an array with size 33 and the array contains all block 1024-byte object
    // Expected result: FAILED because the limit for 1024-byte object is 32 blocks.
    size_t block_sizes_3[33] = { [0 ... 32] = 1024 };
    function_test123(block_sizes_3, sizeof(block_sizes_3)/sizeof(block_sizes_3[0]));

    // TEST 4: Initialize an array with appropriate size. This test wants to check if pool_malloc() works correctly.
    // The user will request the first block of 32-byte object. The allocator will return a block of 32-byte object for user.
    // This address should match with the base address of the g_pool_heap[] 
    // Expected result: PASSED
    size_t block_sizes_4[] = {32,1024,1024, 32, 1024, 32};
    function_test4(block_sizes_4, sizeof(block_sizes_4)/sizeof(block_sizes_4[0]));

    // TEST 5: Initialize an array with appropriate size. This test wants to check if pool_malloc() works correctly.
    // The user will request the first block of 1024-byte object. The allocator will return a block of 1024-byte object for user.
    // This address should match with the address of the g_pool_heap at index 32768
    // Expected result: PASSED
    size_t block_sizes_5[] = {32,1024,1024, 32, 1024, 32};
    function_test5(block_sizes_5, sizeof(block_sizes_5)/sizeof(block_sizes_5[0]));

    // TEST 6: In this test, the user tries to request a block of 32-byte object and free it after that.
    // The purpose of this test is to check if pool_free() works correctly. pool_free() will assign true
    // back to used32[0] = false (mark it available)
    // Expected result: PASSED  
    size_t block_sizes_6[] = {32,1024,1024, 32, 1024, 32};
    function_test6(block_sizes_6, sizeof(block_sizes_6)/sizeof(block_sizes_6[0]));

    // TEST 7: In this test, the user tries to request a block of 1024-byte object and free it after that.
    // The purpose of this test is to check if pool_free() works correctly. pool_free() will assign true
    // back to used32[0] = false (mark it available)
    // Expected result: PASSED  
    size_t block_sizes_7[] = {32,1024,1024, 32, 1024, 32};
    function_test7(block_sizes_7, sizeof(block_sizes_7)/sizeof(block_sizes_7[0]));

    return 0;
}
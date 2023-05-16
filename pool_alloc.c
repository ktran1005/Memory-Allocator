#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "pool_alloc.h"
#include <stdlib.h>
#include <string.h>
static uint8_t g_pool_heap[65536];
static bool used32[1024] = {0};
static bool used1024[32] = {0};
int total_block_32;
int total_block_1024;

int available32(bool* boolBuffer32) {
    for (int i=0; i < 1024; i++) {
        if (boolBuffer32[i] == false)
            return i;
    }
    return -1;
}

int available1024(bool* boolBuffer1024) {
    for (int i=0; i < 32; i++) {
        if (boolBuffer1024[i] == false)
            return i;
    }
    return -1;
}

bool pool_init(const size_t* block_sizes, size_t block_size_count) {
    // Implement me!
    // reset everything 
    total_block_32 = 0;
    total_block_1024 = 0;
    memset(used32, 0, 1024);
    memset(used1024, 0, 32);
    for (int i=0; i < block_size_count; i++) {
        if (block_sizes[i] == 32)
            total_block_32++;
        else if (block_sizes[i] == 1024)
            total_block_1024++;
        
    }
    if (total_block_32 > 1024 || total_block_1024 > 32)
        return false;
    
    return true;
}

void* pool_malloc(size_t n)
{
    // Implement me!
    if (n > 1024){
        printf("The pool memory allocater is only available for 32-byte or 1024-byte object\n");
        exit(EXIT_FAILURE);
    }
    if (n == 32 && total_block_32 > 0) {
        int availableIdx = available32(used32);
        if (availableIdx == -1) {
            printf("No 32-byte block sizes are available at the moment\n");
            return NULL;
        }
        used32[availableIdx] = true;
        total_block_32--;
        return (void*)&g_pool_heap[availableIdx*32];
    }
    if (n == 32 && total_block_32 <= 0 && total_block_1024 > 0) {
        int availableIdx = available1024(used1024);
        used1024[availableIdx] = true;
        total_block_1024--;
    }

    if (n == 1024 && total_block_1024 > 0) {
        int availableIdx = available1024(used1024);
        if (availableIdx == -1) {
            printf("No more block for 1024-byte object\n");
            return NULL;
        }
        used1024[availableIdx] = true;
        total_block_1024--;
        return (void*)&g_pool_heap[availableIdx*1024+32768];
    }
    if (n == 1024 && total_block_1024 <= 0) {
        printf("No more block for 1024-byte object at the moment\n");
        return NULL;
    }
}

void pool_free(void* ptr)
{
    // Implement me!
    uint16_t addr = (uint8_t*)ptr - &g_pool_heap[0];
    // check to see if the pointer is in the fist half of the pool allocator (from 0-32767)
    if (addr < 32767) {
        int idx = addr / 32;
        used32[idx] = false;
        total_block_32++;
    }
    // check to see if the pointer is in the second half of the pool allocator (from 32768-65536)
    else if (addr > 32767) {
        int idx = (addr - 32768)/1024;
        used1024[idx] = false;
        total_block_1024++;
    } 
    else {
        printf("Invalid address\n");
        exit(EXIT_FAILURE);
    }   
}

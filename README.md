# Memory-Allocator

## Overview
   * In this project, I implement a block pool memory allocator (sometimes also refered to as a **slab allocator** in kernel programming) in C. <br />

   * This allocator is optimized for allocating objects of certain sizes that are known at initialization time. The allocator is configured by the user with the set of block sizes that are appropriate for the application (for example, imagine that you know ahead of time that your application will only ever dynamically allocate 32-byte objects and 1024-byte objects). Internally, the allocator will create block pools for each of those specified sizes. When client code calls pool_malloc(n), it will receive the smallest block that is both free and greater than or equal to n in size. This sort of allocator can be very useful in specialized scenarios where the allocation patterns and sizes are well known at initialization time, because it can make a number of simplifying assumptions to improve performance and metadata overhead.

## Assumption <br />
   * I assume that the application will only ever dynamically allocate 32-byte objects and 1024-byte objects 
   * The maximum number of blocks (size 32) that we can allocate in the g_pool_heap are 1024 blocks. The maximum number of blocks (size 1024) that we can allocate are 32 blocks.

## Implementation
   *  Divide the heap array into half which means blocks for 32-byte object will start from index 0 to 32767 in g_pool_heap array, blocks for 1024-byte object will start from index 32768 to 65536. <br />
   *  The **pool_init()** function will traverse the **block_sizes[]** array given by the users to count how many blocks for 32-byte objects and how many blocks for 1024-byte object. The function will populate that many blocks in the **g_pool_heap[]**. The maximum number that we can allocate for 32-byte object is 1024 blocks and 1024-byte object is 32 blocks. If the given array goes beyond these numbers for 32 or 1024-byte object, then the pool_init() 
returns NULL. Otherwise, the total number of 32-byte object will store in **total_block_32 (global variable)**, the total number of 1024-byte object will store in **total_block_1024 (global variable)** and return true. <br />
   * I declare two Boolean arrays (**used32[1024]** and **used1024[32]**) to keep track of the state of each block of memory of 32-byte and 1024-byte object. Each index from these two arrays will correspond to the base address of each block in **g_pool_heap[]**. False means free and True means used. When the user calls pool_malloc(), the function will traverse one of these two boolean arrays to check available block, if it finds one then it will return the its index. We will use this index to find where the block locates in g_pool_heap[] by following the equation:
      * 32-byte object: 32*index -> location of free block in g_pool_heap[] 
      * 1024-byte object: (index * 1024) + 32768 -> location of free block in g_pool_heap[]

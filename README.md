# Memory-Allocator

## Overview
In this project, I implement a block pool memory allocator (sometimes also refered to as a **slab allocator** in kernel programming) in C. <br />

This allocator is optimized for allocating objects of certain sizes that are known at initialization time. The allocator is configured by the user with the set of block sizes that are appropriate for the application (for example, imagine that you know ahead of time that your application will only ever dynamically allocate 32-byte objects and 1024-byte objects). Internally, the allocator will create block pools for each of those specified sizes. When client code calls pool_malloc(n), it will receive the smallest block that is both free and greater than or equal to n in size. This sort of allocator can be very useful in specialized scenarios where the allocation patterns and sizes are well known at initialization time, because it can make a number of simplifying assumptions to improve performance and metadata overhead.

## Assumption <br />
    *I assume that the application will only ever dynamically allocate 32-byte objects and 1024-byte objects
    *The maximum number of blocks (size 32) that we can allocate in the g_pool_heap are 1024 blocks. The maximum number of blocks (size 1024) that we can allocate are 32 blocks.

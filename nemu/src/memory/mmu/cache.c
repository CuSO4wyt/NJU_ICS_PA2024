#include "memory/mmu/cache.h"
#include <stdlib.h>
#include "memory/memory.h"
#include <time.h>

#define CACHE_SIZE 1024 // 64KB Cache size, with 64B per block
#define BLOCK_SIZE 64   // Each Cache block size is 64 bytes
#define SET_ASSOCIATIVITY 8 // 8-way set associative
#define NUM_SETS 128 // 128 sets

// Define cache structure

// Cache line structure
typedef struct {
    bool valid;  // Valid bit to indicate if this line holds valid data
    uint32_t tag; // Tag to identify the data
    uint8_t data[BLOCK_SIZE]; // Data stored in the Cache block
} CacheLine;

// Cache set structure (each set contains 8 cache lines)
typedef struct {
    CacheLine lines[SET_ASSOCIATIVITY];
} CacheSet;

// Cache structure with 128 sets
CacheSet cache[NUM_SETS];



// Initialize the cache
void init_cache() {
    srand(0); // Use a fixed seed for deterministic behavior 
    // Mark all lines as invalid initially
    for (int i = 0; i < NUM_SETS; i++) 
    {
        for (int j = 0; j < SET_ASSOCIATIVITY; j++) 
        {
            cache[i].lines[j].valid = false; 
        }
    }
}

// Write data to cache (write-through)
void cache_write(paddr_t paddr, size_t len, uint32_t data) {
    uint32_t set_index = (paddr >> 6) & 0x7F;
    uint32_t tag = (paddr >> 13);
    uint32_t offset = paddr & 0x3F;

    CacheSet *set = &cache[set_index];
    int line_to_write = -1;

    // Try to find a valid line with the correct tag
    for (int i = 0; i < SET_ASSOCIATIVITY; i++) 
    {
        if (set->lines[i].valid && set->lines[i].tag == tag) 
        {
            line_to_write = i;
            break;
        }
    }

    if (line_to_write == -1) {
        // Cache miss - use random replacement
        line_to_write = rand() % SET_ASSOCIATIVITY;
        set->lines[line_to_write].valid = true;
        set->lines[line_to_write].tag = tag;

        // Read the entire block into the Cache (since we use write-through, load the entire block)
        memcpy(set->lines[line_to_write].data, hw_mem + (paddr & ~0x3F), BLOCK_SIZE);
    }

    // Handle data across block boundaries
    if (offset + len > BLOCK_SIZE) {
        size_t first_part_len = BLOCK_SIZE - offset;  // Length of first part that fits in the current block
        size_t second_part_len = len - first_part_len; // Remaining length that will go into the next block

        // Write the first part to the current block
        memcpy(set->lines[line_to_write].data + offset, &data, first_part_len);
        memcpy(hw_mem + paddr, &data, first_part_len); // Write-through to main memory

        // Write the second part to the next block
        paddr += first_part_len;  // Move to next block
        cache_write(paddr, second_part_len, data >> (first_part_len * 8));  // Shift the data to fit
    } else {
        // Write within the current block
        memcpy(set->lines[line_to_write].data + offset, &data, len);
        memcpy(hw_mem + paddr, &data, len); // Write-through to main memory
    }
}

// Read data from cache
uint32_t cache_read(paddr_t paddr, size_t len) 
{
    uint32_t set_index = (paddr >> 6) & 0x7F;
    uint32_t tag = (paddr >> 13);
    uint32_t offset = paddr & 0x3F;

    CacheSet *set = &cache[set_index];
    uint32_t ret = 0;
    int line_to_read = -1;

    // Try to find a valid line with the correct tag
    for (int i = 0; i < SET_ASSOCIATIVITY; i++) {
        if (set->lines[i].valid && set->lines[i].tag == tag) {
            line_to_read = i;
            break;
        }
    }

    if (line_to_read == -1) {
        // Cache miss - use random replacement
        line_to_read = rand() % SET_ASSOCIATIVITY;
        set->lines[line_to_read].valid = true;
        set->lines[line_to_read].tag = tag;

        // Load the block from main memory to the Cache
        memcpy(set->lines[line_to_read].data, hw_mem + (paddr & ~0x3F), BLOCK_SIZE);
    }

    // Handle data across block boundaries
    if (offset + len > BLOCK_SIZE) {
        size_t first_part_len = BLOCK_SIZE - offset;  // Length of first part that fits in the current block
        size_t second_part_len = len - first_part_len; // Remaining length that will go into the next block

        // Read the first part from the current block
        memcpy(&ret, set->lines[line_to_read].data + offset, first_part_len);

        // Read the second part from the next block
        paddr += first_part_len;  // Move to next block
        uint32_t next_part = cache_read(paddr, second_part_len);  // Recursively read the next block
        ret |= (next_part << (first_part_len * 8));  // Combine the two parts
    } else {
        // Read within the current block
        memcpy(&ret, set->lines[line_to_read].data + offset, len);
    }
    
    return ret;
}

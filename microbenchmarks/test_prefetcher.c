#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>  // For mmap

#define PAGE_SIZE (4 * 1024) // 4KiB
#define HUGE_PAGE_SIZE (2 * 1024 * 1024) // 4KiB
#define CACHE_LINE_SIZE 64   // 64 bytes
#define GROUP_COUNT 64       // Number of 64 byte groups
#define OFFSET (HUGE_PAGE_SIZE) 
#define NUM_GROUPS 1000

int main() {
    size_t alloc_size = OFFSET * NUM_GROUPS;
    // char *base = malloc(alloc_size);
    printf("HIHI");
    char *base = (char *) mmap(NULL, alloc_size, PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    
    printf("HIHI");
    if (base == MAP_FAILED) {
        perror("Failed to allocate memory with hugepages");
        exit(1);
    }

    printf("HIHI");
    // Initialize the allocated memory to avoid page faults during the loop
    // for (size_t i = 0; i < alloc_size; i += OFFSET) {
    //     for (size_t j = 0; j < GROUP_COUNT * CACHE_LINE_SIZE; j += CACHE_LINE_SIZE) {
    //         base[i + j] = (char)(i + j);
    //     }
    // }

    while (1) {
        for (size_t i = 0; i < alloc_size; i += OFFSET) {
            // for (size_t j = PAGE_SIZE * (511); j < HUGE_PAGE_SIZE ; j += 16) {
            for (size_t j = PAGE_SIZE * (230); j < PAGE_SIZE * 231 ; j += 100000) {
                volatile char val = base[i + j];
            }
            volatile float k = 1.01;
            while (k <10) {k = k*1.01;}
            volatile float k2 = k;
        }
    }

 
    munmap(base, alloc_size);
    return 0;
}

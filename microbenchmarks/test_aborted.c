// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>

// #define NUMBER_OF_ADDRESSES 16 // Adjust as needed
// #define OFFSET (2*1024*1024)  // 2MiB


// int main(int argc, char* argv[]) {
//     if (argc != 2) {
//         printf("Usage: %s <probability (0 <= p <= 1)>\n", argv[0]);
//         return 1;
//     }

//     double p = atof(argv[1]);
//     if (p < 0.0 || p > 1.0) {
//         printf("Please provide a valid probability (0 <= p <= 1).\n");
//         return 1;
//     }

//     char *base = malloc(OFFSET * NUMBER_OF_ADDRESSES);
//     if (!base) {
//         perror("Failed to allocate memory");
//         exit(1);
//     }

//     // Initialize the allocated memory to avoid page faults during the loop
//     for (int i = 0; i < NUMBER_OF_ADDRESSES; i++) {
//         base[i * OFFSET] = i;
//     }

//     srand(time(NULL));

//     while (1) {
//         for (int i = 0; i < NUMBER_OF_ADDRESSES; i++) {
//             double random_val = (double)rand() / RAND_MAX;

//             if (random_val <= p) {
//                 volatile char val = base[i * OFFSET];
//             } else {
//                 volatile char val = base[(NUMBER_OF_ADDRESSES - i + 8) % NUMBER_OF_ADDRESSES * OFFSET];
//             }
//         }
//     }

//     free(base);
//     return 0;
// }
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

// int main() {
int main(int argc, char* argv[]) {
        if (argc != 2) {
        printf("Usage: %s <probability (0 <= p <= 1)>\n", argv[0]);
        return 1;
    }

    double p = atof(argv[1]);
    if (p < 0.0 || p > 1.0) {
        printf("Please provide a valid probability (0 <= p <= 1).\n");
        return 1;
    }
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
            double random_val = (double)rand() / RAND_MAX;

            if (random_val <= p) {
                volatile char val = base[i ];
            } else {
                volatile char val = base[(NUM_GROUPS*OFFSET - i + 8) % NUM_GROUPS*OFFSET];
            }
        }
    }

 
    munmap(base, alloc_size);
    return 0;
}

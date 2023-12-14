#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>

#define PAGE_SIZE 4096  // 4KiB
#define HUGE_PAGE_SIZE (2 * 1024 * 1024) // 2MiB

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <number_of_addresses>\n", argv[0]);
        return 1;
    }

    int num_addresses = atoi(argv[1]);

    // Allocate space for the desired addresses based on user input
    void **desired_addresses = malloc(num_addresses * sizeof(void *));

    // Initialize the desired addresses array
    for (int i = 0; i < num_addresses; ++i) {
        // desired_addresses[i] = (void *)(0x1000000000ULL * (i+1));  // Thrash ptpde
        desired_addresses[i] = (void *)(0x10000000ULL * (i+1));  // Thrash pde
    }

    for (int i = 0; i < num_addresses; i++) {
        void *addr = mmap(desired_addresses[i], PAGE_SIZE, PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANON | MAP_FIXED, -1, 0);
        // void *addr = mmap(desired_addresses[i], HUGE_PAGE_SIZE, PROT_READ | PROT_WRITE,
        //                   MAP_PRIVATE | MAP_ANON | MAP_FIXED | MAP_HUGETLB, -1, 0);
        if (addr == MAP_FAILED) {
            perror("mmap failed");
            exit(1);
        } else if (addr != desired_addresses[i]) {
            printf("Failed to map desired address for range %d\n", i);
            exit(1);
        }
    }

    printf("Successfully mapped memory to target GB ranges.\n");

    // Touch each page to ensure they are in memory.
    for (int i = 0; i < num_addresses; i++) {
        *((char *)desired_addresses[i]) = 'a';
    }

    while (1) {
        for (int i = 0; i < num_addresses; i++) {
            volatile char val = ((char*) desired_addresses[i])[0];
        }
    }

    for (int i = 0; i < num_addresses; i++) {
        munmap(desired_addresses[i], PAGE_SIZE);
    }

    return 0;
}

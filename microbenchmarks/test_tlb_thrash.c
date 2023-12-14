#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <number_of_addresses> <diff_cache_lines>\\n", argv[0]);
        exit(1);
    }
    int NUMBER_OF_ADDRESSES = atoi(argv[1]);
    int diff_cache_lines = atoi(argv[2]);
    int OFFSET = 512*1024; 

    char *base = malloc(OFFSET * NUMBER_OF_ADDRESSES);
     if (!base) {
        perror("Failed to allocate memory");
        exit(1);
    }

    for (int i = 0; i < NUMBER_OF_ADDRESSES; i++) {
        base[i * OFFSET] = i;
    }

    while (1) {
        for (int i = 0; i < NUMBER_OF_ADDRESSES; i++) {
            // volatile char val = base[i * OFFSET + ((1<<10)*i*diff_cache_lines)];
            volatile char val = base[(i * OFFSET + ((1<<10)*i*diff_cache_lines)) % (OFFSET * NUMBER_OF_ADDRESSES)];
        }
    }

    free(base);
    return 0;
}

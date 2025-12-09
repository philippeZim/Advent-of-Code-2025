/*
 * Role: Senior Low-Level Systems Optimization Engineer
 * Context: High-Frequency Trading / Competitive Programming
 * Objective: Minimize latency, maximize throughput.
 * Method: mmap, custom parsing, closed-form arithmetic summation.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

// Hardcoded path per constraints
#define INPUT_FILE "/home/eron/Documents/Programming/C/Advent-of-Code-2025/day 2/input.txt"

// Precomputed powers of 10 for quick lookups.
// Supports up to 10^18 (covering uint64_t range).
static const uint64_t P10[] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL
};

// Custom output for __int128 to stdout
void print_u128(unsigned __int128 n) {
    char buffer[40];
    int i = 0;
    if (n == 0) {
        write(1, "0\n", 2);
        return;
    }
    while (n > 0) {
        buffer[i++] = (char)('0' + (n % 10));
        n /= 10;
    }
    // Reverse in place for printing
    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - 1 - j];
        buffer[i - 1 - j] = temp;
    }
    buffer[i] = '\n';
    write(1, buffer, i + 1);
}

// Inline max/min to avoid branching/calls if possible by compiler
static inline uint64_t max_u64(uint64_t a, uint64_t b) { return (a > b) ? a : b; }
static inline uint64_t min_u64(uint64_t a, uint64_t b) { return (a < b) ? a : b; }

int main() {
    int fd = open(INPUT_FILE, O_RDONLY);
    if (fd == -1) {
        // Fail fast and hard
        perror("Error opening file");
        return 1;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) return 1;
    size_t fsize = sb.st_size;

    // mmap the entire file
    // MAP_POPULATE pre-faults the pages to avoid page faults during processing
    const uint8_t *data = (const uint8_t *)mmap(NULL, fsize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    if (data == MAP_FAILED) return 1;
    
    // Hint to OS that we will read sequentially
    madvise((void*)data, fsize, MADV_SEQUENTIAL);

    const uint8_t *p = data;
    const uint8_t *end = data + fsize;

    unsigned __int128 total_invalid_sum = 0;

    // Parser State
    // We expect format: L-R,L-R,L-R...
    // Numbers are uint64_t.
    
    while (p < end) {
        uint64_t start = 0;
        uint64_t end_range = 0;

        // Parse Start (consume until '-')
        // Unrolled fast path for parsing integers
        while (p < end && *p >= '0') {
            start = start * 10 + (*p - '0');
            p++;
        }
        
        // Skip separator ('-')
        if (p < end) p++;

        // Parse End (consume until ',' or newline or EOF)
        while (p < end && *p >= '0') {
            end_range = end_range * 10 + (*p - '0');
            p++;
        }

        // Skip separator (',') or newline
        if (p < end) p++;

        // PROCESS RANGE [start, end_range]
        // An invalid ID is of form S + S.
        // If S has k digits, ID = S * (10^k + 1).
        // ID has 2k digits.
        // We iterate k from 1 up to 9 (since 10^18 is max u64 roughly, 2k=18 -> k=9).
        // Input ranges might exceed u64 if problem was evil, but typical AoC is u64.
        
        for (int k = 1; k <= 9; k++) {
            uint64_t p10_k = P10[k];
            uint64_t multiplier = p10_k + 1; // 10^k + 1

            // Determine bounds for the seed S.
            // S must have exactly k digits: [10^(k-1), 10^k - 1]
            uint64_t lower_digit_bound = P10[k-1];
            uint64_t upper_digit_bound = p10_k - 1;

            // Determine bounds for S based on the numeric range [start, end_range]
            // We need: start <= S * multiplier <= end_range
            // S >= ceil(start / multiplier)
            // S <= floor(end_range / multiplier)
            
            uint64_t min_s_val = (start + multiplier - 1) / multiplier;
            uint64_t max_s_val = end_range / multiplier;

            // Intersect [lower_digit_bound, upper_digit_bound] with [min_s_val, max_s_val]
            uint64_t effective_start = max_u64(lower_digit_bound, min_s_val);
            uint64_t effective_end   = min_u64(upper_digit_bound, max_s_val);

            if (effective_start <= effective_end) {
                // We have a valid range of seeds S [effective_start, effective_end].
                // We need to sum the generated IDs: sum( S * multiplier )
                // = multiplier * sum(S)
                // sum(S) for S in [A, B] is (A + B) * count / 2
                
                unsigned __int128 count = (unsigned __int128)(effective_end - effective_start + 1);
                unsigned __int128 sum_s = ((unsigned __int128)effective_start + effective_end) * count / 2;
                
                total_invalid_sum += (sum_s * multiplier);
            }
        }
    }

    print_u128(total_invalid_sum);

    munmap((void*)data, fsize);
    close(fd);
    return 0;
}
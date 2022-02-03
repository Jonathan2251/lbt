// Cpu0 does not optimize builtins in asm.

// In C, the builtins listed in the file are critical and may used in boot code.
// https://stackoverflow.com/questions/20892847/my-attempt-to-optimize-memset-on-a-64bit-machine-takes-more-time-than-standard-i
// https://lists.rtems.org/pipermail/vc/2018-June/014412.html

// That is, memset(b, c, l) set the l bytes starting at address b to the value c. 
// It just does it much faster than in the following implementation.
// https://stackoverflow.com/questions/13327155/memset-definition-and-use

#include <string.h>

void* internal_memset(void* b, int c, size_t len) {
    char* p = (char*)b;
    for (size_t i = 0; i != len; ++i) {
        p[i] = c;
    }
    return b;
}


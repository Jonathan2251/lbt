
/// start
#ifndef _PRINT_H_
#define _PRINT_H_

// defined in cpu0.v
// cpu032I memory limit, jsub:24-bit
#define IOADDR 0x1000000

void print_char(const char c);
void dump_mem(unsigned char *str, int n);
void print_string(const char *str);
void print_integer(int x);
#endif

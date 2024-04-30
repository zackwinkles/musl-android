//#define __malloc_donate(a,b,c) do {;} while (0)

#include <sys/types.h>
#include <features.h>

hidden inline const void __malloc_donate(unsigned char *, size_t) { ; }

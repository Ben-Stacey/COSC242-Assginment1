#ifndef ASGN1_MYLIB_H
#define ASGN1_MYLIB_H

#include <stdbool.h>

extern void*    emalloc(size_t s);
extern int      getword(char *s, int limit, FILE *stream);
static bool     isPrime(int n);
extern int      nextPrime(int n);
extern char*    ft_strdup(char *src);

#endif
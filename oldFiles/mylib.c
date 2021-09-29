#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "mylib.h"

/**
 *
 * @param s
 * @param limit
 * @param stream
 * @return
 */
int getword(char *s, int limit, FILE *stream) {
    int c;
    char *w = s;
    assert(limit > 0 && s != NULL && stream != NULL);
    /* skip to the start of the word */
    while (!isalnum(c = getc(stream)) && EOF != c);
    if (EOF == c) {
        return EOF;
    } else if (--limit > 0) { /* reduce limit by 1 to allow for the \0 */
        *w++ = tolower(c);
    }
    while (--limit > 0) {
        if (isalnum(c = getc(stream))) {
            *w++ = tolower(c);
        } else if ('\'' == c) {
            limit++;
        } else {
            break;
        }
    }
    *w = '\0';
    return w - s;
}

/**
 *
 * @param s - the size we wish to memory allocate
 * @return void* pointer to the allocated memory
 */
void* emalloc(size_t s) {
    void *result = malloc(s);
    if (NULL == result) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/**
 * In case we need to implement our own strdup()
 * @param src
 * @return
 */
char *ft_strdup(char *src)
{
    char *str;
    char *p;
    int len = 0;

    while (src[len])
        len++;
    str = malloc(len + 1);
    p = str;
    while (*src)
        *p++ = *src++;
    *p = '\0';
    return str;
}

static bool isPrime(int n) {
    int i = 0;
    if (n%2 == 0 || n%3 == 0 || n <= 1) return false;
    for (i=5; i*i<=n; i=i+6)
        if (n%i == 0 || n%(i+2) == 0)
            return false;
    return true;
}

int nextPrime(int n) {
    bool found = false;
    int prime = n;
    /* Loop continuously until isPrime returns
     * true for a number greater than n */
    while (!found) {
        prime++;
        if (isPrime(prime))
            found = true;
    }
    return prime;
}
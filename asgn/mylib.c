#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

/**  "Safer" memory allocation for arrays
 *
 * @param s - the size we wish to memory allocate
 * @return result - void* pointer to the allocated memory
 */
void *emalloc(size_t s){
    void *result = malloc(s);
    if (NULL == result){
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/** resizes a block of memory that has already been 
 * allocated
 * 
 * @param p - is the value being resized
 * @param s - the size we wish to memroy allocate
 * @return result - void* pointer to the allocated memory
 */
void *erealloc(void *p, size_t s){
    void *result = realloc(p, s);
    if (NULL == result){
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/** 
 *
 * @param s - the word 
 * @param limit - the limit that it is searching til
 * @param stream - the file its writing too
 * @return w - s - this return the word
 */
int getword(char *s, int limit, FILE *stream) {
    int c;
    char *w = s;
    assert(limit > 0 && s != NULL && stream != NULL);
    /* skip to the start of the word */
    while (!isalnum(c = getc(stream)) && EOF != c)
        ;
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
 * is called by nextPrime(). This method
 * contains the algorithm for fining
 * prime numbers
 * 
 * @param n - the value passed
 * @return true - if it is prime
 */
static bool isPrime(int n) {
    int i = 0;
    if (n%2 == 0 || n%3 == 0 || n <= 1){
         return false;
    }    
    for (i=5; i*i<=n; i=i+6){
        if (n%i == 0 || n%(i+2) == 0){
            return false;
        }
    }
    return true;
}

/**
 * looks for the next prime number
 * by calling isPrime()
 * 
 * @param n - the value we are looking for
 * @return prime - the prime number
 */
int nextPrime(int n) {
    bool found = false;
    int prime = n;
    /* Loop continuously until isPrime returns
     * true for a number greater than n */
    while (!found) {
        prime++;
        if (isPrime(prime)){
            found = true;
        }
    }
    return prime;
}



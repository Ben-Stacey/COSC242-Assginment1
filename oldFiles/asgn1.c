#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>
#include "mylib.h"
#include "htable.h"


/**
 * We pass this function to htable_print.
 * @param freq
 * @param word
 */
static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
}       

/**
 *
 * @param argc - count of args
 * @param argv - the vector of args
 * @return - EXIT_SUCCESS or EXIT_FAILTURE
 */

int main(int argc, char *argv[]) {
    FILE *infile = NULL;
    char word[256];
    const char *optstring = "deps:t:h";
    char *tval, *sval = NULL;
    char option;
    int i = 0;
    /* Default values */
    hashing_t hash_strategy = LINEAR_P;
    int tablesize = TABLE_SIZE;
    htable h = NULL;

    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            /* Use double hashing as the collision resolution strategy */
            case 'd': {
                printf("Option d\n");
                hash_strategy = DOUBLE_H;
                break;
            }
            /*
                Display entire contents of hash table on stderr using the format
                string ("%5d␣%5d␣%5d␣␣␣%sn" to display the index, frequency,
                stats, and the key if it exists. (Note that spaces have been made
                visible in the format string so you can see how many there are)
            */
            case 'e': {
                htable_print_entire_table(h);
                break;
            }
            /*
             * Print stats info using the functions provided in print-stats.txt
                instead of printing the frequencies and words
            */
            case 'p': {
                /*htable_print_stats(h, infile, sval); //need to find the int that goes where sval is. the int is the size of the snapshot*/
                break;
            }
            /*
             * Display up to the given number of stats snapshots when given -p
                as an argument. If the table is not full then fewer snapshots will
                be displayed. Snapshots with 0 entries are not shown
             */
            case 's': {
                sval = optarg; 
                if((int)sval == NULL){
                    break;
                }else if((int) sval == tablesize){
                    htable_print_stats(h, infile,  (int) sval);
                    break;
                }else if((int) sval < tablesize){
                    htable_print_stats(h, infile,  (int) sval);
                    break;
                }
                
            }
            /* Use the first prime >= tablesize as the size of your hash table.
                You can assume that tablesize will be a number greater than 0. */
            case 't': {
                /* Create a function to calculate the first prime less than tablesize. */
                printf("Option t\n");
                tval = optarg;
                /* CONVERT tval to int from char* first */
                /* use strtol() */
                tablesize = nextPrime((int) tval);
                printf("The tval: %i\n", tablesize);
                break;
            }
            /* Print a help message describing how to use the program */
            case 'h': {
                printf("Options:\n-h\t\t|\t Print this help\n");
                printf("-d\t\t|\t Use double hashing as the collision resolution strategy\n");
                printf("-e\t\t|\t Display entire contents of hash table on stderr\n");
                printf("-p\t\t|\t Print stats info using the functions provided in print-stats.txt\n"
                       "\t\t|\t instead of printing the frequencies and words\n");
                printf("-s [snapshots]\t|\t Display up to the given number of stats snapshots when given -p\n"
                       "\t\t|\t as an argument.\n");
                printf("-t [tablesize]\t|\t Use the first prime >= tablesize as the size of your hash table.\n");
                break;
            }
            /* No default.  */
        }
    }
    /* Initialize hash-table */
    h = htable_new(TABLE_SIZE, DOUBLE_H );
    /* Insert into hash table in loop */
    while (getword(word, sizeof word, stdin) != EOF) {
        if (htable_insert(h, word) == -1) {
            printf("Hash table is full");
            htable_free(h);
        }

        /* For demonstration purposes, print entire table every 5 inserts.*/
        if (i % 5 == 0) {
            htable_print_entire_table(h);
        }
        i++;
    }
    htable_free(h);

    return EXIT_SUCCESS;
}
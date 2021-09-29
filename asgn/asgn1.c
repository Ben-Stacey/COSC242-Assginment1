#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "htable.h"
#include <getopt.h>
#include <stdbool.h>

/** this method prints the info
 * 
 * @param freq - frequency to print
 * @param word - the string
 */
static void print_info(int freq, char *word) {
   printf("%-4d %s\n", freq, word);
}

/** Program entrypoint, main method that contains switches
 * for running the program and dealing with the command line input.
 * Each different hashing strategy and the way it is printed 
 * is done with the switch. Memory is then freed at the end of 
 * the method.
 * Case "d" will call the double hashing strategy.
 * Case "e" will display the entire contents of the htable.
 * Case "p" prints out function from print-stats.txt
 *      instead of the freqs and words
 * Case s prints out a snapshot of the upto a given
 *      argument.
 * Case "t" to find the new size of the htable
 * Case "h" will display help information,
 *      also will display when an invalid
 *      param is entered
 * At the bottom of the method it then calls
 * the correct htable or htable stats
 * that was requested through the command line
 * 
 * @param argc - value read in through command line
 * @param argv - value read in through command line
 * @return EXIT_SUCCESS - if there is n problems
 */
int main(int argc, char *argv[]){
    int echoice = 0, pchoice = 0, schoice = 0, hchoice = 0, invalid = 0;
    int num_stats = 0;
    char word[256];
    int tablesize = TABLE_SIZE;
    hashing_t hash_type = LINEAR_P;
    htable h = NULL;
    char option;
    const char *optstring = "deps:t:h";

    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'd':
                hash_type = DOUBLE_H;
                break;
            case 'e':
                echoice = 1;
                break;
            case 'p':
                pchoice = 1;
                break;
            case 's':
                schoice = 1;
                num_stats = atoi(optarg);
                break;
            case 't':
                tablesize = nextPrime(atoi(optarg));
                break;
            case 'h':
                hchoice = 1;
                printf("Usage: ./asgn1 [OPTION]... <STDIN>\n\n");
                printf("Perform various operations using a hash table. "
                "By default, words are\nread from stdin and added to the " 
                "hash table, before being printed out\nalongside their "
                "frequencies to stdout.\n\n");
                printf("-d\t\tUse double hashing (linear probing is the "
                "default)\n");
                printf("-e\t\tDisplay entire contents of hash table on "
                "stderr\n");
                printf("-p\t\tPrint stats info instead of frequencies & "
                "words\n");
                printf("-s  SNAPSHOTS   Show SNAPSHOTS stats snapshots (if "
                "-p is used)\n");
                printf("-t  TABLESIZE   Use the first prime >= TABLESIZE as "
                "htable size\n\n");
                printf("-h\t\tDisplay this message\n\n");
                break;
            default:
                invalid = 1;
                printf("Usage: ./asgn1 [OPTION]... <STDIN>\n\n");
                printf("Perform various operations using a hash table. "
                "By default, words are\nread from stdin and added to the " 
                "hash table, before being printed out\nalongside their "
                "frequencies to stdout.\n\n");
                printf("-d\t\tUse double hashing (linear probing is the "
                "default)\n");
                printf("-e\t\tDisplay entire contents of hash table on "
                "stderr\n");
                printf("-p\t\tPrint stats info instead of frequencies & "
                "words\n");
                printf("-s  SNAPSHOTS   Show SNAPSHOTS stats snapshots (if "
                "-p is used)\n");
                printf("-t  TABLESIZE   Use the first prime >= TABLESIZE as "
                "htable size\n\n");
                printf("-h\t\tDisplay this message\n\n");
        }
    }
    
    h = htable_new(tablesize, hash_type);

    while (getword(word, sizeof word, stdin) != EOF){
        htable_insert(h, word);
    }

    if (echoice == 1){
        htable_print_entire_table(h);
    }

    if(pchoice == 1){
        if(schoice == 0){
            htable_print_stats(h, stdout, 10);
        }
    }

    if(schoice == 1 && pchoice == 1){
       htable_print_stats(h, stdout, num_stats);
    }

    if (schoice == 1 && pchoice == 0){
        htable_print(h, print_info);
    }

    if (invalid == 0){
        if (hchoice == 0){
            if (pchoice == 0 && schoice == 0){
                htable_print(h, print_info);
            }
        }
    }

    htable_free(h);

    return EXIT_SUCCESS;
}

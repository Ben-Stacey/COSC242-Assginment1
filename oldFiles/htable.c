#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mylib.h"
#include "htable.h"

struct htablerec {
    int capacity;
    int num_keys;
    char **keys;
    int *freqs;
    unsigned int *stats;
    hashing_t method;
};

/**
 *
 * @param h - the hashtable
 * @param i_key - the index key
 * @return - the resultant hash
 */
static int htable_step(htable h, unsigned int i_key) {
    return 1 + (i_key % (h->capacity - 1));
}

/**
 * This is the primary hashing function
 * @param word - input word to hash
 * @return - our resultant hash
 */
static int htable_word_to_int(char *word) {
    unsigned int result = 0;

    while (*word != '\0') {
        result = (*word ++ + 31 * result);
    }
    return result;
}

/*
 *  Our insertion function, where we insert into the hashtable.
 *  Using the default linear probing strategy.
 *
 *  The program will use the word as the "key" in the hash table
 *  And the value will be the "frequency". If the key already exists,
 *  Simply increment the corresponding frequency for that key.
 *
 * */

/**
 * Insert the word into our frequency hash table
 * @param h - our hashtable
 * @param str - the word to insert
 * @return - the position we inserted the word.
 */
int htable_insert(htable h, char *str) {
    int i, try, collisions;
    int index = htable_word_to_int(str);
    printf("Insert word: %s\n", str);
    if (h->num_keys == TABLE_SIZE) {
        return -1;
    }
    collisions = 0;
    for (i = 0; i < TABLE_SIZE; i++) {
        if (i > 0 && (h->method == (hashing_t) DOUBLE_H)) {
            try = (htable_step(h, i) + index % TABLE_SIZE);
        }   else {
            try = (i + index) % TABLE_SIZE;
        }
        if (h->keys[try] == NULL) {
            printf("Create hash entry\n");
            h->keys[try] = ft_strdup(str);
            h->freqs[try] = 1;
            h->stats[h->num_keys] = collisions;
            h->num_keys++;
            return try;
        }   else if (strcmp(h->keys[try], str) == 0) {
            printf("Increment the frequency value by 1 for key.\n");
            h->freqs[try]++;
            return try;
        }   else {
            printf("Collision occurred & keys are not equal\n");
            collisions++;
        }
    }
    return 0;
}

/**
 * Create a new hash table given the collision
 * resolution strategy desired as input.
 */
htable htable_new(int capacity, hashing_t collision_resolution_strategy) {
    int i;
    /* allocate the table */
    htable hashtable = emalloc(sizeof(htable) * 1);
    /* allocate the table entries */
    hashtable->method = (hashing_t) collision_resolution_strategy;
    hashtable->num_keys = 0;
    hashtable->freqs = emalloc(sizeof(unsigned int) * capacity);
    hashtable->stats = emalloc(sizeof(int));

    hashtable->keys = emalloc(sizeof(char *)* capacity);
    /* set each to null (needed for proper operation) */
    for (i = 0; i < TABLE_SIZE; ++i) {
        hashtable->keys[i] = NULL;
    }

    return hashtable;
}

/*
 * Delete the hash table in its entirety
 * Deallocate memory for keys, freqs, stats, method etc.
 * Free the memory based on num_keys in the datastructure.
 * Delete in reverse order of creation.
 * */

/**
 * Delete the hashtable in its entirety
 * Deallocate memory for the table and all the keys & freqs
 * @param h - the hashtable
 * @return void.
 */

void htable_free(htable h) {
    int i = 0;
    /* 1 - free stats */
    free(h->stats);
    /* 2 - free freqs */
    free(h->freqs);
    /* 3 - free keys (we can do this without a loop.) */
    for (i = 0; i < h->num_keys; i++ ) {
        if (h->keys[i] != NULL) {
            free(h->keys[i]);
        }
    }
    free(h->keys);
    /* 4 - free structure */
    free(h);
}

/**
 * Prints out a line of data from the hash table to reflect the state
 * the table was in when it was a certain percentage full.
 * Note: If the hashtable is less full than percent_full then no data
 * will be printed.
 *
 * @param h - the hash table.
 * @param stream - a stream to print the data to.
 * @param percent_full - the point at which to show the data from.
 */
static void print_stats_line(htable h, FILE *stream, int percent_full) {
    int current_entries = h->capacity * percent_full / 100;
    double average_collisions = 0.0;
    int at_home = 0;
    unsigned int max_collisions = 0;
    int i = 0;

    if (current_entries > 0 && current_entries <= h->num_keys) {
        for (i = 0; i < current_entries; i++) {
            if (h->stats[i] == 0) {
                at_home++;
            }
            if (h->stats[i] > max_collisions) {
                max_collisions = h->stats[i];
            }
            average_collisions += h->stats[i];
        }

        fprintf(stream, "%4d %10d %11.1f %10.2f %11d\n", percent_full,
                current_entries, at_home * 100.0 / current_entries,
                average_collisions / current_entries, max_collisions);
    }
}

/**
 * Prints out a table showing what the following attributes were like
 * at regular intervals (as determined by num_stats) while the
 * hashtable was being built.
 *
 * @li Percent At Home - how many keys were placed without a collision
 * occurring.
 * @li Average Collisions - how many collisions have occurred on
 *  average while placing all of the keys so far.
 * @li Maximum Collisions - the most collisions that have occurred
 * while placing a key.
 *
 * @param h the hashtable to print statistics summary from.
 * @param stream the stream to send output to.
 * @param num_stats the maximum number of statistical snapshots to print.
 */

void htable_print_stats(htable h, FILE *stream, int num_stats) {
    int i;

    fprintf(stream, "\n%s\n\n",
            h->method == LINEAR_P ? "Linear Probing" : "Double Hashing");
    fprintf(stream, "Percent   Current    Percent    Average      Maximum\n");
    fprintf(stream, " Full     Entries    At Home   Collisions   Collisions\n");
    fprintf(stream, "------------------------------------------------------\n");
    for (i = 1; i <= num_stats; i++) {
        print_stats_line(h, stream, 100 * i / num_stats);
    }
    fprintf(stream, "------------------------------------------------------\n\n");
}

void htable_print_entire_table(htable h) {
    /*
    * use the format string "\%5d␣\%5d␣\%5d␣␣␣\%s\n" to print
    each line (spaces have been made visible so you can count them).
     */
    int i = 0;
    for (i = 0; i < TABLE_SIZE; i++) {
        /*if (h->keys[i] == NULL) {
            printf("Key: %i: -------------\n", i);
        } else {*/
            printf("\%5d \%5d \%5d   \%s\n", i, h->freqs[i], h->stats[i], h->keys[i]);
        /*}*/
    }
 }
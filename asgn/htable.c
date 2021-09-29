#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "htable.h"
#include <string.h>
#include <stdbool.h>

/*
 * The hash data structure that
 * will be used throughout the program
*/
struct htablerec{
    int cap;
    int num_keys;
    char **keys;
    int *freqs;
    int *stats;
    hashing_t method;
};

/**
 * Create a new hash table given the collision
 * resolution strategy desired as input.
 * 
 * @param c - the capacity of the htable
 * @return h - the new htable
 */
htable htable_new(int c, hashing_t hash_type){
    int i, j, k;
    htable h = emalloc(sizeof *h);
    h->cap = c;
    h->num_keys = 0;
    h->keys = emalloc(h->cap * sizeof h->keys[0]);
    h->freqs = emalloc(h->cap * sizeof h->freqs[0]);
    h->stats = emalloc(h->cap * sizeof h->stats[0]);
    for (i = 0; i < c; i++) h->keys[i] = NULL;
    for (j = 0; j < c; j++) h->freqs[j] = 0;
    for (k = 0; k < c; k++) h->stats[k] = 0;
    h->method = hash_type;
    return h;
}

/**
 * Frees the table memory once it
 * is no longer needed 
 * 
 * @param h - the htable
 */
void htable_free(htable h){
    int i;
    for (i = 0; i < h->cap; i++){
        if (h->keys[i] == NULL){
            continue;
        }
        free(h->keys[i]);
    }
    free(h->keys);
    free(h->freqs);
    free(h->stats);
    free(h);
}

/**
 * his method steps through the
 * htable
 * @param h - the htable
 * @param i_key - the key value
 * @return - the value stepped up once
 */
static unsigned int htable_step(htable h, unsigned int i_key){
    return 1 + (i_key % (h->cap - 1));
}

/**
 * This is the primary hashing function
 * (linear probing)
 * @param word - input word to hash
 * @return result - our resultant hash
 */
static unsigned int word_to_int(char *word) {
    unsigned int result = 0;
    while (*word != '\0') {
        result = (*word++ + 31 * result);
    }
    return result;
}

/**  Our insertion function, where we insert into the hashtable.
 *  Using the default linear probing strategy.
 *  The program will use the word as the "key" in the hash table
 *  And the value will be the "frequency". If the key already exists,
 *  Simply increment the corresponding frequency for that key.
 * 
 * @param h - our hashtable
 * @param str - the word to insert
 * @return 0 - the position we inserted the word.
 */
int htable_insert(htable h, char *key){
    unsigned int x, index, step;
    int i = 0;
    x = word_to_int(key);
    index = x % h->cap;
    step = htable_step(h, x);
    while (i < h->cap){
    if (h->keys[index] == NULL){
        h->keys[index] = emalloc((strlen(key) + 1) * sizeof(key[0]));
        strcpy(h->keys[index], key);
        h->freqs[index]++;
        h->num_keys++;
        return 1;
    }else if (strcmp(h->keys[index], key) == 0){
        h->freqs[index]++;
        return h->freqs[index];
    }
    if (h->method == (hashing_t) LINEAR_P){
        index = (index + 1) % h->cap;
    }else{
        index = (index + step) % h->cap;
    }
    i++;
    h->stats[h->num_keys]++;
    }
    
    return 0;
}

/**
 * Prints out the htable
 * @param h - the hashtable datastructure
 * @param stream - the file print too
 */
void htable_print(htable h, void (*print_info)(int, char*)){
    int i;
    for (i = 0; i < h->cap; i++){
        if (h->keys[i] != NULL){
            (*print_info)(h->freqs[i], h->keys[i]);
        }
    }
}

/** Use the format string "\%5d␣\%5d␣\%5d␣␣␣\%s\n" to print 
 * each line (spaces have been made visible so you can count them).
 * 
 * @param h - the htable
 */
void htable_print_entire_table(htable h){
    int i;
    printf("Pos  Freq  Stats  Word\n");
    printf("-------------------------------------------\n");
    for (i = 0; i < h->cap; i++){
        printf("%5d %5d %5d   %s\n", i, h->freqs[i], h->stats[i], h->keys[i]);
    }
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
     int current_entries = h->cap * percent_full / 100;                                                                                  
     double average_collisions = 0.0;                                                                                                         
     int at_home = 0;                                                                                                                         
     int max_collisions = 0;                                                                                                                  
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

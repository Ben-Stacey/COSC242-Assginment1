#ifndef ASIGN1_HTABLE_H
#define ASIGN1_HTABLE_H

#define TABLE_SIZE 113

typedef enum hashing_e { LINEAR_P, DOUBLE_H } hashing_t;

typedef struct htablerec *htable;

extern void         htable_free(htable h);
extern int          htable_insert(htable h, char *str);
extern htable       htable_new(int capacity, hashing_t collision_resolution_strategy);
extern void         htable_print(void (*print_info));
extern void         htable_print_entire_table(htable h);
extern int          htable_search(htable h, char *str);
extern void         htable_print_stats(htable h, FILE *stream, int num_stats);
static void         print_stats_line(htable h, FILE *stream, int percent_full);
static int          htable_word_to_int(char *word);
static int          htable_step(htable h, unsigned int i_key);

#endif
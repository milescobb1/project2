#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct occurence Occurence;

typedef struct hashtable HashTable;

#define TABLESIZE 1000

struct occurence {
    char *word;
    int frequency;
};

struct hashtable {
    Occurence *table[TABLESIZE];
    int size;
    int items;
};

void open_files(int argc, char *argv[], FILE **files);

void get_line(FILE *file, char *line);

void add_word(char *word, HashTable hash_table);

HashTable init_table();

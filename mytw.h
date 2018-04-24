#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

typedef struct occurrence Occurrence;

typedef struct hashtable HashTable;

#define TABLESIZE 5

struct occurrence {
    char *word;
    int frequency;
};

struct hashtable {
    Occurrence **table;
    int size;
    int items;
};

unsigned long hash (const char* word);

void rehash(HashTable *hash_table);

int parse_input(int argc, int *num_files, int *n, char *argv[], FILE **files);

void open_files(int *num_files, int start, int end, char *argv[], FILE **files);

char *get_line(FILE *file, char *line);

void get_words(char *line, HashTable* hash_table);

void add_word(char *word, unsigned long hash_code, HashTable *hash_table);

HashTable init_table();

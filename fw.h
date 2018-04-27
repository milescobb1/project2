#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

typedef struct occurrence Occurrence;

typedef struct hashtable HashTable;

#define TABLESIZE 100000

struct occurrence {
    char *word;
    int frequency;
};

struct hashtable {
    Occurrence **table;
    int size;
    int items;
};

int partition( Occurrence *a[], int l, int r);

void quickSort( Occurrence *a[], int l, int r);

unsigned long hash (const char* word);

void rehash(HashTable *hash_table);

int parse_input(int argc, char *argv[], HashTable *hash_table);

void open_files(int start, int end, char *argv[], HashTable *hash_table);

char *get_line(FILE *file, char *line);

void get_words(char *line, HashTable* hash_table);

void add_word(char *word, int frequency, unsigned long hash_code, HashTable *hash_table);

HashTable init_table();

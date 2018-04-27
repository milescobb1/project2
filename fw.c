#include "fw.h"


void quick_sort(Occurrence *a[], int l, int r) {
    int j;
    if(l < r) {
        j = partition(a, l, r);
        quick_sort(a, l, j - 1);
        quick_sort(a, j + 1, r);
    }
}

int compare(Occurrence *a, Occurrence *b) {
    int freq = 0;
    freq = a->frequency - b->frequency;
    return(freq ? freq : strcmp(a->word, b->word));
}

int partition(Occurrence *a[], int l, int r) {
    int i, j;
    Occurrence *t, *pivot;
    pivot = a[l];
    i = l - 1; j = r + 1;
    while(1) {
        do ++i; while(i <= r && compare(a[i], pivot) >= 0);
        do --j; while(compare(a[j], pivot) < 0);
        if(i >= j) {
            break;
        }
        t = a[i]; a[i] = a[j]; a[j] = t;
    }
    t = a[l]; a[l] = a[j]; a[j] = t;
    return j;
}

HashTable init_table() {
    int i;
    HashTable hash_table;
    hash_table.size = TABLESIZE;
    hash_table.items = 0;
    hash_table.table = (Occurrence**)malloc(sizeof(Occurrence) * TABLESIZE);
    for(i = 0; i < TABLESIZE; i++) {
        hash_table.table[i] = NULL;
    }
    return hash_table;
}


void add_word(char *word, int frequency, unsigned long hash_code, HashTable *hash_table) {
    unsigned long index;
    Occurrence *new_word = NULL;
    hash_code = hash_code % hash_table->size;
    index = hash_code;
    if(hash_table->items / ((double)hash_table->size) > 0.75){
        rehash(hash_table);
    }
    while(hash_table->table[hash_code]) {
        if (!strcmp(hash_table->table[hash_code]->word, word)){
            (hash_table->table[hash_code])->frequency += 1;
            return;
        }
        else {
            hash_code++;
            if(hash_code == index) {
                return;
            }
        }
    }
    hash_table->items++;
    new_word = (Occurrence *)malloc(sizeof(Occurrence));
    new_word->word = (char *)malloc(sizeof(char) * strlen(word) + 1); 
    strcpy(new_word->word, word);
    new_word->frequency = frequency;
    hash_table->table[hash_code] = new_word;
}

void rehash(HashTable *hash_table) {
    int i;
    int old_size;
    Occurrence **newTable = (Occurrence**)malloc(sizeof(Occurrence) * hash_table->size * 2);
    Occurrence **temp = hash_table->table;
    hash_table->table = newTable;
    hash_table->items = 0;
    old_size = hash_table->size;
    hash_table->size *= 2;
    for(i = 0; i < hash_table->size; i++) {
        hash_table->table[i] = NULL;
    }
    for(i = 0; i < old_size; i++) {
        if(temp[i]) {
            add_word(temp[i]->word, temp[i]->frequency, hash(temp[i]->word), hash_table);
            free(temp[i]->word);
        }
        free(temp[i]);
    }
    free(temp);
}

/* This is a horrific function */
int parse_input(int argc, int *num_files, int *n, char *argv[], FILE **files) {
    int i;
    if(argc > 1) {
        if(!strcmp(argv[1], "-n")) {
            if(argc > 2) {
                for(i = 0; i < strlen(argv[2]); i++) {
                    if (isdigit(argv[2][i])) {
                        (*n) += atoi(&argv[2][i]);
                    }
                    else {
                        fprintf(stderr, "usage: fw [-n num] [ file1 [ file2 [...] ] ]\n");
                        exit(0);
                        return -1;
                    }
                }
                if(argc > 3) {
                    return 3;
                }
                else {
                    return 0;
                }
            }
            else {
                fprintf(stderr, "usage: fw [-n num] [ file1 [ file2 [...] ] ]\n");
                exit(0);
                return -1;
            }
        }
        else {
            *n = 10;
            return 1;
        }
    }
    *n = 10;
    return 0;
}

void open_files(int *num_files, int start, int end, char *argv[], FILE **files) {
    int i;
    FILE *new_file = NULL;
    for(i = 0; i < end - start; i++) {
        new_file = fopen(argv[i + start], "r");
        if(new_file) {
            files[(*num_files)++] = new_file;
        }
        else {
            fprintf(stderr, "%s: %s\n", argv[i + start], strerror(errno));
        }
    }
}

char *get_line(FILE *file, char *line) {
    char *new_line = NULL;
    int size = 100;
    int c = 0;
    int i = 0;
    while((c = getc(file)) != EOF) {
        line[i++] = tolower(c);
        if(c == '\n') {
            break;
        }
        if(i == size - 2) {
            size *= 2;
            new_line = (char *)realloc(line, sizeof(char) * size);
            if(new_line == NULL) {
                break;
            }
            line = new_line;
        }
    }
    line[i] = '\0';
    return line;
}

void get_words(char *line, HashTable *hash_table) {
    int i;
    int j;
    char *current_word = (char *)malloc(sizeof(char) * strlen(line));
    for(i = 0, j = 0; i < strlen(line); i++) {
        if(isalpha(line[i])) {
            current_word[j] = line[i];
            j++;
        }
        else {
            current_word[j] = '\0';
            if(current_word[0] != '\n' && current_word[0] != '\0' && current_word[0] != 0 && strlen(current_word)) {
                add_word(current_word, 1, hash(current_word), hash_table);
            }
            memset(current_word, 0, sizeof(char) * strlen(line));
            j = 0;
        }
    }
    current_word[j] = '\0';
    if(current_word[0] != '\n' && current_word[0] != '\0' && current_word[0] != 0 && strlen(current_word)) {
        add_word(current_word, 1, hash(current_word), hash_table);
    }
    free(current_word);
}

unsigned long hash (const char* word) {
    int i;
    unsigned long hash = 1;
    for (i = 0 ; word[i] != '\0' ; i++)
    {
        hash = 31 * hash + word[i];
    }
    return hash;
} 

int main(int argc, char *argv[]) {
    int i;
    int p;
    int n = 0;
    int num_files = 0;
    int input;
    Occurrence **occurrences = NULL;
    HashTable hash_table = init_table();
    FILE *new_file = NULL;
    FILE **files = (FILE **)malloc(sizeof(FILE) * (argc - 1));
    char *line = (char *)malloc(sizeof(char) * 100);
    input = parse_input(argc, &num_files, &n, argv, files);
    if(input > 0) {
        for(i = input; i < argc; i++) {
            new_file = fopen(argv[i], "r");
            if(new_file) {
                while((line = get_line(new_file, line)) && line[0] != '\0') {
                    get_words(line, &hash_table);
                    memset(line, 0, sizeof(char) * 100);
                }
                fclose(new_file);
            }
            else {
                fprintf(stderr, "%s: %s\n", argv[i], strerror(errno));
            }
        }
    }
    else {
        while((line = get_line(stdin, line)) && line[0] != '\0') {
            get_words(line, &hash_table);
            memset(line, 0, sizeof(char) * 100);
        }
    }
    free(files);
    free(line);
    occurrences = (Occurrence**)malloc(sizeof(Occurrence) * hash_table.items);
    for(i = 0, p = 0; i < hash_table.size; i++) {
        if(hash_table.table[i]) {
            occurrences[p] = hash_table.table[i];
            p++;
        }
    }
    quick_sort(occurrences, 0, hash_table.items - 1);
    printf("The top %d words (out of %d) are:\n", n, hash_table.items);
    new_file = fopen("output.txt", "w");
    for(i=0; i < n && i < hash_table.items; i++) {
        printf("%9d %s\n", occurrences[i]->frequency, occurrences[i]->word);
        fprintf(new_file, "%s\n", occurrences[i]->word);
    }
    fclose(new_file);
    for(i = 0; i < hash_table.size; i++) {
        if(hash_table.table[i]) {
            free(hash_table.table[i]->word);
            free(hash_table.table[i]);
        }
    }
    free(occurrences);
    free(hash_table.table);
    return 0;
}

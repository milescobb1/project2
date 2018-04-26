#include "mytw.h"

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


void add_word(char *word, unsigned long hash_code, HashTable *hash_table) {
    unsigned long index;
    Occurrence *new_word = NULL;
    hash_code = hash_code % hash_table->size;
    index = hash_code;
    hash_table->items++;
    new_word = (Occurrence *)malloc(sizeof(Occurrence));
    new_word->word = (char *)malloc(sizeof(char) * strlen(word) + 1); 
    strcpy(new_word->word, word);
    new_word->frequency = 1;
    if(hash_table->items / ((double)hash_table->size) > .33){
        rehash(hash_table);
    }
    while(hash_table->table[hash_code]) {
        if (!strcmp(hash_table->table[hash_code]->word, word)){
            hash_table->table[hash_code]->frequency += 1;
            free(new_word->word);
            free(new_word);
            return;
        }
        else {
            hash_code++;
            if(hash_code == index) {
                hash_table->items--;
                return;
            }
        }
    }
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
            add_word(temp[i]->word, hash(temp[i]->word), hash_table);
            free(temp[i]->word);
        }
        free(temp[i]);
    }
    free(temp);
}

int parse_input(int argc, int *num_files, int *n, char *argv[], FILE **files) {
    int i;
    if(argc > 1) {
        if(!strcmp(argv[1], "-n")) {
            for(i = 0; i < strlen(argv[2]); i++) {
                if (isdigit(argv[2][i])) {
                    (*n) += atoi(&argv[2][i]);
                }
                else {
                    fprintf(stderr, "Usage: mytw -n <num> file1 [file2...]\n");
                    exit(0);
                    return -1;
                }
                if(argc > 2) {
                    open_files(num_files, 3, argc, argv, files);
                    return 1;
                }
                else {
                    return 0;
                }
            }
        }
        else {
            *n = 10;
            if(argc > 1) {
                open_files(num_files, 1, argc, argv, files);
                return 1;
            }
            return 0;
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
            files[i] = new_file;
            (*num_files)++;
        }
        else {
            fprintf(stderr, "Cannot open file: %s", strerror(errno));
        }
    }
}

char *get_line(FILE *file, char *line) {
    char *new_line = NULL;
    int size = 100;
    int c = 0;
    int i = 0;
    while((c = getc(file)) != EOF) {
        line[i++] = c;
        if(c == '\n') {
            break;
        }
        if(i == size - 1) {
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
                add_word(current_word, hash(current_word), hash_table);
            }
            free(current_word);
            current_word = NULL;
            current_word = (char *)malloc(sizeof(char) * strlen(line));
            j = 0;
        }
    }
    free(current_word);
}

unsigned long hash (const char* word) {
    int i;
    unsigned long hash;
    for (i = 0 ; word[i] != '\0' ; i++)
    {
        hash = 31 * hash + word[i];
    }
    return hash;
} 

int comparator(const void *p, const void *q) 
{
    if(p && q) {
        int l = ((Occurrence *)p)->frequency;
        int r = ((Occurrence *)q)->frequency;
        printf("%d vs %d\n", l, r);
        if(l == r) {
            printf("%s vs %s\n", ((Occurrence *)p)->word, ((Occurrence *)q)->word);
            return strcmp(((Occurrence *)p)->word, ((Occurrence *)q)->word);
        } 
        return (l - r);
    }
    return 0;
}

void print_table(HashTable *hash_table) {
    int i;
    // qsort(hash_table, hash_table->size, sizeof(Occurrence *), comparator);
    for(i = 0; i < hash_table->size; i++) {
        if(hash_table->table[i]) {
            printf("%s %d\n", hash_table->table[i]->word, i);
            free(hash_table->table[i]->word);
        }
        free(hash_table->table[i]);
    }
    free(hash_table->table);
}

int main(int argc, char *argv[]) {
    int i;
    int n = 0;
    int num_files = 0;
    int input;
    HashTable hash_table = init_table();
    FILE **files = (FILE **)malloc(sizeof(FILE) * (argc - 1));
    char *line = (char *)malloc(sizeof(char) * 100);
    input = parse_input(argc, &num_files, &n, argv, files);
    if(input> 0) {
        for(i = 0; i < num_files; i++) {
            while((line = get_line(files[i], line)) && line[0] != '\0') {
                get_words(line, &hash_table);
                memset(line, 0, sizeof(char) * 100);
            }
            fclose(files[i]);
        }
        free(files);
        free(line);
    }
    else {
        while((line = get_line(stdin, line)) && line[0] != '\0' && line[0] != '\n') {
            get_words(line, &hash_table);
            memset(line, 0, sizeof(char) * 100);
        }
        free(files);
        free(line);
    }
    print_table(&hash_table);
    return 0;
}

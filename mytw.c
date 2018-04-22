#include "mytw.h"

HashTable init_table() {
    int i;
    HashTable hash_table;
    for(i = 0; i < TABLESIZE; i++) {
        hash_table.table[i] = NULL;
    }
    hash_table.size = TABLESIZE;
    hash_table.items = 0;
    return hash_table;
}

void open_files(int argc, char *argv[], FILE **files) {
    int i;
    for(i = 1; i < argc; i++) {
        files[i - 1] = fopen(argv[i], "r");
    }
}

void get_line(FILE *file, char *line) {
    int size = 0;
    int c = 0;
    int i = 0;
    size = 100;
    while((c = getc(file)) != EOF) {
        line[i++] = c;
        if(c == '\n') {
            break;
        }
        if(i == size - 2) {
            line = (char *)realloc(line, sizeof(char) * size * 2);
            size *= 2;
        }
    }
    line[i] = '\0';
}

void get_words(char *line, HashTable hash_table) {
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
            printf("%s ", current_word);
            if(current_word[0] != '\n' || current_word[0] != '\0') {
                add_word(current_word, hash_table);
            }
            free(current_word);
            current_word = NULL;
            current_word = (char *)malloc(sizeof(char) * strlen(line));
            j = 0;
        }
    }
    free(current_word);
}

int hash (const char* word) {
    int i;
    unsigned int hash;
    hash = 0;
    for (i = 0 ; word[i] != '\0' ; i++)
    {
        hash = 31 * hash + word[i];
    }
    return hash;
}

void add_word(char *word, HashTable hash_table) {
    int hash_code;
    Occurence *new_word = (Occurence *)malloc(sizeof(Occurence));
    printf("%s ", word);
    new_word->word = word;
    new_word->frequency = 1;
    hash_code = hash(new_word->word);
    while(1) {
        if(hash_table.table[hash_code % hash_table.size] == 0) {
            hash_table.table[hash_code % hash_table.size] = new_word;
            break;
        }
        else if (hash_table.table[hash_code % hash_table.size] -> word == new_word->word){
            (hash_table.table[hash_code % hash_table.size])->frequency += 1;
            break;
        }
        else {
            hash_code ++;
        }
    }
}

void rehash(HashTable hash_table) {
    Occurence *newTable = (Occurence*) malloc(sizeof(Occurence) * hash_table.size * 2);

}

int main(int argc, char *argv[]) {
    int i;
    HashTable hash_table = init_table();
    FILE **files = (FILE **)malloc(sizeof(FILE *) * (argc - 1));
    char *line = (char *)malloc(sizeof(char) * 100);
    open_files(argc, argv, files);
    for(i = 1; i < argc; i++) {
        while(get_line(files[i - 1], line), 1 && line[0] != '\0') {
            get_words(line, hash_table);
            free(line);
            line = NULL;
            line = (char *)malloc(sizeof(char) * 100);
        }
        fclose(files[i - 1]);
    }
    free(files);
    free(line);
    return 0;
}

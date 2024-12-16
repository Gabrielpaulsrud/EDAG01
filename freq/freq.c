#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int len = 0;

typedef struct {
    char* key;
    int value;
} item;

int search_set(item* items, char* key) {
    for (int i=0; i<len; i++) {
        if (strcmp(items[i].key, key) == 0) {
            return 1;
        }
    }
    return 0;
}

int count_set(item* items, char* key) {
    for (int i=0; i<len; i++) {
        if (strcmp(items[i].key, key) == 0) {
            items[i].value += 1;
            return 1;
        }
    }
    return 0;
}

void add_set(item* items, char* key) {
    items[len].value = 1;
    items[len].key = key;
    len++;
}

int remove_set(item* items, char* key) {
    for (int i=0; i<len; i++) {
        if (strcmp(items[i].key, key) == 0) {
            free(items[i].key);
            items[i].value = items[len-1].value;
            items[i].key = items[len-1].key;
            len --;
            printf("trying to delete %s: deleted\n", key);
            return 1;
        }
    }
    printf("trying to delete %s: not found\n", key);
    return 0;
}

void print_items(item* items) {
    for (int i = 0; i < len; i++) {
        printf("Item %d: key = %s, value = %d\n", i, items[i].key, items[i].value);
    }
}

void print_result(item* items) {
    int largest_i = -1;
    int largest = -1;
    for (int i = 0; i < len; i++) {
        if (items[i].value > largest || (items[i].value == largest && strcmp(items[i].key, items[largest_i].key) < 0)) {
            largest_i = i;
            largest = items[i].value;
        }
    }
    if (largest_i != -1) {
        printf("result: %s %d\n", items[largest_i].key, largest);
    }
}

int IsPrime(unsigned int number) {
    if (number <= 1) return 0; // zero and one are not prime
    unsigned int i;
    for (i=2; i*i<=number; i++) {
        if (number % i == 0) return 0;
    }
    return 1;
}

int main(void) {
    char word[1024];             // Buffer for reading each word
    int c;                       // To handle character-by-character flushing of the input buffer
    int line = 1;

    item* items = calloc(sizeof(item), 400);

    while (scanf("%1023s", word) == 1) { // Read one word at a time


        if (IsPrime(line)){
            remove_set(items, word);
        }
        else {
            if (search_set(items, word)) {
                // char* saved_word = calloc(sizeof(char), 1024);
                // strcpy(saved_word, word);
                count_set(items, word);
                printf("counted %s\n", word);
                // print_items(items);
                // free(saved_word);
            }
            else {
                char* saved_word = calloc(sizeof(char), 1024);
                strcpy(saved_word, word);
                add_set(items, saved_word);
                printf("added %s\n", saved_word);
                // print_items(items);
            }
        }

        // Discard the rest of the line
        while ((c = getchar()) != '\n' && c != EOF) {
            // Skip remaining characters in the input buffer
        }
        line++;
    }
    // print_items(items);
    print_result(items);
  for (int i = 0; i < len; i++) {
        free(items[i].key);
    }  
  free(items);
    
    return 0;
}

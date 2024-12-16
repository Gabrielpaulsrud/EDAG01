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
            items[i].value = items[len-1].value;
            items[i].key = items[len-1].key;
            len --;
            printf("trying to delete %s: deleted\n", key);
            // free(items[i].key);
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
        printf("Result: %s %d\n", items[largest_i].key, items[largest_i].value);
    }
}

int IsPrime(unsigned int number) {
    if (number <= 1) return 0; // zero and one are not prime
    unsigned int i;
    for (i=2; i*i<=number; i++) {
        if (number % i == 0) return 0;
    }
    // printf("%d is prime\n", number);
    return 1;
}

int main(void) {
    char word[1024];             // Buffer for reading each word
    // char *words[1024];           // Array to store pointers to dynamically allocated words
    // int word_index = 0;          // Counter for the number of words
    int c;                       // To handle character-by-character flushing of the input buffer
    int line = 1;

    // printf("Enter text (Ctrl+D to quit):\n");
    item* items = calloc(sizeof(item), 400);

    // add_set(items, "aaa");
    // add_set(items, "bbb");
    // print_items(items);
    // remove_set(items, "bbb");
    // count_set(items, "aaa");
    // print_items(items);
    // Read words in a loop
    while (scanf("%1023s", word) == 1) { // Read one word at a time


        if (IsPrime(line)){
            // char* saved_word = calloc(sizeof(char), 1024);
            // strcpy(saved_word, word);
            remove_set(items, word);
            // free(saved_word);
            // print_items(items);
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
    free(items);
    
    return 0;
}

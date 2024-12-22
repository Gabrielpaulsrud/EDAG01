#include <stdio.h>
#include <string.h>
#include <ctype.h>


// poly_t* new_poly_from_string(const char* string) {

//     int c;
//     int exp;
//     int result;
//     char x;
//     char hat;

//     int multiplier;
//     char* copy = strdup(string);
//     int poly_len = -1;
//     poly_t* p = NULL;
    
//     char* token;
//     int normal = 0;
//     token = strtok(copy, " ");
//     while( token != NULL ) {
//         if (token[0] == '-') {
//             multiplier = -1;
//         }
//         else {
//             multiplier = 1;
//         }
//         if (normal){
//             token = strtok(NULL, " ");
//         }
//         else {
//             normal = 1;
//         }
//         exp = 0;
//         c = 1;
//         result = sscanf(token, "%d%c%c%d", &c, &x, &hat, &exp);
//         if (result == 2) {
//             exp = 1;
//         }
//         else if (result == 0) {
//             result = sscanf(token, "%c%c%d", &x, &hat, &exp);
//             if (result == 1) {
//                 exp = 1;
//             }
//         }
//         // printf("%dx^%d\n", multiplier*c, exp);
//         if(poly_len == -1){
//             poly_len = exp+1;
//             size_t total_size = sizeof(poly_t) + poly_len * sizeof(int32_t);
//             p = calloc(1, total_size);
//             p->polys = (int32_t*)((char*)p + sizeof(poly_t));

//             p->len = poly_len;
//         }
//         // printf("constructing: %dx^%d\n", c, exp);
//         p->polys[exp] = multiplier*c;
//         token = strtok(NULL, " ");
//     }
//     return p;
// }


int count_until_whitespace(const char* str) {
    const char* start = str;
    while (*str && *str != ' ') {
        str++;
    }
    return str - start; // Difference gives the count
}

int main(){
  
    const char* input = "-1234x^5678 + 333x^888 - 2x + 5 - x^10 + x";
    const char *a = &input[0];
    // while(a!='\0') {
    if (a == input) {
        printf("is equal");
    }
    printf("hej");
    printf("%c", *a);
    a++;
    printf("%c", *a);
    a++;
    printf("%c", *a);
    a++;
    printf("%c", *a);

    if (*a == ' ') {
        a ++;
    }
    // a = string[i];
    if (*a == '-') {
        // multiplier = -1;
        if (a == input){
            a++;
            printf("%c", *a);
        }
        else {
            a+=2;
            printf("%c", *a);
            
        }
    }
    else if (*a == '+') {
        a+=2;
    }
    printf("\n");
    return 0;
}

// #includ
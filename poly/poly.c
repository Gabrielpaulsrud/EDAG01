#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>



#include "error.h"
#include "poly.h"

struct poly_t{
    int32_t len;       // Number of elements in the array
    int32_t* polys;   // Flexible array member
};

typedef struct poly_t poly_t;

static inline int32_t abs_int(int32_t x) {
    int32_t mask = x >> (sizeof(int32_t) * 8 - 1);  // Create a mask of all 1s if x < 0, else all 0s
    return (x + mask) ^ mask;
}

poly_t* new_poly_from_string(const char* string) {
    uint16_t i = 0;
    char a;
    int32_t poly_len = -1;
    poly_t* p;
    
    int32_t c = 0;
    int8_t multiplier;
    
    int32_t exp = 0;
    a = string[i];
    while(a!='\0') {
        multiplier = 1;
        exp = 0;
        c = 0;
        if (a == ' ') {
            i ++;
        }
        a = string[i];
        if (a == '-') {
            multiplier = -1;
            if (i == 0){
                i++;
            }
            else {
                i+=2;
            }
        }
        else if (a == '+') {
            i+=2;
        }
        
        while ((a = string[i]) != 'x' && a != '\0') {
            c = c * 10 + (a - '0');
            i++;
        }
        if (a == 'x'){
            i++;
            a = string[i];
            exp = 1;
            if (a == '^') {
                exp = 0;
                i++;
                while ((a = string[i]) != ' ') {  
                    exp = exp * 10 + (a - '0');
                    i++;
                }
            }
        }
        if (c == 0 && exp > 0){
            c = 1;
        }
        if(poly_len == -1){
            poly_len = exp+1;
            // p = malloc(sizeof(poly_t));
            size_t total_size = sizeof(poly_t) + poly_len * sizeof(int32_t);
            p = malloc(total_size);
            memset(p, 0, total_size);
            p->polys = (int32_t*)((char*)p + sizeof(poly_t));

            p->len = poly_len;
            // p->polys = p + sizeof(poly_t);
            
            // p->polys = calloc(poly_len, sizeof(int32_t));
        }
        // printf("constructing: %dx^%d\n", c, exp);
        p->polys[exp] = multiplier*c;
        // printf("poly[0] = %d\n", p->polys[0]);
        // printf("poly[1] = %d\n", p->polys[1]);
        // printf("poly[2] = %d\n", p->polys[2]);
        // printf("poly[3] = %d\n", p->polys[3]);
    }
    return p;
}

void free_poly(poly_t* p) {
    free(p->polys);
    free(p);
}

poly_t*	mul(poly_t* p, poly_t*q) {
    poly_t* r = malloc(sizeof(poly_t));
    r->len = p->len+q->len-1;
    r->polys = calloc(r->len, sizeof(int));
    for (uint32_t i = 0; i < p->len; i++) {
        for (uint32_t j = 0; j < q->len; j++) {
            r->polys[i+j] += p->polys[i] * q->polys[j];;
        }
    }
    return r;
}

void print_poly(poly_t* p) {
    // printf("printing\n");
    // printf("poly[0] = %d\n", p->polys[0]);
    // printf("poly[1] = %d\n", p->polys[1]);
    // printf("poly[2] = %d\n", p->polys[2]);
    // printf("poly[3] = %d\n", p->polys[3]);
    for (int32_t i = p->len-1; i >= 0; i--) {
        int32_t c = p->polys[i];
        int32_t abs_c = abs_int(c);
        // printf("actual = %dx^%d\n", c, exp);
        if (c == 0){
            continue;
        }
        if (i < p->len-1) {
            printf(" ");
        }
        if (c < 0) {
            printf("- ");
        }
        else if (i < p->len-1) {
            printf("+ ");
        }
        if (abs_c > 1 || i == 0) {
            printf("%d", abs_c);
        }
        if (i >= 1){
            printf("x");
        }
        if (i > 1)
        {
            printf("^%d", i);
        }
    }
    printf("\n");
}

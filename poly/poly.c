#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>

#include "error.h"
#include "poly.h"

struct poly_t{
    int32_t len;
    int32_t* polys;
};

typedef struct poly_t poly_t;

static int32_t abs_int(int32_t x) {
    int32_t mask = x >> (sizeof(int32_t) * 8 - 1);  // Create a mask of all 1s if x < 0, else all 0s
    return (x + mask) ^ mask;
}

poly_t* new_poly_from_string(const char* string) {
    const char* a = &string[0];
    poly_t* p = NULL;
    int32_t poly_len = -1;
    int32_t c;
    int32_t exp;
    int8_t multiplier;
    
    outer:
        multiplier = 1;
        exp = 0;
        c = 0;
        if (*a == ' ') {
            a ++;
        }
        if (*a == '-') {
            multiplier = -1;
            if (a == string){
                a++;
            }
            else {
                a+=2;
            }
        }
        else if (*a == '+') {
            a+=2;
        }
        
        while (*a != 'x' && *a != '\0') {
            c = c * 10 + (*a - '0');
            a++;
        }
        if (*a == 'x'){
            a++;
            exp = 1;
            if (*a == '^') {
                exp = 0;
                a++;
                while (*a != ' ') {  
                    exp = exp * 10 + (*a - '0');
                    a++;
                }
            }
        }

        if (c == 0 && exp > 0){
            c = 1;
        }
        if(poly_len == -1){
            poly_len = exp+1;
            size_t total_size = sizeof(poly_t) + poly_len * sizeof(int32_t);
            p = calloc(1, total_size);
            p->polys = (int32_t*)((char*)p + sizeof(poly_t));

            p->len = poly_len;
        }
        // printf("constructing: %dx^%d\n", c, exp);
        p->polys[exp] = multiplier*c;
        // printf("poly[0] = %d\n", p->polys[0]);
        // printf("poly[1] = %d\n", p->polys[1]);
        // printf("poly[2] = %d\n", p->polys[2]);
        // printf("poly[3] = %d\n", p->polys[3]);
    // }
    if (*a!='\0'){
        goto outer;
    }

    return p;
}



void free_poly(poly_t* p) {
    free(p);
}

poly_t*	mul(poly_t* p, poly_t*q) {
    int32_t poly_len = p->len+q->len-1;
    size_t total_size = sizeof(poly_t) + poly_len * sizeof(int32_t);
    poly_t* r = calloc(1, total_size);
    r->polys = (int32_t*)((char*)r + sizeof(poly_t));
    r->len = poly_len;
  
    int i = 0; 
    int j = 0;
    outer:
        j = 0;
        inner:
            r->polys[i+j] += p->polys[i] * q->polys[j];
            j++;
            if (j<q->len) {
                goto inner;
            }
        i++;
        if (i<p->len) {
            goto outer;
        }    
    return r;
}

void print_poly(poly_t* p) {
    // printf("printing\n");
    // printf("poly[0] = %d\n", p->polys[0]);
    // printf("poly[1] = %d\n", p->polys[1]);
    // printf("poly[2] = %d\n", p->polys[2]);
    // printf("poly[3] = %d\n", p->polys[3]);
    // for (int32_t i = p->len-1; i >= 0; i--) {
    int32_t i = p->len-1;
    int32_t c;
    int32_t abs_c;
    outer:
        c = p->polys[i];
        abs_c = abs_int(c);
        // printf("actual = %dx^%d\n", c, exp);
        if (c == 0){
            goto evaluate;
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
        evaluate:
        i--;
        if (i >= 0) {
            goto outer;
        }
    // }
    printf("\n");
}

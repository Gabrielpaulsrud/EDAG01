#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#include "error.h"
#include "poly.h"

struct poly_part_t {
    int c;
    int exp;
};

typedef struct poly_part_t poly_part_t;

struct poly_t {
    int len;
    poly_part_t* polys;
};

typedef struct poly_t poly_t;

static inline int abs_int(int x) {
    int mask = x >> (sizeof(int) * 8 - 1);  // Create a mask of all 1s if x < 0, else all 0s
    return (x + mask) ^ mask;
}

void add_poly(poly_t* poly, poly_part_t* poly_part) {
    int found_i = -1;
    poly_part_t tmp;
    poly_part_t insert_tmp;
    for (int i=0; i<poly->len; i++) {
        if (poly->polys[i].exp == poly_part->exp) {
            poly->polys[i].c += poly_part->c;
            return;
        }
        else if (poly_part->exp > poly->polys[i].exp) {
            if (found_i==-1){
                found_i = i;
                printf("found_i: %d", found_i);
                insert_tmp = poly->polys[i];
            }
            tmp = poly->polys[i+1];
            poly->polys[i+1] = insert_tmp;
            insert_tmp = tmp;
            // for (int j=poly->len-1; j>=i; j--){
            //     poly->polys[j+1] = poly->polys[j];
            // }
        }
    }
    if (found_i!=-1) {
        poly->polys[found_i] = *poly_part;
        poly->len++;
        return;
    }
    poly->polys[poly->len] = *poly_part;
    poly->len++;
    return;
}

void print_poly(poly_t* p) {
    for (int i = 0; i < p->len; i++) {
        int c = p->polys[i].c;
        int exp = p->polys[i].exp;
        int abs_c = abs_int(c);
        if (c == 0){
            continue;
        }
        if (i > 0) {
            printf(" ");
        }
        if (c < 0) {
            printf("- ");
        }
        else if (i > 0) {
            printf("+ ");
        }
        if (abs_c > 1 || exp == 0) {
            printf("%d", abs_c);
        }
        if (exp >= 1){
            printf("x");
        }
        if (exp > 1)
        {
            printf("^%d", exp);
        }
    }
    printf("\n");
}

int main (){
    poly_part_t polys[5]; 
    poly_t poly;
    poly.len = 3;
    poly.polys = polys;
    poly_part_t a;
    a.c = 1;
    a.exp = 10;
    polys[0] = a;
    
    poly_part_t b;
    b.c = 1;
    b.exp = 2;
    polys[1] = b;


    poly_part_t c;
    c.c = 1;
    c.exp = 0;
    polys[2] = c;

    
    print_poly(&poly);

    poly_part_t d;
    d.c = 1;
    d.exp = 9;

    add_poly(&poly, &d);
    print_poly(&poly);
    return 0;
}